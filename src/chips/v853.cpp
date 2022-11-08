/*
 * Copyright (c) 2022, YuzukiTsuru <GloomyGhost@GloomyGhost.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * See README and LICENSE for more details.
 */

#include "v853.h"

v853::v853(class fel *f, chip_version_t chip_version) : Chips(f, chip_version) {
    chip_info.chip_name = "V853/V853s/V851s/V851se/R853/R853s";
    chip_info.chip_id = 0x00188600;
    chip_info.chip_type = chip_type_e::Heterogeneous;
    chip_info.chip_core = core_name_.CortexA7;
    chip_info.chip_core_count = 1;
    chip_info.chip_core_count_str = core_count_.core_count_1;
    chip_info.chip_heterogeneous_core.push_back("E907");
    chip_info.chip_heterogeneous_core.push_back("NPU");

    // dram presets
    dram_info.append(v853_ddr3);
}

chip_function_e v853::check_chip_id() {
    chip_id_map_[0x7400] = QString("R853");
    chip_id_map_[0x7800] = QString("V853");
    chip_id_map_[0x7c00] = QString("V853s");
    chip_id_map_[0x5e00] = QString("V851s");

    auto chip_id = fel_->payload_arm_read32(0x3006200) & 0xffff;
    qDebug() << "Get Chip ID: 0x" << QString::number(chip_id, 16);

    for (const auto &i: chip_id_map_) {
        if (i.first == chip_id) {
            chip_info.chip_name = i.second;
        }
    }
    return chip_function_e::Success;
}

chip_function_e v853::chip_detect() {
    if (chip_info.chip_version.id == chip_info.chip_id)
        return chip_function_e::Success;
    return chip_function_e::Fail;
}

chip_function_e v853::chip_reset() {
    return chip_function_e::NotSupport;
}

chip_function_e v853::chip_sid() {
    uint32_t id[4];
    id[0] = fel_->payload_arm_read32(0x03006200 + 0x0);
    id[1] = fel_->payload_arm_read32(0x03006200 + 0x4);
    id[2] = fel_->payload_arm_read32(0x03006200 + 0x8);
    id[3] = fel_->payload_arm_read32(0x03006200 + 0xc);

    chip_info.chip_sid = "";
    for (const uint32_t &j: id) {
        chip_info.chip_sid.append(QString::number(j, 16));
    }
    return chip_function_e::Success;
}

chip_function_e v853::chip_jtag() {
    return chip_function_e::NotSupport;
}

chip_function_e v853::chip_ddr(chip_ddr_type_e dram_type) {
    // default using R528 ddr init code
    if (dram_type == chip_ddr_type_e::DDR3) {
        fel_->fel_write(0x00028000, &ddr3_payload[0], sizeof(ddr3_payload));
        fel_->fel_write(0x00028038, &v853_ddr3, sizeof(v853_ddr3));
        fel_->fel_exec(0x00028000);
        return chip_function_e::Success;
    }
    return chip_function_e::NotSupport;
}

chip_function_e v853::chip_ddr(dram_param_t param) {
    if (param.dram_type == chip_ddr_type_e::DDR3) {
        fel_->fel_write(0x00028000, &ddr3_payload[0], sizeof(ddr3_payload));
    } else {
        return chip_function_e::NotSupport;
    }
    fel_->fel_write(0x00028038, &param, sizeof(param));
    fel_->fel_exec(0x00028000);
    return chip_function_e::Success;
}

chip_function_e v853::chip_spi_init(uint32_t *swap_buf, uint32_t *swap_len, uint32_t *cmd_len) {
    return chip_function_e::NotSupport;
}

chip_function_e v853::chip_spi_run(uint8_t *cbuf, uint32_t clen) {
    return chip_function_e::NotSupport;
}
