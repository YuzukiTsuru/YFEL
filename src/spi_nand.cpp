/*
 * Copyright (c) 2022, YuzukiTsuru <GloomyGhost@GloomyGhost.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * See README and LICENSE for more details.
 */

//
// Created by gloom on 2022/9/27.
//

#include "spi_nand.h"
#include "x.h"

spi_nand::spi_nand(Chips *chips, fel *fels) {
    spi_ = new(std::nothrow) spi(chips, fels);

    if (spi_ == nullptr)
        exit(0xff);

    spi_->spi_init(&pdata.swap_buf, &pdata.swap_len, &pdata.cmd_len);
}

spi_nand::~spi_nand() {
    delete spi_;
}

void spi_nand::init() {
    spi_nand_init();
}

QString spi_nand::get_spi_nand_name() const {
    return pdata.info.name;
}

uint64_t spi_nand::get_spi_nand_size() {
    pdata.info.nand_size = pdata.info.page_size * pdata.info.pages_per_block
                           * pdata.info.blocks_per_die * pdata.info.ndies;
    return pdata.info.nand_size;
}

void spi_nand::erase(uint64_t addr, uint64_t len) {
    // init spi nand memory
    spi_nand_init();
    auto esize = pdata.info.page_size * pdata.info.pages_per_block;
    auto emask = esize - 1;
    auto base = addr & ~emask;
    auto cnt = (addr & emask) + len;
    cnt = (cnt + ((cnt & emask) ? esize : 0)) & ~emask;

    uint64_t n;
    while (cnt > 0) {
        n = cnt > esize ? esize : cnt;
        spi_nand_erase(base, n);
        base += n;
        cnt -= n;
    }
}

bool spi_nand::get_spi_nand_info() {
    uint8_t tx[2], rx[4];

    tx[0] = SPI_NAND_OPCODE::OPCODE_RDID;
    tx[1] = 0x0;

    try {
        spi_->spi_xfer(pdata.swap_buf, pdata.cmd_len, pdata.swap_len, tx, 2, rx, 4);
    } catch (const std::runtime_error &e) {
        qDebug() << e.what();
    }

    for (const auto &i: spinand_info) {
        if (memcmp(rx, i.id.val, i.id.len) == 0) {
            pdata.info = i;
            return true;
        }
    }

    // Try OPCODE
    tx[0] = SPI_NAND_OPCODE::OPCODE_RDID;
    try {
        spi_->spi_xfer(pdata.swap_buf, pdata.cmd_len, pdata.swap_len, tx, 1, rx, 4);
    } catch (const std::runtime_error &e) {
        qDebug() << e.what();
    }
    for (const auto &i: spinand_info) {
        if (memcmp(rx, i.id.val, i.id.len) == 0) {
            pdata.info = i;
            return true;
        }
    }

    // Do not support
    return false;
}

void spi_nand::spi_nand_reset() {
    uint8_t tx[1] = {SPI_NAND_OPCODE::OPCODE_RESET};

    try {
        spi_->spi_xfer(pdata.swap_buf, pdata.swap_len, pdata.cmd_len,
                       tx, 1, nullptr, 0);
    }
    catch (const std::runtime_error &e) {
        qDebug() << e.what();
    }
}

void spi_nand::spi_nand_get_feature(uint8_t addr, uint8_t *val) {
    uint8_t tx[2];

    tx[0] = SPI_NAND_OPCODE::OPCODE_GET_FEATURE;
    tx[1] = addr;
    try {
        spi_->spi_xfer(pdata.swap_buf, pdata.swap_len, pdata.cmd_len, tx, 2,
                       val, 1);
    } catch (const std::runtime_error &e) {
        qDebug() << e.what();
    }
}

void spi_nand::spi_nand_set_feature(uint8_t addr, uint8_t val) {
    uint8_t tx[3];

    tx[0] = SPI_NAND_OPCODE::OPCODE_GET_FEATURE;
    tx[1] = addr;
    tx[2] = val;
    try {
        spi_->spi_xfer(pdata.swap_buf, pdata.swap_len, pdata.cmd_len, tx, 3,
                       nullptr, 0);
    } catch (const std::runtime_error &e) {
        qDebug() << e.what();
    }
}

void spi_nand::spi_nand_wait_for_busy() {
    uint8_t cbuf[256];
    uint32_t clen = 0;

    cbuf[clen++] = chip_spi_ctrl_e::SPI_CMD_SELECT;
    cbuf[clen++] = chip_spi_ctrl_e::SPI_CMD_SPINAND_WAIT;
    cbuf[clen++] = chip_spi_ctrl_e::SPI_CMD_DESELECT;
    cbuf[clen++] = chip_spi_ctrl_e::SPI_CMD_END;

    if (clen <= pdata.cmd_len) {
        spi_->get_current_chip()->chip_spi_run(cbuf, clen);
    }
}

void spi_nand::spi_nand_init() {
    uint8_t val;

    spi_->get_current_chip()->chip_spi_init(&pdata.swap_buf, &pdata.swap_len, &pdata.cmd_len);
    if (get_spi_nand_info()) {
        spi_nand_reset();
        spi_nand_wait_for_busy();
        spi_nand_get_feature(SPI_NAND_OPCODE::OPCODE_FEATURE_PROTECT, &val);
        if (val != 0x0) {
            spi_nand_set_feature(SPI_NAND_OPCODE::OPCODE_FEATURE_PROTECT, 0x0);
            spi_nand_wait_for_busy();
        }
    }
}

void spi_nand::spi_nand_erase(uint64_t addr, uint64_t count) {
    auto esize = pdata.info.page_size * pdata.info.pages_per_block;
    auto emask = esize - 1;
    auto base = addr & ~emask;
    auto cnt = (addr & emask) + count;
    cnt = (cnt + ((cnt & emask) ? esize : 0)) & ~emask;

    uint8_t cbuf[256];
    uint32_t clen, pa;
    while (cnt > 0) {
        pa = base / pdata.info.page_size;
        clen = 0;
        cbuf[clen++] = chip_spi_ctrl_e::SPI_CMD_SELECT;
        cbuf[clen++] = chip_spi_ctrl_e::SPI_CMD_FAST;
        cbuf[clen++] = 1;
        cbuf[clen++] = SPI_NAND_OPCODE::OPCODE_WRITE_ENABLE;
        cbuf[clen++] = chip_spi_ctrl_e::SPI_CMD_DESELECT;
        cbuf[clen++] = chip_spi_ctrl_e::SPI_CMD_SELECT;
        cbuf[clen++] = chip_spi_ctrl_e::SPI_CMD_SPINAND_WAIT;
        cbuf[clen++] = chip_spi_ctrl_e::SPI_CMD_DESELECT;
        cbuf[clen++] = chip_spi_ctrl_e::SPI_CMD_SELECT;
        cbuf[clen++] = chip_spi_ctrl_e::SPI_CMD_FAST;
        cbuf[clen++] = 4;
        cbuf[clen++] = SPI_NAND_OPCODE::OPCODE_BLOCK_ERASE;
        cbuf[clen++] = (uint8_t) (pa >> 16);
        cbuf[clen++] = (uint8_t) (pa >> 8);
        cbuf[clen++] = (uint8_t) (pa >> 0);
        cbuf[clen++] = chip_spi_ctrl_e::SPI_CMD_DESELECT;
        cbuf[clen++] = chip_spi_ctrl_e::SPI_CMD_SELECT;
        cbuf[clen++] = chip_spi_ctrl_e::SPI_CMD_SPINAND_WAIT;
        cbuf[clen++] = chip_spi_ctrl_e::SPI_CMD_DESELECT;
        cbuf[clen++] = chip_spi_ctrl_e::SPI_CMD_END;
        if (clen <= pdata.cmd_len)
            spi_->get_current_chip()->chip_spi_run(cbuf, clen);
        base += esize;
        cnt -= esize;
    }
}

