#include "chip_type.h"

Chips::Chips(chip_version_t chip_version) {
    chip_info.chip_version = chip_version;
}

chip_function_e Chips::chip_detect() {
    if (chip_info.chip_version.id == chip_info.chip_id)
        return chip_function_e::Success;
    return chip_function_e::Fail;
}

chip_function_e Chips::chip_reset() {
    return chip_function_e::NotSupport;
}

chip_function_e Chips::chip_sid() {
    return chip_function_e::NotSupport;
}

chip_function_e Chips::chip_jtag() {
    return chip_function_e::NotSupport;
}

chip_function_e Chips::chip_ddr(chip_ddr_type_e dram_type) {
    return chip_function_e::NotSupport;
}

chip_function_e Chips::chip_spi_init(uint32_t *swap_buf, uint32_t *swap_len, uint32_t *cmd_len) {
    return chip_function_e::NotSupport;
}

chip_function_e Chips::chip_spi_run(uint8_t *cbuf, uint32_t clen) {
    return chip_function_e::NotSupport;
}

chip_t Chips::get_chip_info() {
    return chip_info;
}
