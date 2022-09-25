#include <QDebug>

#include "chips/chip_db.h"
#include "chipop.h"

ChipOP::ChipOP() = default;

ChipOP::~ChipOP() {
    delete fel_;
}

void ChipOP::scan_chip() {
    fel_->fel_scan_chip();
    chip_version = fel_->fel_get_chip_version();

    // Generate chip db
    generate_chip_db();

    // check chip type form chip id
    if (!check_chip()) {
        throw std::runtime_error("Unsupported Chip\nfunction not implemented");
    }
}

void ChipOP::reset_chip() {
    try {
        current_chip->chip_reset();
    } catch (const std::runtime_error &error) {
        qDebug() << "Reset Done";
    }
}

chip_t ChipOP::get_current_chip() {
    return current_chip->get_chip_info();
}

void ChipOP::generate_chip_db() {
    chip_db.push_back(new d1(fel_, chip_version));
    chip_db.push_back(new r528(fel_, chip_version));
}

bool ChipOP::check_chip() {
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

