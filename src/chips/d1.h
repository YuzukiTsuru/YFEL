//
// Created by gloom on 2022/9/23.
//

#ifndef YFEL_D1_H
#define YFEL_D1_H

#include "chips.h"

class D1_H : public Chips {
public:
    chip_function_e chip_detect() override
    {
        if(chip_info.chip_id == 0x00000000)
            return chip_function_e::Success;
        return chip_function_e::Fail;
    }

    chip_function_e chip_reset() override
    {
        return chip_function_e::NotSupport;
    }

    chip_function_e chip_sid(uint8_t sid) override
    {
        return chip_function_e::NotSupport;
    }

    chip_function_e chip_jtag() override
    {
        return chip_function_e::NotSupport;
    }

    chip_function_e chip_ddr(chip_ddr_type_e dram_type) override
    {
        return chip_function_e::NotSupport;
    }

    chip_function_e chip_spi_init(uint32_t *swap_buf, uint32_t *swap_len, uint32_t *cmd_len) override
    {
        return chip_function_e::NotSupport;
    }

    chip_function_e chip_spi_run(uint8_t *cbuf, uint32_t clen) override
    {
        return chip_function_e::NotSupport;
    }

    chip_t get_chip_info() override
    {
        return chip_info;
    }
};

#endif //YFEL_D1_H
