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
// Created by gloom on 2022/9/28.
//

#include <QMessageBox>
#include <QFontDatabase>

#include "exceptions.h"
#include "spi_nor.h"

spi_nor::spi_nor(Chips *chips, fel *fels) {
    // Init spi handler
    spi_ = new spi(chips, fels);

    // init spi peripheral
    spi_->spi_init(&pdata.swap_buf, &pdata.swap_len, &pdata.cmd_len);
}

spi_nor::~spi_nor() {
    delete spi_;
}

void spi_nor::init() {
    qDebug() << "Init SPI NOR";
    QFutureWatcher<void> watcher;
    connect(&watcher, &QFutureWatcher<void>::finished, &watcher, &QFutureWatcher<void>::deleteLater);

    // init spi pref
    watcher.setFuture(QtConcurrent::run([=]() mutable {
        spi_nor_init();
    }));
    watcher.waitForFinished();
}

QString spi_nor::get_spi_nor_name() const {
    return pdata.info.name;
}

uint64_t spi_nor::get_spi_nor_size() const {
    return pdata.info.capacity;
}

void spi_nor::spi_nor_init() {
    uint8_t cbuf[256];
    uint32_t clen = 0;

    try {
        spi_->spi_init(&pdata.swap_buf, &pdata.swap_len, &pdata.cmd_len);
        if (get_spi_nor_info()) {
            // spi select 
            cbuf[clen++] = chip_spi_ctrl_e::SPI_CMD_SELECT;
            // chip reset 
            cbuf[clen++] = chip_spi_ctrl_e::SPI_CMD_FAST;
            cbuf[clen++] = 2;
            cbuf[clen++] = 0x66;
            cbuf[clen++] = 0x99;
            // spi deselect 
            cbuf[clen++] = chip_spi_ctrl_e::SPI_CMD_DESELECT;

            // spi select 
            cbuf[clen++] = chip_spi_ctrl_e::SPI_CMD_SELECT;
            // wait busy 
            cbuf[clen++] = chip_spi_ctrl_e::SPI_CMD_SPINOR_WAIT;
            // spi deselect 
            cbuf[clen++] = chip_spi_ctrl_e::SPI_CMD_DESELECT;

            // spi select 
            cbuf[clen++] = chip_spi_ctrl_e::SPI_CMD_SELECT;
            // write enable 
            cbuf[clen++] = chip_spi_ctrl_e::SPI_CMD_FAST;
            cbuf[clen++] = 1;
            cbuf[clen++] = pdata.info.opcode_write_enable;
            // spi deselect 
            cbuf[clen++] = chip_spi_ctrl_e::SPI_CMD_DESELECT;

            // spi select 
            cbuf[clen++] = chip_spi_ctrl_e::SPI_CMD_SELECT;
            // write status 
            cbuf[clen++] = chip_spi_ctrl_e::SPI_CMD_FAST;
            cbuf[clen++] = 2;
            cbuf[clen++] = SPI_NOR_OPS::NOR_OPCODE_WRSR;
            cbuf[clen++] = 0;
            // spi deselect 
            cbuf[clen++] = chip_spi_ctrl_e::SPI_CMD_DESELECT;

            // spi select 
            cbuf[clen++] = chip_spi_ctrl_e::SPI_CMD_SELECT;
            // wait busy 
            cbuf[clen++] = chip_spi_ctrl_e::SPI_CMD_SPINOR_WAIT;
            // spi deselect 
            cbuf[clen++] = chip_spi_ctrl_e::SPI_CMD_DESELECT;

            if (pdata.info.address_length == 4) {
                // spi select 
                cbuf[clen++] = chip_spi_ctrl_e::SPI_CMD_SELECT;
                // write enable 
                cbuf[clen++] = chip_spi_ctrl_e::SPI_CMD_FAST;
                cbuf[clen++] = 1;
                cbuf[clen++] = pdata.info.opcode_write_enable;
                // spi deselect 
                cbuf[clen++] = chip_spi_ctrl_e::SPI_CMD_DESELECT;

                // spi select 
                cbuf[clen++] = chip_spi_ctrl_e::SPI_CMD_SELECT;
                // entern 4byte address 
                cbuf[clen++] = chip_spi_ctrl_e::SPI_CMD_FAST;
                cbuf[clen++] = 1;
                cbuf[clen++] = SPI_NOR_OPS::NOR_OPCODE_ENTER_4B;
                // spi deselect 
                cbuf[clen++] = chip_spi_ctrl_e::SPI_CMD_DESELECT;

                // spi select 
                cbuf[clen++] = chip_spi_ctrl_e::SPI_CMD_SELECT;
                // wait busy 
                cbuf[clen++] = chip_spi_ctrl_e::SPI_CMD_SPINOR_WAIT;
                // spi deselect 
                cbuf[clen++] = chip_spi_ctrl_e::SPI_CMD_DESELECT;
            }

            // end
            cbuf[clen++] = chip_spi_ctrl_e::SPI_CMD_END;
            if (clen <= pdata.cmd_len) {
                spi_->get_current_chip()->chip_spi_run(cbuf, clen);
            }
        }else {

        }
    } catch (const function_not_implemented &e) {
        throw e;
    }
}

bool spi_nor::spi_nor_read_sfdp() {
    uint32_t addr;
    uint8_t tx[5];

    // generate sfdp tx command
    tx[0] = SPI_NOR_OPS::NOR_OPCODE_SFDP;
    tx[1] = 0x0;
    tx[2] = 0x0;
    tx[3] = 0x0;
    tx[4] = 0x0;

    // read sfdp header
    spi_->spi_xfer(pdata.swap_buf, pdata.swap_len, pdata.cmd_len, tx, 5,
                   (uint8_t *) &sfdp.header, sizeof(sfdp_header_t));

    // check sfdp header, if not match, return false
    if (sfdp.header.sign[0] != 'S' || sfdp.header.sign[1] != 'F' || sfdp.header.sign[2] != 'D' ||
        sfdp.header.sign[3] != 'P') {
        return false;
    }

    // check sfdp nph size, if > SFDP_MAX_NPH, check to SFDP_MAX_NPH
    if (sfdp.header.nph > SFDP_MAX_NPH) {
        sfdp.header.nph = sfdp.header.nph + 1;
    } else {
        sfdp.header.nph = SFDP_MAX_NPH;
    }

    // read sfdp parameter header
    for (int j = 0; j < sfdp.header.nph; ++j) {
        addr = j * sizeof(sfdp_parameter_header_t) + sizeof(sfdp_header_t);
        tx[0] = SPI_NOR_OPS::NOR_OPCODE_SFDP;
        tx[1] = (addr >> 16) & 0xff;
        tx[2] = (addr >> 8) & 0xff;
        tx[3] = (addr >> 0) & 0xff;
        tx[4] = 0x0;
        spi_->spi_xfer(pdata.swap_buf, pdata.swap_len, pdata.cmd_len, tx, 5,
                       (uint8_t *) &sfdp.parameter_header[j], sizeof(sfdp_parameter_header_t));
    }

    // check for basic table major and minor
    for (int j = 0; j < sfdp.header.nph; ++j) {
        if ((sfdp.parameter_header[j].idlsb == 0x00) && (sfdp.parameter_header[j].idmsb == 0xff)) {
            addr = (sfdp.parameter_header[0].ptp[0] << 0) | (sfdp.parameter_header[0].ptp[1] << 8) |
                   (sfdp.parameter_header[0].ptp[2] << 16);
            tx[0] = SPI_NOR_OPS::NOR_OPCODE_SFDP;
            tx[1] = (addr >> 16) & 0xff;
            tx[2] = (addr >> 8) & 0xff;
            tx[3] = (addr >> 0) & 0xff;
            tx[4] = 0x0;
            spi_->spi_xfer(pdata.swap_buf, pdata.swap_len, pdata.cmd_len, tx, 5,
                           (uint8_t *) &sfdp.basic_table.table[0], sfdp.parameter_header[0].length * 4);
            sfdp.basic_table.major = sfdp.parameter_header[j].major;
            sfdp.basic_table.minor = sfdp.parameter_header[j].minor;
            return true;
        }
    }
    return false;
}

uint32_t spi_nor::spi_nor_read_id() {
    uint8_t tx[1];
    uint8_t rx[3];
    tx[0] = SPI_NOR_OPS::NOR_OPCODE_RDID;
    spi_->spi_xfer(pdata.swap_buf, pdata.swap_len, pdata.cmd_len, tx, 1, rx, 3);
    return (rx[0] << 16) | (rx[1] << 8) | (rx[2] << 0);
}

bool spi_nor::get_spi_nor_info() {
    // read spi nor sfdp
    if (spi_nor_read_sfdp()) {
        spi_nor_sfdp_handler();
        // find spi nor sfdp device
        return true;
    } else {
        // check spi nor id
        auto id = spi_nor_read_id();
        if (id == 0xffffff || id == 0) {
            // empty spi nor
            return false;
        } else {
            // find spi nor id device in spi_nor_info_table
            for (const auto &i: spi_nor_info_table) {
                if (i.id == id) {
                    pdata.info = i;
                    return true;
                }
                qDebug() << "id:" << id;
            }
            // unknown spi nor
            return false;
        }
    }
}

void spi_nor::spi_nor_sfdp_handler() {
    pdata.info.name = "SFDP";
    pdata.info.id = 0;
    // Basic flash parameter table 2th dword
    auto v = (sfdp.basic_table.table[7] << 24) | (sfdp.basic_table.table[6] << 16) | (sfdp.basic_table.table[5] << 8) |
             (sfdp.basic_table.table[4] << 0);
    if (v & (1 << 31)) {
        v &= 0x7fffffff;
        pdata.info.capacity = 1 << (v - 3);
    } else {
        pdata.info.capacity = (v + 1) >> 3;
    }
    // Basic flash parameter table 1th dword
    v = (sfdp.basic_table.table[3] << 24) | (sfdp.basic_table.table[2] << 16) | (sfdp.basic_table.table[1] << 8) |
        (sfdp.basic_table.table[0] << 0);

    if ((pdata.info.capacity <= (16 * 1024 * 1024)) && (((v >> 17) & 0x3) != 0x2))
        pdata.info.address_length = 3;
    else
        pdata.info.address_length = 4;
    if (((v >> 0) & 0x3) == 0x1)
        pdata.info.opcode_erase_4k = (v >> 8) & 0xff;
    else
        pdata.info.opcode_erase_4k = 0x00;
    pdata.info.opcode_erase_32k = 0x00;
    pdata.info.opcode_erase_64k = 0x00;
    pdata.info.opcode_erase_256k = 0x00;

    // Basic flash parameter table 8th dword
    v = (sfdp.basic_table.table[31] << 24) | (sfdp.basic_table.table[30] << 16) |
        (sfdp.basic_table.table[29] << 8) | (sfdp.basic_table.table[28] << 0);

    switch ((v >> 0) & 0xff) {
        case 12:
            pdata.info.opcode_erase_4k = (v >> 8) & 0xff;
            break;
        case 15:
            pdata.info.opcode_erase_32k = (v >> 8) & 0xff;
            break;
        case 16:
            pdata.info.opcode_erase_64k = (v >> 8) & 0xff;
            break;
        case 18:
            pdata.info.opcode_erase_256k = (v >> 8) & 0xff;
            break;
        default:
            break;
    }
    switch ((v >> 16) & 0xff) {
        case 12:
            pdata.info.opcode_erase_4k = (v >> 24) & 0xff;
            break;
        case 15:
            pdata.info.opcode_erase_32k = (v >> 24) & 0xff;
            break;
        case 16:
            pdata.info.opcode_erase_64k = (v >> 24) & 0xff;
            break;
        case 18:
            pdata.info.opcode_erase_256k = (v >> 24) & 0xff;
            break;
        default:
            break;
    }
    // Basic flash parameter table 9th dword
    v = (sfdp.basic_table.table[35] << 24) | (sfdp.basic_table.table[34] << 16) |
        (sfdp.basic_table.table[33] << 8) | (sfdp.basic_table.table[32] << 0);

    switch ((v >> 0) & 0xff) {
        case 12:
            pdata.info.opcode_erase_4k = (v >> 8) & 0xff;
            break;
        case 15:
            pdata.info.opcode_erase_32k = (v >> 8) & 0xff;
            break;
        case 16:
            pdata.info.opcode_erase_64k = (v >> 8) & 0xff;
            break;
        case 18:
            pdata.info.opcode_erase_256k = (v >> 8) & 0xff;
            break;
        default:
            break;
    }
    switch ((v >> 16) & 0xff) {
        case 12:
            pdata.info.opcode_erase_4k = (v >> 24) & 0xff;
            break;
        case 15:
            pdata.info.opcode_erase_32k = (v >> 24) & 0xff;
            break;
        case 16:
            pdata.info.opcode_erase_64k = (v >> 24) & 0xff;
            break;
        case 18:
            pdata.info.opcode_erase_256k = (v >> 24) & 0xff;
            break;
        default:
            break;
    }
    if (pdata.info.opcode_erase_4k != 0x00)
        pdata.info.blksz = 4096;
    else if (pdata.info.opcode_erase_32k != 0x00)
        pdata.info.blksz = 32768;
    else if (pdata.info.opcode_erase_64k != 0x00)
        pdata.info.blksz = 65536;
    else if (pdata.info.opcode_erase_256k != 0x00)
        pdata.info.blksz = 262144;
    pdata.info.opcode_write_enable = NOR_OPCODE_WREN;
    pdata.info.read_granularity = 1;
    pdata.info.opcode_read = SPI_NOR_OPS::NOR_OPCODE_READ;

    if ((sfdp.basic_table.major == 1) && (sfdp.basic_table.minor < 5)) {
        // Basic flash parameter table 1th dword
        v = (sfdp.basic_table.table[3] << 24) | (sfdp.basic_table.table[2] << 16) |
            (sfdp.basic_table.table[1] << 8) | (sfdp.basic_table.table[0] << 0);
        if ((v >> 2) & 0x1)
            pdata.info.write_granularity = 64;
        else
            pdata.info.write_granularity = 1;
    } else if ((sfdp.basic_table.major == 1) && (sfdp.basic_table.minor >= 5)) {
        // Basic flash parameter table 11th dword
        v = (sfdp.basic_table.table[43] << 24) | (sfdp.basic_table.table[42] << 16) |
            (sfdp.basic_table.table[41] << 8) | (sfdp.basic_table.table[40] << 0);
        pdata.info.write_granularity = 1 << ((v >> 4) & 0xf);
    }
    pdata.info.opcode_write = NOR_OPCODE_PROG;
}


