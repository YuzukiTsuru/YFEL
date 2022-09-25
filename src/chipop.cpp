/*
 * Copyright (c) 2022, YuzukiTsuru <GloomyGhost@GloomyGhost.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * See README and LICENSE for more details.
 */

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

    fel_status = chip_fel_e::fel_chip_ok;
}

void ChipOP::reset_chip() {
    if (fel_status == chip_fel_e::fel_chip_ok) {
        try {
            if (current_chip->chip_reset() == chip_function_e::NotSupport)
                throw std::runtime_error("Function not implemented");
        } catch (const std::runtime_error &error) {
            qDebug() << "Reset Done";
        }
    } else {
        throw std::runtime_error("reScan First");
    }
}

void ChipOP::enable_jtag() {
    if (fel_status == chip_fel_e::fel_chip_ok) {
        try {
            if (current_chip->chip_jtag() == chip_function_e::NotSupport)
                throw std::runtime_error("Function not implemented");
        } catch (const std::runtime_error &error) {
            qDebug() << "Reset Done";
        }
    } else {
        throw std::runtime_error("reScan First");
    }
}

chip_t ChipOP::get_current_chip() {
    if (fel_status == chip_fel_e::fel_chip_ok) {
        return current_chip->get_chip_info();
    } else {
        throw std::runtime_error("reScan First");
    }
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

