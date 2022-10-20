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
// Created by gloom on 2022/9/28.
//

#ifndef YFEL_SPI_NOR_H
#define YFEL_SPI_NOR_H

#include <QProgressDialog>
#include <QFutureWatcher>
#include <QtConcurrent>
#include <QObject>
#include <QDebug>

#include "spi_nor_info.h"
#include "spi.h"


class spi_nor : public QObject {
    Q_OBJECT
public:
    spi_nor(Chips *chips, fel *fels);

    ~spi_nor() override;

    void init();

    [[nodiscard]] QString get_spi_nor_name() const;

    [[nodiscard]] uint64_t get_spi_nor_size() const;

private:
    bool get_spi_nor_info();

    void spi_nor_init();

    bool spi_nor_read_sfdp();

    // read spi nor id
    // input id ptr to store id
    // output true if success
    uint32_t spi_nor_read_id();

    void spi_nor_sfdp_handler();

private:
    spi_nor_pdata_t pdata{};
    sfdp_t sfdp{};
    spi *spi_ = nullptr;
};


#endif //YFEL_SPI_NOR_H
