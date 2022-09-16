#ifndef A13_H
#define A13_H

#include "chips.h"

class a13 : public Chips
{
public:
    a13()
    {
        chip_info.chip_name = "A13/A10S/R8";
        chip_info.chip_id = 0x00162500;
        chip_info.chip_core = core_name_.CortexA8;
        chip_info.core_count = core_count_.core_count_1;
        chip_info.chip_type = chip_type_e::Normal;
    };
};

#endif // A13_H
