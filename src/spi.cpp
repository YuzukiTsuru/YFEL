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

#include "spi.h"

spi::spi(Chips *chips, fel *fels) : current_chip(chips), fel_(fels) {

}

void spi::spi_init(uint32_t *swap_buf, uint32_t *swap_len, uint32_t *cmd_len) {
    uint8_t cbuf[2] = {0, 0};
    if (current_chip->chip_spi_init(swap_buf, swap_len, cmd_len) == chip_function_e::NotSupport)
        throw std::runtime_error("Function not implemented");
    cbuf[0] = chip_spi_ctrl_e::SPI_CMD_INIT;
    cbuf[1] = chip_spi_ctrl_e::SPI_CMD_END;
    if (current_chip->chip_spi_run(cbuf, sizeof(cbuf)) == chip_function_e::NotSupport)
        throw std::runtime_error("Function not implemented");
}

void spi::spi_xfer(uint32_t swap_buf, uint32_t swap_len, uint32_t cmd_len, uint8_t *tx_buf,
                   uint32_t tx_len, uint8_t *rx_buf, uint32_t rx_len) {
    uint8_t cbuf[256];
    uint32_t clen;
    uint32_t n;

    if ((tx_len <= swap_len) && (rx_len <= swap_len)) {
        clen = 0;
        cbuf[clen++] = chip_spi_ctrl_e::SPI_CMD_SELECT;

        if (tx_len > 0) {
            cbuf[clen++] = chip_spi_ctrl_e::SPI_CMD_TXBUF;
            cbuf[clen++] = (swap_buf >> 0) & 0xff;
            cbuf[clen++] = (swap_buf >> 8) & 0xff;
            cbuf[clen++] = (swap_buf >> 16) & 0xff;
            cbuf[clen++] = (swap_buf >> 24) & 0xff;
            cbuf[clen++] = (tx_len >> 0) & 0xff;
            cbuf[clen++] = (tx_len >> 8) & 0xff;
            cbuf[clen++] = (tx_len >> 16) & 0xff;
            cbuf[clen++] = (tx_len >> 24) & 0xff;
        }
        if (rx_len > 0) {
            cbuf[clen++] = chip_spi_ctrl_e::SPI_CMD_RXBUF;
            cbuf[clen++] = (swap_buf >> 0) & 0xff;
            cbuf[clen++] = (swap_buf >> 8) & 0xff;
            cbuf[clen++] = (swap_buf >> 16) & 0xff;
            cbuf[clen++] = (swap_buf >> 24) & 0xff;
            cbuf[clen++] = (rx_len >> 0) & 0xff;
            cbuf[clen++] = (rx_len >> 8) & 0xff;
            cbuf[clen++] = (rx_len >> 16) & 0xff;
            cbuf[clen++] = (rx_len >> 24) & 0xff;
        }
        cbuf[clen++] = chip_spi_ctrl_e::SPI_CMD_DESELECT;
        cbuf[clen++] = chip_spi_ctrl_e::SPI_CMD_END;
        if (tx_len > 0)
            fel_->fel_write(swap_buf, tx_buf, tx_len);
        if (clen > cmd_len)
            throw std::runtime_error("clen > cmd_len");
        current_chip->chip_spi_run(cbuf, clen);
        if (rx_len > 0)
            fel_->fel_read(swap_buf, rx_buf, rx_len);
    } else {
        clen = 0;
        cbuf[clen++] = chip_spi_ctrl_e::SPI_CMD_SELECT;
        cbuf[clen++] = chip_spi_ctrl_e::SPI_CMD_END;

        current_chip->chip_spi_run(cbuf, clen);

        while (tx_len > 0) {
            n = tx_len > swap_len ? swap_len : tx_len;
            clen = 0;
            cbuf[clen++] = chip_spi_ctrl_e::SPI_CMD_TXBUF;
            cbuf[clen++] = (swap_buf >> 0) & 0xff;
            cbuf[clen++] = (swap_buf >> 8) & 0xff;
            cbuf[clen++] = (swap_buf >> 16) & 0xff;
            cbuf[clen++] = (swap_buf >> 24) & 0xff;
            cbuf[clen++] = (n >> 0) & 0xff;
            cbuf[clen++] = (n >> 8) & 0xff;
            cbuf[clen++] = (n >> 16) & 0xff;
            cbuf[clen++] = (n >> 24) & 0xff;
            cbuf[clen++] = chip_spi_ctrl_e::SPI_CMD_END;

            fel_->fel_write(swap_buf, tx_buf, n);
            current_chip->chip_spi_run(cbuf, clen);

            tx_buf += n;
            tx_len -= n;
        }

        while (rx_len > 0) {
            n = rx_len > swap_len ? swap_len : rx_len;
            clen = 0;
            cbuf[clen++] = chip_spi_ctrl_e::SPI_CMD_RXBUF;
            cbuf[clen++] = (swap_buf >> 0) & 0xff;
            cbuf[clen++] = (swap_buf >> 8) & 0xff;
            cbuf[clen++] = (swap_buf >> 16) & 0xff;
            cbuf[clen++] = (swap_buf >> 24) & 0xff;
            cbuf[clen++] = (n >> 0) & 0xff;
            cbuf[clen++] = (n >> 8) & 0xff;
            cbuf[clen++] = (n >> 16) & 0xff;
            cbuf[clen++] = (n >> 24) & 0xff;
            cbuf[clen++] = chip_spi_ctrl_e::SPI_CMD_END;

            current_chip->chip_spi_run(cbuf, clen);
            fel_->fel_read(swap_buf, rx_buf, n);
            rx_buf += n;
            rx_len -= n;
        }
        clen = 0;
        cbuf[clen++] = chip_spi_ctrl_e::SPI_CMD_DESELECT;
        cbuf[clen++] = chip_spi_ctrl_e::SPI_CMD_END;
        if (clen > cmd_len)
            throw std::runtime_error("clen > cmd_len");
        current_chip->chip_spi_run(cbuf, clen);
    }
}
