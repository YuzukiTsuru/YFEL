#ifndef CHIPWORKER_H
#define CHIPWORKER_H

#include "ChipList.h"
#include <QString>

class ChipWorker
{
public:
    ChipWorker() = default;

    ~ChipWorker();

    void setChipID(uint64_t chip_id);

private:
    chip_t chip = {};
};

#endif // CHIPWORKER_H
