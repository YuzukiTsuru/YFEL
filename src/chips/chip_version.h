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
// Created by gloom on 2022/9/23.
//

#ifndef YFEL_CHIP_VERSION_H
#define YFEL_CHIP_VERSION_H

#include <QObject>

typedef struct chip_version {
    char magic[8];
    uint32_t id;
    uint32_t firmware;
    uint16_t protocol;
    uint8_t dflag;
    uint8_t dlength;
    uint32_t scratchpad;
    uint8_t pad[8];
} chip_version_t;

#endif //YFEL_CHIP_VERSION_H
