#ifndef CHIPDB_H
#define CHIPDB_H

#include <QString>
#include <QVector>
#include "chips/chip_type.h"
#include "libusb-1.0/libusb.h"

class ChipDB {
public:
    explicit ChipDB(chip_version_t chip_version);

    chip_t get_currect_chip();

private:
    void generate_chip_db();

    void check_chip();

private:
    chip_version_t chip_version;
    QVector<Chips*> chip_db;

    chip_t selected_chip;
};

#endif // CHIPDB_H
