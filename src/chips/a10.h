#ifndef A10_H
#define A10_H

#include "chips.h"

class a10 : public Chips
{
public:
    a10()
    {
        chip_info.chip_name = "A10";
        chip_info.chip_id = 0x00162300;
        chip_info.chip_core = core_name_.CortexA8;
        chip_info.core_count = core_count_.core_count_1;
        chip_info.chip_type = chip_type_e::Normal;
    }
};

#endif // A10_H
