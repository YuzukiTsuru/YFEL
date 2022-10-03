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

class usb_bulk_send_error : public QException {
public:
    void raise() const override {
        throw *this;
    }

    [[nodiscard]] usb_bulk_send_error *clone() const override {
        return new usb_bulk_send_error(*this);
    }
};

class usb_bulk_recv_error : public QException {
public:
    void raise() const override {
        throw *this;
    }

    [[nodiscard]] usb_bulk_recv_error *clone() const override {
        return new usb_bulk_recv_error(*this);
    }
};

class read_usb_response_failed : public QException {
public:
    void raise() const override {
        throw *this;
    }

    [[nodiscard]] read_usb_response_failed *clone() const override {
        return new read_usb_response_failed(*this);
    }
};

class cannot_find_fel_device : public QException {
public:
    void raise() const override {
        throw *this;
    }

    [[nodiscard]] cannot_find_fel_device *clone() const override {
        return new cannot_find_fel_device(*this);
    }
};

#endif //YFEL_EXCEPTIONS_H
