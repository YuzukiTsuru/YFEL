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
        chip_t chip_data;
        chip_data.chip_name = "A10";
        chip_data.chip_id = 0x00162300;
        chip_data.chip_core = core_name_.CortexA8;
        chip_data.core_count = core_count_.core_count_1;
        chip_data.chip_type = chip_type_e::Normal;
        return chip_data;
    }

    chip_t a10s() {
        chip_t chip_data;
        chip_data.chip_name = "A10s";
        chip_data.chip_id = 0x00162500;
        chip_data.chip_core = core_name_.CortexA8;
        chip_data.core_count = core_count_.core_count_1;
        chip_data.chip_type = chip_type_e::Normal;
        return chip_data;
    }

    chip_t a13() {
        chip_t chip_data;
        chip_data.chip_name = "A13";
        chip_data.chip_id = 0x00162500;
        chip_data.chip_core = core_name_.CortexA8;
        chip_data.core_count = core_count_.core_count_1;
        chip_data.chip_type = chip_type_e::Normal;
        return chip_data;
    }

    chip_t r8() {
        chip_t chip_data;
        chip_data.chip_name = "R8";
        chip_data.chip_id = 0x00162500;
        chip_data.chip_core = core_name_.CortexA8;
        chip_data.core_count = core_count_.core_count_1;
        chip_data.chip_type = chip_type_e::Normal;
        return chip_data;
    }

    chip_t a20() {
        chip_t chip_data;
        chip_data.chip_name = "A20-H";
        chip_data.chip_id = 0x00165100;
        chip_data.chip_core = core_name_.CortexA7;
        chip_data.core_count = core_count_.core_count_2;
        chip_data.chip_type = chip_type_e::Normal;
        return chip_data;
    }

    chip_t a23() {
        chip_t chip_data;
        chip_data.chip_name = "A23";
        chip_data.chip_id = 0x00165000;
        chip_data.chip_core = core_name_.CortexA7;
        chip_data.core_count = core_count_.core_count_2;
        chip_data.chip_type = chip_type_e::Normal;
        return chip_data;
    }

    chip_t a31() {
        chip_t chip_data;
        chip_data.chip_name = "A31";
        chip_data.chip_id = 0x00163300;
        chip_data.chip_core = core_name_.CortexA7;
        chip_data.core_count = core_count_.core_count_2;
        return chip_data;
    }

    chip_t a33() {
        chip_t chip_data;
        chip_data.chip_name = "A33";
        chip_data.chip_id = 0x00166700;
        chip_data.chip_core = core_name_.CortexA7;
        chip_data.core_count = core_count_.core_count_4;
        chip_data.chip_type = chip_type_e::Normal;
        return chip_data;
    }

    chip_t r16() {
        chip_t chip_data;
        chip_data.chip_name = "r16";
        chip_data.chip_id = 0x00166700;
        chip_data.chip_core = core_name_.CortexA7;
        chip_data.core_count = core_count_.core_count_4;
        chip_data.chip_type = chip_type_e::Normal;
        return chip_data;
    }

    chip_t a40i() {
        chip_t chip_data;
        chip_data.chip_name = "A40i";
        chip_data.chip_id = 0x00170100;
        chip_data.chip_core = core_name_.CortexA7;
        chip_data.core_count = core_count_.core_count_4;
        chip_data.chip_type = chip_type_e::Normal;
        return chip_data;
    }

    chip_t r40() {
        chip_t chip_data;
        chip_data.chip_name = "R40";
        chip_data.chip_id = 0x00170100;
        chip_data.chip_core = core_name_.CortexA7;
        chip_data.core_count = core_count_.core_count_4;
        chip_data.chip_type = chip_type_e::Normal;
        return chip_data;
    }
private:
    chip_core_name_t core_name_;
    chip_core_count_t core_count_;
    chip_type_e chip_type_;
};


#endif // CHIPID_H
