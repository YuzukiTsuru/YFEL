#ifndef CHIPID_H
#define CHIPID_H

#include <QString>

enum chip_type_e {
    Normal,
    Heterogeneous,
};

typedef const struct __chip_core_name {
    QString ARM9         = "ARMv5 ARM926-EJS";
    QString CortexA7     = "ARM Cortex-A7 32Bit";
    QString CortexA8     = "ARM Cortex-A8 32Bit";
    QString CortexA15    = "ARM Cortex-A15 32Bit";
    QString CortexA15_A7 = "ARM Cortex-A15/A7 big.LITTLE";
    QString CortexA53    = "ARM Cortex-A53 64Bit";
    QString CortexA55    = "ARM Cortex-A55 64Bit";
} chip_core_name_t;

typedef const struct __chip_core_count {
    QString core_count_1 = "";
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

    uint64_t chip_id;
    uint64_t chip_dflag;
    uint64_t chip_dlength;
    uint64_t chip_scratch_pad;

    uint64_t chip_core_count;
    QString core_count;
    QString chip_core;
} chip_t;

class ChipID {
    chip_t a10() {
        chip_t a10_t;
        a10_t.chip_name = "A10";
        a10_t.chip_core = core_name_.CortexA8;
        a10_t.core_count = core_count_.core_count_1;
        return a10_t;
    }
private:
    chip_core_name_t core_name_;
    chip_core_count_t core_count_;
    chip_type_e chip_type_;
};


#endif // CHIPID_H
