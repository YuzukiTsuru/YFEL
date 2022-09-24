//
// Created by gloom on 2022/9/23.
//

#include "d1.h"

d1::d1(chip_version_t chip_version) : Chips(chip_version) {
    chip_info.chip_name = "D1-H/D1s/F133-A/F133-B/R528-RV";
    chip_info.chip_id = 0x00185900;
    chip_info.chip_type = chip_type_e::Heterogeneous;
    chip_info.chip_core = core_name_.XuantieC906;
    chip_info.chip_core_count = 1;
    chip_info.chip_core_count_str = core_count_.core_count_1;
    chip_info.chip_heterogeneous_core.push_back("HIFI4");
}

d1::~d1() {
    delete fel_;
}

chip_function_e d1::chip_detect() {
    if (chip_info.chip_version.id == chip_info.chip_id) {
        return chip_function_e::Success;
    }
    return chip_function_e::Fail;
}

chip_function_e d1::chip_reset() {
    return chip_function_e::NotSupport;
}

chip_function_e d1::chip_sid(uint8_t sid) {
    return chip_function_e::NotSupport;
}

chip_function_e d1::chip_jtag() {
    return chip_function_e::NotSupport;
}

chip_function_e d1::chip_ddr(chip_ddr_type_e dram_type) {
    return chip_function_e::NotSupport;
}

chip_function_e d1::chip_spi_init(uint32_t *swap_buf, uint32_t *swap_len, uint32_t *cmd_len) {
    return chip_function_e::NotSupport;
}

chip_function_e d1::chip_spi_run(uint8_t *cbuf, uint32_t clen) {
    return chip_function_e::NotSupport;
}
