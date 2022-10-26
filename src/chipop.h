/*
 * Copyright (c) 2022, YuzukiTsuru <GloomyGhost@GloomyGhost.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * See README and LICENSE for more details.
 */

#ifndef ChipOP_H
#define ChipOP_H

#include <QString>
#include <QObject>
#include <QThread>
#include <QList>
#include <QMultiMap>
#include <QVector>
#include <QtConcurrent>

#include "chips/chip_type.h"

#include "spi_nand.h"
#include "spi_nor.h"
#include "fel.h"

class ChipOP : public QObject {
Q_OBJECT

signals:

    void release_ui();

private slots:

    void chip_release_ui();

public:
    ChipOP();

    ~ChipOP() override;

    /*
     * Scan Chip, will add fel_status -> fel_chip_ok
     */
    void chip_scan_chip();

    void chip_reset_chip();

    void chip_enable_jtag();

    void chip_exec(uint32_t addr);

    void chip_init_dram(const dram_info_t &param);

    void chip_init_dram(const dram_param_t &param);

    void chip_sid();

    /*
     * SPI NAND OP
     */
    QString chip_scan_spi_nand();

    void chip_erase_spi_nand(uint32_t addr, uint32_t len);

    void chip_erase_all_spi_nand();

    QByteArray chip_read_spi_nand(uint64_t addr, uint64_t len);

    void chip_write_spi_nand(uint64_t addr,const QByteArray &buf, uint64_t len);

    /*
     * SPI NOR OP
     */
    QString chip_scan_spi_nor();

public: // getter
    chip_t get_current_chip();

    QVector<dram_info_t> get_dram_params();

private:
    void generate_chip_db();

    bool check_chip();

private:
    fel *fel_ = new fel();
    Chips *current_chip = nullptr;

    chip_version_t chip_version{};
    QVector<Chips *> chip_db;
};

#endif // ChipOP_H
