#ifndef CHIPDB_H
#define CHIPDB_H

#include <QString>
#include <QObject>
#include <QThread>
#include <QList>
#include <QMultiMap>
#include <QVector>
#include "chips/chip_type.h"

#include "fel.h"

class ChipDB {
public:
    ChipDB();

    ~ChipDB();

    chip_t get_current_chip();

private:
    void generate_chip_db();

    bool check_chip();

private:
    fel *fel_ = new fel();
    chip_version_t chip_version{};
    QVector<Chips *> chip_db;
    Chips *current_chip = nullptr;
};

#endif // CHIPDB_H
