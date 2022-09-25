/*
 * Copyright (c) 2022, YuzukiTsuru <GloomyGhost@GloomyGhost.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * See README and LICENSE for more details.
 */

#ifndef CHIPS_H
#define CHIPS_H

#include <QString>

#include "dram_header.h"
#include "chips/chip_version.h"
#include "fel.h"

enum chip_type_e {
    Normal,
    Heterogeneous,
};

enum chip_function_e {
    Success,
    Fail,
    NotSupport,
};

enum chip_ddr_type_e {
    DDR2 = 2,
    DDR3 = 3,
    DDR4 = 4,
    LPDDR2 = 6,
    LPDDR3 = 7,
    LPDDR4,
    LPDDR4X,
};

typedef const struct chip_core_name {
    QString ARM9 = "ARMv5 ARM926-EJS";
    QString CortexA7 = "ARM Cortex-A7 32Bit";
    QString CortexA8 = "ARM Cortex-A8 32Bit";
    QString CortexA15 = "ARM Cortex-A15 32Bit";
    QString CortexA15_A7 = "ARM Cortex-A15/A7 big.LITTLE";
    QString CortexA53 = "ARM Cortex-A53 64Bit";
    QString CortexA55 = "ARM Cortex-A55 64Bit";
    QString XuantieC906 = "XuanTie C906 RISC-V 64Bit";
} chip_core_name_t;

typedef const struct chip_core_count {
    QString core_count_1 = "1x";
    QString core_count_2 = "Dual-Core";
    QString core_count_3 = "3x";
    QString core_count_4 = "Quad-Core";
    QString core_count_5 = "5x";
    QString core_count_6 = "Hexa-Core";
    QString core_count_8 = "Octa-Core";
} chip_core_count_t;

typedef struct chip {
    QString chip_name;
    QString chip_sid;
    uint32_t chip_id;

    chip_type_e chip_type;

    QString chip_core;
    uint64_t chip_core_count;
    QString chip_core_count_str;
    QVector<QString> chip_heterogeneous_core;

    chip_version_t chip_version;
} chip_t;

class Chips {
public:
    Chips(fel *f, chip_version_t chip_version) : fel_(f) {
        chip_info.chip_version = chip_version;
    };

    virtual chip_function_e chip_detect() { return chip_function_e::NotSupport; };

    virtual chip_function_e chip_reset() { return chip_function_e::NotSupport; };

    virtual chip_function_e chip_sid() { return chip_function_e::NotSupport; };

    virtual chip_function_e chip_jtag() { return chip_function_e::NotSupport; };

    virtual chip_function_e chip_ddr(chip_ddr_type_e dram_type) { return chip_function_e::NotSupport; };

    virtual chip_function_e chip_ddr(dram_param_t param) { return chip_function_e::NotSupport; };

    virtual chip_function_e chip_spi_init(uint32_t *swap_buf, uint32_t *swap_len, uint32_t *cmd_len) {
        return chip_function_e::NotSupport;
    };

    virtual chip_function_e chip_spi_run(uint8_t *cbuf, uint32_t clen) { return chip_function_e::NotSupport; };

    chip_t get_chip_info() {
        return chip_info;
    };

protected:
    chip_t chip_info{};
    chip_core_name_t core_name_{};
    chip_core_count_t core_count_{};
    chip_type_e chip_type_{};
    fel *fel_ = nullptr;
};

#endif // CHIPS_H
