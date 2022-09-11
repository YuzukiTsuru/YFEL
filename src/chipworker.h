#ifndef CHIPWORKER_H
#define CHIPWORKER_H

#include "ChipID.h"
#include <QString>

class ChipWorker
{
public:
    ChipWorker(QString chipname);

private:
    chip_t chip = {};
};

#endif // CHIPWORKER_H
