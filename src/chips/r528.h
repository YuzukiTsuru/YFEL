#ifndef R528_H
#define R528_H


#include "chips.h"

class r528 : public Chips
{
public:
    r528()
    {
        chip_info.chip_name = "R528";
        chip_info.chip_id = 0x00162500;
        chip_info.chip_core = core_name_.CortexA7;
        chip_info.core_count = core_count_.core_count_2;
        chip_info.chip_type = chip_type_e::Heterogeneous;
    };
};

#endif // R528_H
