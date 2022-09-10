#ifndef CHIPWORKER_H
#define CHIPWORKER_H

#include <QString>

typedef struct Chip {
    QString chip_name;
    QString chip_sid;
    QString chip_version;
    QString chip_core;
    uint64_t chip_id;
    uint64_t chip_dflag;
    uint64_t chip_dlength;
    uint64_t chip_scratch_pad;
} Chip_t;

class ChipWorker
{
public:
    ChipWorker(QString chipname);

private:
    Chip_t chip = {};
};

#endif // CHIPWORKER_H
