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
