/*
 * Copyright (c) 2022, YuzukiTsuru <GloomyGhost@GloomyGhost.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * See README and LICENSE for more details.
 */

#ifndef YFEL_V853_H
#define YFEL_V853_H

#include "chip_type.h"
#include "fel.h"

class v853 : public Chips {
    chip_function_e chip_detect() override;

    chip_function_e chip_reset() override;

    chip_function_e chip_sid() override;

    chip_function_e chip_jtag() override;

    chip_function_e chip_ddr(chip_ddr_type_e dram_type) override;

    chip_function_e chip_ddr(dram_param_t param) override;

    chip_function_e chip_spi_init(uint32_t *swap_buf, uint32_t *swap_len, uint32_t *cmd_len) override;

    chip_function_e chip_spi_run(uint8_t *cbuf, uint32_t clen) override;

public:
    v853(class fel *f, chip_version_t chip_version);

private:
    const dram_info_t v853_s3_ddr3 = {
            .dram_param_name = "R528-S3 DDR3 SIP 792M",
            .dram_param = {
                    .dram_clk = 792,
                    .dram_type = 3,
                    .dram_zq = 0x7b7bfb,
                    .dram_odt_en = 0x00,
                    .dram_para1 = 0x000010d2,
                    .dram_para2 = 0x0000,
                    .dram_mr0 = 0x1c70,
                    .dram_mr1 = 0x042,
                    .dram_mr2 = 0x18,
                    .dram_mr3 = 0x0,
                    .dram_tpr0 = 0x004A2195,
                    .dram_tpr1 = 0x02423190,
                    .dram_tpr2 = 0x0008B061,
                    .dram_tpr3 = 0xB4787896,
                    .dram_tpr4 = 0x0,
                    .dram_tpr5 = 0x48484848,
                    .dram_tpr6 = 0x00000048,
                    .dram_tpr7 = 0x1620121e,
                    .dram_tpr8 = 0x0,
                    .dram_tpr9 = 0x0,
                    .dram_tpr10 = 0x0,
                    .dram_tpr11 = 0x00340000,
                    .dram_tpr12 = 0x00000046,
                    .dram_tpr13 = 0x34000100,
            }
    };

    const dram_info_t t113_s3_ddr3 = {
            .dram_param_name = "T113-S3 DDR3 SIP 792M",
            .dram_param {
                    .dram_clk = 792,
                    .dram_type = 3,
                    .dram_zq = 0x7b7bfb,
                    .dram_odt_en = 0x00,
                    .dram_para1 = 0x000010d2,
                    .dram_para2 = 0x0000,
                    .dram_mr0 = 0x1c70,
                    .dram_mr1 = 0x042,
                    .dram_mr2 = 0x18,
                    .dram_mr3 = 0x0,
                    .dram_tpr0 = 0x004A2195,
                    .dram_tpr1 = 0x02423190,
                    .dram_tpr2 = 0x0008B061,
                    .dram_tpr3 = 0xB4787896,
                    .dram_tpr4 = 0x0,
                    .dram_tpr5 = 0x48484848,
                    .dram_tpr6 = 0x00000048,
                    .dram_tpr7 = 0x1620121e,
                    .dram_tpr8 = 0x0,
                    .dram_tpr9 = 0x0,
                    .dram_tpr10 = 0x0,
                    .dram_tpr11 = 0x00340000,
                    .dram_tpr12 = 0x00000046,
                    .dram_tpr13 = 0x34000100,
            }
    };

    const dram_info_t t113_s3_ddr3_oc_1008 = {
            .dram_param_name = "T113-S3 DDR3 SIP 1008M OC",
            .dram_param {
                    .dram_clk = 1008,
                    .dram_type = 3,
                    .dram_zq = 0x7b7bfb,
                    .dram_odt_en = 0x00,
                    .dram_para1 = 0x000010d2,
                    .dram_para2 = 0x0000,
                    .dram_mr0 = 0x1c70,
                    .dram_mr1 = 0x042,
                    .dram_mr2 = 0x18,
                    .dram_mr3 = 0x0,
                    .dram_tpr0 = 0x004A2195,
                    .dram_tpr1 = 0x02423191,
                    .dram_tpr2 = 0x0008B061,
                    .dram_tpr3 = 0xB4787896,
                    .dram_tpr4 = 0x0,
                    .dram_tpr5 = 0x48484848,
                    .dram_tpr6 = 0x00000048,
                    .dram_tpr7 = 0x1620121e,
                    .dram_tpr8 = 0x0,
                    .dram_tpr9 = 0x0,
                    .dram_tpr10 = 0x0,
                    .dram_tpr11 = 0x00340000,
                    .dram_tpr12 = 0x00000046,
                    .dram_tpr13 = 0x34000100,
            }
    };
};

#endif //YFEL_V853_H
