#ifndef CHIPS_H
#define CHIPS_H

#include <QString>

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
    DDR1,
    DDR2,
    DDR3,
    DDR4,
};

typedef const struct __chip_core_name {
    QString ARM9         = "ARMv5 ARM926-EJS";
    QString CortexA7     = "ARM Cortex-A7 32Bit";
    QString CortexA8     = "ARM Cortex-A8 32Bit";
    QString CortexA15    = "ARM Cortex-A15 32Bit";
    QString CortexA15_A7 = "ARM Cortex-A15/A7 big.LITTLE";
    QString CortexA53    = "ARM Cortex-A53 64Bit";
    QString CortexA55    = "ARM Cortex-A55 64Bit";
    QString XuantieC906  = "XuanTie C906 RISC-V 64Bit";
} chip_core_name_t;

typedef const struct __chip_core_count {
    QString core_count_1 = "1x";
    QString core_count_2 = "Dual-Core";
    QString core_count_3 = "3x";
    QString core_count_4 = "Quad-Core";
    QString core_count_5 = "5x";
    QString core_count_6 = "Hexa-Core";
    QString core_count_8 = "Octa-Core";
} chip_core_count_t;

typedef struct __chip {
    QString chip_name;
    QString chip_sid;
    QString chip_version;

    chip_type_e chip_type;

    uint32_t chip_id;
    uint32_t chip_firmware;
    uint16_t chip_protocol;
    uint8_t chip_dflag;
    uint8_t chip_dlength;
    uint32_t chip_scratch_pad;
    uint8_t chip_pad[8];

    uint64_t chip_core_count;
    QString core_count;
    QString chip_core;
} chip_t;

class Chips {
public:
    Chips() = default;

    virtual chip_function_e chip_detect();

    virtual chip_function_e chip_reset();

    virtual chip_function_e chip_sid(uint8_t sid);

    virtual chip_function_e chip_jtag();

    virtual chip_function_e chip_ddr(chip_ddr_type_e dram_type);

    virtual chip_function_e chip_spi_init(uint32_t *swap_buf, uint32_t *swap_len, uint32_t *cmd_len);

    virtual chip_function_e chip_spi_run(uint8_t *cbuf, uint32_t clen);

    chip_t get_chip_info();

protected:
    chip_t chip_info{};
    chip_core_name_t core_name_{};
    chip_core_count_t core_count_{};
    chip_type_e chip_type_{};
};


#endif // CHIPID_H
