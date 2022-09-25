//
// Created by gloom on 2022/9/23.
//

#include "r528.h"

r528::r528(class fel *f, chip_version_t chip_version) : Chips(f, chip_version) {
    chip_info.chip_name = "R528-S1/R528-S2/R528-S3/R528-S4/T113-I/T113-S3/H133";
    chip_info.chip_id = 0x00185900;
    chip_info.chip_type = chip_type_e::Heterogeneous;
    chip_info.chip_core = core_name_.CortexA7;
    chip_info.chip_core_count = 2;
    chip_info.chip_core_count_str = core_count_.core_count_2;
    chip_info.chip_heterogeneous_core.push_back("HIFI4");
}

chip_function_e r528::chip_detect() {
    if (chip_info.chip_version.id == chip_info.chip_id)
        // Check 0 addr is 0x43014281, ARM Cortex-A7
        if (fel_->fel_read32(0x00000000) == 0xea000019)
            return chip_function_e::Success;
    return chip_function_e::Fail;
}

chip_function_e r528::chip_reset() {
    // Timer::WDOG_IRQ_EN_REG
    fel_->fel_write32(0x020500a0 + 0x08, (0x16aa << 16) | (0x1 << 0));
    return chip_function_e::Success;
}

chip_function_e r528::chip_sid() {
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

chip_function_e r528::chip_jtag() {
    return chip_function_e::NotSupport;
}

chip_function_e r528::chip_ddr(chip_ddr_type_e dram_type) {
    return chip_function_e::NotSupport;
}

chip_function_e r528::chip_spi_init(uint32_t *swap_buf, uint32_t *swap_len, uint32_t *cmd_len) {
    return chip_function_e::NotSupport;
}

chip_function_e r528::chip_spi_run(uint8_t *cbuf, uint32_t clen) {
    return chip_function_e::NotSupport;
}
