/*
 * Copyright (c) 2022, YuzukiTsuru <GloomyGhost@GloomyGhost.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 *  See README and LICENSE for more details.
 */

//
// Created by YuzukiTsuru on 2022/9/29.
//

#ifndef YFEL_EXCEPTIONS_H
#define YFEL_EXCEPTIONS_H

#include <QException>
#include <string>

class function_not_implemented : public QException {
public:
    void raise() const override {
        throw *this;
    }

    [[nodiscard]] function_not_implemented *clone() const override {
        return new function_not_implemented(*this);
    }
};

class spi_len_error : public QException {
public:
    void raise() const override {
        throw *this;
    }

    [[nodiscard]] spi_len_error *clone() const override {
        return new spi_len_error(*this);
    }
};

#endif //YFEL_EXCEPTIONS_H
