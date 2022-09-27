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

#include <QObject>
#include <QDebug>

#include "spi_nand_info.h"
#include "spi.h"

class spi_nand : public spi {
public:
    spi_nand(Chips *chips, fel *fels) : spi(chips, fels) {

    }

private:

};


#endif //YFEL_SPI_NAND_H
