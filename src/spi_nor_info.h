/*
 * Copyright (c) 2022, YuzukiTsuru <GloomyGhost@GloomyGhost.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 *  See README and LICENSE for more details.
 */

//
// Created by YuzukiTsuru on 2022/10/15.
//

#ifndef YFEL_SPI_NOR_INFO_H
#define YFEL_SPI_NOR_INFO_H

#include <QObject>

#include "spi_nor_sfdp.h"

typedef struct spi_nor_info {
    QString name;
    uint32_t id;
    uint32_t capacity;
    uint32_t blksz;
    uint32_t read_granularity;
    uint32_t write_granularity;
    uint8_t address_length;
    uint8_t opcode_read;
    uint8_t opcode_write;
    uint8_t opcode_write_enable;
    uint8_t opcode_erase_4k;
    uint8_t opcode_erase_32k;
    uint8_t opcode_erase_64k;
    uint8_t opcode_erase_256k;
} spi_nor_info_t;

typedef struct spi_nor_pdata {
    spi_nor_info_t info;
    uint32_t swap_buf{};
    uint32_t swap_len{};
    uint32_t cmd_len{};
} spi_nor_pdata_t;

enum SPI_NOR_OPS {
    NOR_OPCODE_SFDP = 0x5a,
    NOR_OPCODE_RDID = 0x9f,
    NOR_OPCODE_WRSR = 0x01,
    NOR_OPCODE_RDSR = 0x05,
    NOR_OPCODE_WREN = 0x06,
    NOR_OPCODE_READ = 0x03,
    NOR_OPCODE_PROG = 0x02,
    NOR_OPCODE_E4K = 0x20,
    NOR_OPCODE_E32K = 0x52,
    NOR_OPCODE_E64K = 0xd8,
    NOR_OPCODE_ENTER_4B = 0xb7,
    NOR_OPCODE_EXIT_4B = 0xe9,
};

const spi_nor_info_t spi_nor_info_table[] = {
        {
                .name = "W25X40",
                .id = 0xef3013,
                .capacity =512 * 1024,
                .blksz = 4096,
                .read_granularity = 1,
                .write_granularity = 256,
                .address_length = 3,
                .opcode_read = SPI_NOR_OPS::NOR_OPCODE_READ,
                .opcode_write = SPI_NOR_OPS::NOR_OPCODE_PROG,
                .opcode_write_enable = SPI_NOR_OPS::NOR_OPCODE_WREN,
                .opcode_erase_4k = SPI_NOR_OPS::NOR_OPCODE_E4K,
                .opcode_erase_32k = 0,
                .opcode_erase_64k = SPI_NOR_OPS::NOR_OPCODE_E64K,
                .opcode_erase_256k = 0
        },
};

#endif //YFEL_SPI_NOR_INFO_H
