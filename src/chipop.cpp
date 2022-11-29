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
#include <utility>

#include "chips/chip_db.h"
#include "chipop.h"
#include "exceptions.h"

ChipOP::ChipOP() {
    // Generate chip db
    qDebug() << "init chip op, generate chip db";
    generate_chip_db();
};

ChipOP::~ChipOP() {
    qDebug() << "ChipOP::~ChipOP()";
    delete fel_;
    delete current_chip;
}

void ChipOP::chip_release_ui() {
    qDebug() << "ChipOP::chip_release_ui()";
    emit release_ui();
}

void ChipOP::generate_chip_db() {
    // clear chip db
    chip_db.clear();

    chip_db.push_back(new d1(fel_, chip_version));
    chip_db.push_back(new r528(fel_, chip_version));
    chip_db.push_back(new f1c100s(fel_, chip_version));
    chip_db.push_back(new v853(fel_, chip_version));
}

void ChipOP::chip_scan_chip() {
    fel_->fel_open_connection();

    fel_->fel_scan_chip();
    chip_version = fel_->fel_get_chip_version();
    fel_->fel_close_connection();

    // Generate chip db
    generate_chip_db();

    // check chip type form chip id
    if (!check_chip()) {
        throw std::runtime_error("Unsupported Chip 0x" +
                                 QString::number(chip_version.id, 16).toStdString() +
                                 "\nfunction not implemented");
    }
}

void ChipOP::chip_reset_chip() {
    try {
        if (current_chip->chip_reset() == chip_function_e::NotSupport)
            throw function_not_implemented();
    } catch (const usb_bulk_recv_error &error) { // Catch usb_bulk_recv_error handle as success
        qDebug() << "Reset Done";
    } catch (const usb_bulk_send_error &error) { // Catch usb_bulk_send_error handle as success
        qDebug() << "Reset Done";
    }
}

void ChipOP::chip_enable_jtag() {
    if (current_chip->chip_jtag() == chip_function_e::NotSupport)
        throw function_not_implemented();
}

chip_t ChipOP::get_current_chip() {
    return current_chip->get_chip_info();
}

bool ChipOP::check_chip() {
    // enable long connection of fel
    fel_->fel_open_connection();
    for (auto item: qAsConst(chip_db)) {
        qDebug() << "Checking chip 0x" << QString::number(item->get_chip_info().chip_id, 16);
        if (item->chip_detect() == chip_function_e::Success) {
            current_chip = item;
            qDebug() << "Current Chip" << current_chip->get_chip_info().chip_name;

            // check chip id
            current_chip->check_chip_id();

            // deinit fel
            fel_->fel_close_connection();
            return true;
        }
    }
    fel_->fel_close_connection();
    return false;
}

void ChipOP::chip_init_dram(const dram_info_t &info) {
    current_chip->chip_ddr(info.dram_param);
}

void ChipOP::chip_init_dram(const dram_param_t &param) {
    current_chip->chip_ddr(param);
}

void ChipOP::chip_exec(uint32_t addr) {
    fel_->fel_exec(addr);
}

QVector<dram_info_t> ChipOP::get_dram_params() {
    return current_chip->get_chip_dram_info();
}

void ChipOP::chip_sid() {
    // Read SID Here
    current_chip->chip_sid();
}

QString ChipOP::chip_scan_spi_nand() {
    fel_->fel_open_connection();
    try {
        spi_nand spinand(current_chip, fel_);
        spinand.init();
        fel_->fel_close_connection();

        chip_release_ui();
        if (spinand.get_spi_nand_size() == 0) {
            throw cannot_find_spi_flash_device();
        } else {
            return spinand.get_spi_nand_name() + " "
                   + QString::number(spinand.get_spi_nand_size() / 1024 / 1024) + "MB 0x"
                   + QString::number(spinand.get_spi_nand_size(), 16);
        }
    } catch (const function_not_implemented &e) {
        fel_->fel_close_connection();
        chip_release_ui();
        throw function_not_implemented();
    } catch (const std::runtime_error &e) {
        fel_->fel_close_connection();
        chip_release_ui();
        throw std::runtime_error(e.what());
    }
}

void ChipOP::chip_erase_spi_nand(uint32_t addr, uint32_t len) {
    fel_->fel_open_connection();
    try {
        spi_nand spinand(current_chip, fel_);
        connect(&spinand, &spi_nand::release_ui, this, &ChipOP::chip_release_ui);
        spinand.init();
        if ((len + addr) > spinand.get_spi_nand_size()) {
            // if len + addr > spi nand size, disconnect signal, emit release ui and throw exception
            disconnect(&spinand, &spi_nand::release_ui, this, &ChipOP::chip_release_ui);
            fel_->fel_close_connection();
            emit release_ui();
            throw spi_erase_out_of_range();
        } else {
            spinand.erase(addr, len);
            disconnect(&spinand, &spi_nand::release_ui, this, &ChipOP::chip_release_ui);
            fel_->fel_close_connection();
        }
    } catch (const function_not_implemented &e) {
        fel_->fel_close_connection();
        chip_release_ui();
        throw function_not_implemented();
    } catch (const std::runtime_error &e) {
        fel_->fel_close_connection();
        chip_release_ui();
        throw std::runtime_error(e.what());
    }
}

void ChipOP::chip_erase_all_spi_nand() {
    fel_->fel_open_connection();
    try {
        spi_nand spinand(current_chip, fel_);
        connect(&spinand, &spi_nand::release_ui, this, &ChipOP::chip_release_ui);
        spinand.init();
        spinand.erase(0x00, spinand.get_spi_nand_size());
        disconnect(&spinand, &spi_nand::release_ui, this, &ChipOP::chip_release_ui);
        fel_->fel_close_connection();
    } catch (const function_not_implemented &e) {
        fel_->fel_close_connection();
        chip_release_ui();
        throw function_not_implemented();
    } catch (const std::runtime_error &e) {
        fel_->fel_close_connection();
        chip_release_ui();
        throw std::runtime_error(e.what());
    }
}

QByteArray ChipOP::chip_read_spi_nand(uint64_t addr, uint64_t len) {
    auto *buf = new uint8_t[len];
    fel_->fel_open_connection();
    spi_nand spinand(current_chip, fel_);
    connect(&spinand, &spi_nand::release_ui, this, &ChipOP::chip_release_ui);
    spinand.init();
    spinand.read(addr, buf, len);
    disconnect(&spinand, &spi_nand::release_ui, this, &ChipOP::chip_release_ui);
    fel_->fel_close_connection();

    return QByteArray::fromRawData(reinterpret_cast<char *>(buf), static_cast<int64_t>(len));
}

void ChipOP::chip_write_spi_nand(const uint64_t addr, const QByteArray &buf, const uint64_t len) {
    auto buffer = reinterpret_cast<const uint8_t *>(buf.data());
    fel_->fel_open_connection();
    spi_nand spinand(current_chip, fel_);
    connect(&spinand, &spi_nand::release_ui, this, &ChipOP::chip_release_ui);
    spinand.init();
    spinand.erase(addr, len);
    spinand.write(addr, buffer, len);
    disconnect(&spinand, &spi_nand::release_ui, this, &ChipOP::chip_release_ui);
    fel_->fel_close_connection();
}

QString ChipOP::chip_scan_spi_nor() {
    fel_->fel_open_connection();
    try {
        spi_nor spi_nor(current_chip, fel_);
        spi_nor.init();
        fel_->fel_close_connection();

        chip_release_ui();
        if (spi_nor.get_spi_nor_size() < 1) {
            throw cannot_find_spi_flash_device();
        } else {
            if (spi_nor.get_spi_nor_name() == "SFDP") {
                return "JEDEC SFDP Compatible SPI NOR " + QString::number(spi_nor.get_spi_nor_size() / 1024 / 1024) +
                       "MB 0x" + QString::number(spi_nor.get_spi_nor_size(), 16);
            } else {
                return spi_nor.get_spi_nor_name() + " "
                       + QString::number(spi_nor.get_spi_nor_size() / 1024 / 1024) + "MB 0x"
                       + QString::number(spi_nor.get_spi_nor_size(), 16);
            }
        }
    } catch (const function_not_implemented &e) {
        fel_->fel_close_connection();
        chip_release_ui();
        throw function_not_implemented();
    } catch (const std::runtime_error &e) {
        fel_->fel_close_connection();
        chip_release_ui();
        throw std::runtime_error(e.what());
    }
}

uint32_t ChipOP::chipReadArm32(uint32_t addr) {
    return fel_->payload_arm_read32(addr);
}

void ChipOP::chip_write(uint64_t addr, const QByteArray &buf, uint64_t len) {
    auto buffer = reinterpret_cast<const uint8_t *>(buf.data());
    fel_->fel_open_connection();
    try {
        fel_->fel_write(addr, buffer, len);
    } catch (const function_not_implemented &e) {
        fel_->fel_close_connection();
        chip_release_ui();
        throw function_not_implemented();
    } catch (const std::runtime_error &e) {
        fel_->fel_close_connection();
        chip_release_ui();
        throw std::runtime_error(e.what());
    }
}

