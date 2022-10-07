/*
 * Copyright (c) 2022, YuzukiTsuru <GloomyGhost@GloomyGhost.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * See README and LICENSE for more details.
 */

//
// Created by gloom on 2022/9/27.
//

#ifndef YFEL_SPI_NAND_H
#define YFEL_SPI_NAND_H

#include <QProgressDialog>
#include <QFutureWatcher>
#include <QtConcurrent>
#include <QObject>
#include <QDebug>

#include "spi_nand_info.h"
#include "spi.h"

class spi_nand : public QObject {
Q_OBJECT
signals:
    void update_progress(int value);

    void release_ui();

public:
    spi_nand(Chips *chips, fel *fels);

    ~spi_nand() override;

    void init();

    void read(uint64_t addr, uint8_t *buf, uint64_t len);

    void write(uint64_t addr, uint8_t *buf, uint64_t len);

    void erase(uint64_t addr, uint64_t len);

    [[nodiscard]] QString get_spi_nand_name() const;

    [[nodiscard]] uint64_t get_spi_nand_size() const;

private:
    bool get_spi_nand_info();

    void spi_nand_reset();

    void spi_nand_get_feature(uint8_t addr, uint8_t *val);

    void spi_nand_set_feature(uint8_t addr, uint8_t val);

    void spi_nand_wait_for_busy();

    void spi_nand_init();

    void spi_nand_read(uint32_t addr, uint8_t *buf, uint32_t count);

    void spi_nand_write(uint32_t addr, uint8_t *buf, uint32_t count);

    void spi_nand_erase(uint64_t addr, uint64_t count);

private:
    spinand_pdata_t pdata = {};
    spi *spi_ = nullptr;

    QProgressDialog dialog;
    QEventLoop loop;
    QFutureWatcher<void> *watcher = nullptr;
};

#endif //YFEL_SPI_NAND_H
