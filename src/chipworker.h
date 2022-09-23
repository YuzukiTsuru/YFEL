#ifndef CHIPWORKER_H
#define CHIPWORKER_H

#include <QString>
#include <QVector>
#include "chips/chip_type.h"
#include "libusb-1.0/libusb.h"

class ChipWorker {
public:
    explicit ChipWorker(chip_version_t chip_version);

private:
    void generate_chip_db();

    void check_chip();

private:
    chip_version_t chip_version;
    QVector<Chips*> chip_db;
};

#endif // CHIPWORKER_H
