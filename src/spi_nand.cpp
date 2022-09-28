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

#include "spi_nand.h"

spi_nand::spi_nand(Chips *chips, fel *fels) : spi(chips, fels) {
    spi_init(&pdata.swap_buf, &pdata.swap_len, &pdata.cmd_len);

}

void spi_nand::get_spi_nand_info() {
    const spinand_info_t *info;
    uint8_t tx[2], rx[4];

    tx[0] = SPI_OPCODE::OPCODE_RDID;
    tx[1] = 0x0;


}
