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

spi_nand::spi_nand(Chips *chips, fel *fels) : spi(chips, fels) {
    spi_init(&pdata.swap_buf, &pdata.swap_len, &pdata.cmd_len);

}

bool spi_nand::get_spi_nand_info() {
    const spinand_info_t *info;
    uint8_t tx[2], rx[4];

    tx[0] = SPI_NAND_OPCODE::OPCODE_RDID;
    tx[1] = 0x0;

    try {
        spi_xfer(pdata.swap_buf, pdata.cmd_len, pdata.swap_len, tx, 2, rx, 4);
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
        spi_xfer(pdata.swap_buf, pdata.cmd_len, pdata.swap_len, tx, 1, rx, 4);
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
        spi_xfer(pdata.swap_buf, pdata.swap_len, pdata.cmd_len, tx, 1, 0, 0);
    }
    catch (const std::runtime_error &e) {
        qDebug() << e.what();
    }
}

void spi_nand::spi_nand_get_feature(uint8_t addr, uint8_t *val) {
    uint8_t tx[2];

    tx[0] = OPCODE_GET_FEATURE;
    tx[1] = addr;
    try {
        spi_xfer(pdata.swap_buf, pdata.swap_len, pdata.cmd_len, tx, 2, val, 1);
    } catch (const std::runtime_error &e) {
        qDebug() << e.what();
    }
}

void spi_nand::spi_nand_set_feature(uint8_t addr, uint8_t val) {
    uint8_t tx[3];

    tx[0] = OPCODE_GET_FEATURE;
    tx[1] = addr;
    tx[2] = val;
    try {
        spi_xfer(pdata.swap_buf, pdata.swap_len, pdata.cmd_len, tx, 3, 0, 0);
    } catch (const std::runtime_error &e) {
        qDebug() << e.what();
    }
}

void spi_nand::spi_nand_wait_for_busy() {
    uint8_t cbuf[256];
    uint32_t clen = 0;

    cbuf[clen++] = SPI_CMD_SELECT;
    cbuf[clen++] = SPI_CMD_SPINAND_WAIT;
    cbuf[clen++] = SPI_CMD_DESELECT;
    cbuf[clen++] = SPI_CMD_END;

    if (clen <= pdata.cmd_len) {
        fel_->fel_chip_spi_run(cbuf, clen);
        return 1;
    }
}

