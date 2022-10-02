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

#ifndef YFEL_SPI_H
#define YFEL_SPI_H

#include <QObject>
#include <QDebug>

#include "chips/chip_type.h"
#include "fel.h"

enum chip_spi_ctrl_e {
    SPI_CMD_END = 0x00,
    SPI_CMD_INIT = 0x01,
    SPI_CMD_SELECT = 0x02,
    SPI_CMD_DESELECT = 0x03,
    SPI_CMD_FAST = 0x04,
    SPI_CMD_TXBUF = 0x05,
    SPI_CMD_RXBUF = 0x06,
    SPI_CMD_SPINOR_WAIT = 0x07,
    SPI_CMD_SPINAND_WAIT = 0x08,
};

class spi {
public:
    spi(Chips *chips, fel *fels);

    void spi_init(uint32_t *swap_buf, uint32_t *swap_len, uint32_t *cmd_len);

    void spi_xfer(uint32_t swap_buf, uint32_t swap_len, uint32_t cmd_len, uint8_t *tx_buf,
                  uint32_t tx_len, uint8_t *rx_buf, uint32_t rx_len);

    Chips *get_current_chip();

    fel *get_current_fel();

private:
    Chips *current_chip = nullptr;
    fel *fel_ = nullptr;
};


#endif //YFEL_SPI_H
