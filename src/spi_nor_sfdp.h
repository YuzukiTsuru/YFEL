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

#ifndef YFEL_SPI_NOR_SFDP_H
#define YFEL_SPI_NOR_SFDP_H

#include <QObject>

const int SFDP_MAX_NPH(6);

typedef struct sfdp_header {
    uint8_t sign[4];
    uint8_t minor;
    uint8_t major;
    uint8_t nph;
    uint8_t unused;
} sfdp_header_t;

typedef struct sfdp_parameter_header {
    uint8_t idlsb;
    uint8_t minor;
    uint8_t major;
    uint8_t length;
    uint8_t ptp[3];
    uint8_t idmsb;
} sfdp_parameter_header_t;

typedef struct sfdp_basic_table {
    uint8_t minor;
    uint8_t major;
    uint8_t table[16 * 4];
} sfdp_basic_table_t;

typedef struct sfdp {
    sfdp_header_t header{};
    sfdp_parameter_header_t parameter_header[SFDP_MAX_NPH]{};
    sfdp_basic_table_t basic_table{};
} sfdp_t;

#endif //YFEL_SPI_NOR_SFDP_H
