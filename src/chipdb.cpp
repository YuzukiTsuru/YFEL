#include <QDebug>

#include "chips/chip_db.h"
#include "chipdb.h"

ChipDB::ChipDB(chip_version_t chip_version) : chip_version(chip_version) {
    // Generate chip db
    generate_chip_db();

    // check chip type form chip id
    if (!check_chip()) {
        throw std::runtime_error("Unsupported Chip\nfunction not implemented");
    }
}

void ChipDB::generate_chip_db() {
    chip_db.push_back(new d1(chip_version));
    chip_db.push_back(new r528(chip_version));
}

bool ChipDB::check_chip() {
    for (auto item: qAsConst(chip_db)) {
        qDebug() << "Checking chip " << item->get_chip_info().chip_id;
        if (item->chip_detect() == chip_function_e::Success) {
            current_chip = item;
            // Read SID Here
            current_chip->chip_sid();
            qDebug() << "Current Chip" << current_chip->get_chip_info().chip_name;
            return true;
        }
    }
    return false;
}

chip_t ChipDB::get_current_chip() {
    return current_chip->get_chip_info();
}