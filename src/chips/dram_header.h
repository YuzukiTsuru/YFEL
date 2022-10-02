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
// Created by YuzukiTsuru on 2022/9/25.
//

#ifndef YFEL_DRAM_HEADER_H
#define YFEL_DRAM_HEADER_H

#include <QString>
#include <QObject>

typedef struct dram_param {
    QString dram_param_name;
    uint32_t dram_clk;
    uint32_t dram_type;
    uint32_t dram_zq;
    uint32_t dram_odt_en;
    uint32_t dram_para1;
    uint32_t dram_para2;
    uint32_t dram_mr0;
    uint32_t dram_mr1;
    uint32_t dram_mr2;
    uint32_t dram_mr3;
    uint32_t dram_tpr0;
    uint32_t dram_tpr1;
    uint32_t dram_tpr2;
    uint32_t dram_tpr3;
    uint32_t dram_tpr4;
    uint32_t dram_tpr5;
    uint32_t dram_tpr6;
    uint32_t dram_tpr7;
    uint32_t dram_tpr8;
    uint32_t dram_tpr9;
    uint32_t dram_tpr10;
    uint32_t dram_tpr11;
    uint32_t dram_tpr12;
    uint32_t dram_tpr13;
    uint32_t dram_tpr14;
    uint32_t reserve[7];
} dram_param_t;

#endif //YFEL_DRAM_HEADER_H
