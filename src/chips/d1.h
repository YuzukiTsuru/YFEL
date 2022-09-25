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
// Created by gloom on 2022/9/23.
//

#ifndef YFEL_D1_H
#define YFEL_D1_H

#include "chip_type.h"
#include "fel.h"

class d1 : public Chips {
    chip_function_e chip_detect() override;

    chip_function_e chip_reset() override;

    chip_function_e chip_sid() override;

    chip_function_e chip_jtag() override;

    chip_function_e chip_ddr(chip_ddr_type_e dram_type) override;

    chip_function_e chip_spi_init(uint32_t *swap_buf, uint32_t *swap_len, uint32_t *cmd_len) override;

    chip_function_e chip_spi_run(uint8_t *cbuf, uint32_t clen) override;

public:
    d1(class fel *f, chip_version_t chip_version);
};


#endif //YFEL_D1_H
