#ifndef CHIPWORKER_H
#define CHIPWORKER_H

#include "ChipList.h"
#include <QString>

class ChipWorker
{
public:
    ChipWorker(QString chipname);

private:
    chip_t chip = {};
};

#endif // CHIPWORKER_H
