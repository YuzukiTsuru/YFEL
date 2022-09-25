//
// Created by gloom on 2022/9/23.
//

#ifndef YFEL_R528_H
#define YFEL_R528_H

#include "chip_type.h"
#include "fel.h"

class r528 : public Chips {
    chip_function_e chip_detect() override;

    chip_function_e chip_reset() override;

    chip_function_e chip_sid() override;

    chip_function_e chip_jtag() override;

    chip_function_e chip_ddr(chip_ddr_type_e dram_type) override;

    chip_function_e chip_spi_init(uint32_t *swap_buf, uint32_t *swap_len, uint32_t *cmd_len) override;

    chip_function_e chip_spi_run(uint8_t *cbuf, uint32_t clen) override;

public:
    r528(class fel *f, chip_version_t chip_version);
};

#endif //YFEL_R528_H
