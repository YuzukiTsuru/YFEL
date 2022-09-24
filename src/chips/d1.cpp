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
        // Check 0 addr is 0x43014281, RISC-V
        if (fel_->fel_read32(0x00000000) == 0x43014281)
            return chip_function_e::Success;
    }
    return chip_function_e::Fail;
}

chip_function_e d1::chip_reset() {
    fel_->fel_write32(0x020500a0 + 0x08, (0x16aa << 16) | (0x1 << 0));
    return chip_function_e::Success;
}

chip_function_e d1::chip_sid() {
    const uint8_t payload[] = {
            0x37, 0x03, 0x40, 0x00, 0x73, 0x20, 0x03, 0x7c, 0x37, 0x03, 0x03, 0x00,
            0x1b, 0x03, 0x33, 0x01, 0x73, 0x20, 0x23, 0x7c, 0x6f, 0x00, 0x40, 0x00,
            0x13, 0x01, 0x01, 0xfe, 0x23, 0x34, 0x81, 0x00, 0x23, 0x38, 0x91, 0x00,
            0x23, 0x3c, 0x11, 0x00, 0x13, 0x04, 0x05, 0x00, 0x37, 0x15, 0x02, 0x00,
            0xef, 0x00, 0x00, 0x02, 0x83, 0x30, 0x81, 0x01, 0x03, 0x34, 0x81, 0x00,
            0x83, 0x34, 0x01, 0x01, 0x13, 0x01, 0x01, 0x02, 0x67, 0x80, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xb7, 0x67, 0x00, 0x03,
            0x03, 0xa7, 0x07, 0x20, 0x23, 0x20, 0xe5, 0x00, 0x03, 0xa7, 0x47, 0x20,
            0x23, 0x22, 0xe5, 0x00, 0x03, 0xa7, 0x87, 0x20, 0x23, 0x24, 0xe5, 0x00,
            0x83, 0xa7, 0xc7, 0x20, 0x23, 0x26, 0xf5, 0x00, 0x67, 0x80, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
    };
    uint32_t id[4];

    fel_->fel_write(0x00020000, &payload[0], sizeof(payload));
    fel_->fel_exec(0x00020000);
    fel_->fel_read(0x00021000, id, sizeof(id));

    for (const uint32_t &j: id) {
        chip_info.chip_sid.append(QString::number(j, 16));
    }
    return chip_function_e::Success;
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
