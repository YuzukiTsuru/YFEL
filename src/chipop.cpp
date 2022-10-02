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

void ChipOP::chip_scan_chip() {
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

void ChipOP::chip_reset_chip() {
    if (fel_status == chip_fel_e::fel_chip_ok) {
        try {
            if (current_chip->chip_reset() == chip_function_e::NotSupport)
                throw std::runtime_error("Function not implemented");
        } catch (const std::runtime_error &error) { // Catch error handle as success
            qDebug() << "Reset Done";
            fel_status = chip_fel_e::fel_chip_none;
        }
    } else {
        throw std::runtime_error("reScan First");
    }
}

void ChipOP::chip_enable_jtag() {
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

QFuture<QString> ChipOP::chip_scan_spi_nand() {
    if (fel_status == chip_fel_e::fel_chip_ok) {
        QFuture<QString> future = QtConcurrent::run([=]() -> QString {
            spi_nand spinand(current_chip, fel_);
            spinand.init();
            return spinand.get_spi_nand_name() + " "
                   + QString::number(spinand.get_spi_nand_size() / 1024 / 1024) + "MB 0x" +
                   QString::number(spinand.get_spi_nand_size(), 16);
        });
        return future;
    } else {
        throw std::runtime_error("reScan First");
    }
}

void ChipOP::chip_erase_spi_nand(uint32_t addr, uint32_t len) {
    if (fel_status == chip_fel_e::fel_chip_ok) {
        spi_nand spinand(current_chip, fel_);
        spinand.init();
        spinand.erase(addr, len);
    } else {
        throw std::runtime_error("reScan First");
    }
}

void ChipOP::chip_exec(uint32_t addr) {
    if (fel_status == chip_fel_e::fel_chip_ok) {
        fel_->fel_exec(addr);
    } else {
        throw std::runtime_error("reScan First");
    }
}

fel *ChipOP::get_current_fel() {
    return fel_;
}



