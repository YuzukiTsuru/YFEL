//
// Created by gloom on 2023/2/4.
//

#ifndef YFEL_UTILS_H
#define YFEL_UTILS_H

#include <QObject>

static QString fixedUint32ToString(uint32_t value) {
    return "0x" + QString::number(value, 16).toUpper().rightJustified(8, '0');
}

#endif //YFEL_UTILS_H
