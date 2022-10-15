/*
 * Copyright (c) 2022, YuzukiTsuru <GloomyGhost@GloomyGhost.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * See README and LICENSE for more details.
 */

#ifndef YFEL_FEL_H
#define YFEL_FEL_H

#include <QString>
#include <QObject>
#include <QThread>
#include <QList>
#include <QMultiMap>

#include <libusb-1.0/libusb.h>

#include "usb.h"
#include "chips/chip_version.h"

class fel : QObject {
Q_OBJECT
protected:
    enum FEL_COMMAND {
        FEL_VERSION = 0x001,
        FEL_WRITERAW = 0x101,
        FEL_EXEC = 0x102,
        FEL_READRAW = 0x103,
    };

    enum FEL_STATUS {
        FEL_NONE = 0x00,
        FEL_OK = 0x01,
        FEL_ERROR = 0x02,
    };

    struct fel_request_t {
        uint32_t request;
        uint32_t address;
        uint32_t length;
        uint32_t pad;
    } __attribute__((packed));

private:
    chip_version_t version{};
    usb usb_handler;
    FEL_STATUS fel_status = FEL_STATUS::FEL_NONE;

public:
    fel();

    ~fel() override;

    void fel_open_connection();

    void fel_close_connection();

    void fel_scan_chip();

    [[nodiscard]] chip_version_t fel_get_chip_version() const;

    uint32_t fel_read32(uint32_t addr);

    void fel_write32(uint32_t addr, uint32_t val);

    void fel_exec(uint32_t addr);

    uint32_t payload_arm_read32(uint32_t addr);

public:
    template<typename T>
    void fel_read(uint32_t addr, T *buf, size_t len) {
        // Check current fel status, if enabled long connection, skip fel open usb
        if (fel_status != FEL_STATUS::FEL_OK) {
            fel_open_usb();
        }

        // do fel read raw
        size_t n;
        while (len > 0) {
            n = len > 65536 ? 65536 : len;
            fel_read_raw(addr, (void *) buf, n);
            addr += n;
            buf += n;
            len -= n;
        }

        // Check current fel status, if enabled long connection, skip fel close usb
        if (fel_status != FEL_STATUS::FEL_OK) {
            fel_close_usb();
        }
    };

    template<typename T>
    void fel_write(uint32_t addr, T *buf, size_t len) {
        // Check current fel status, if enabled long connection, skip fel open usb
        if (fel_status != FEL_STATUS::FEL_OK) {
            fel_open_usb();
        }

        // do fel write raw
        size_t n;
        while (len > 0) {
            n = len > 65536 ? 65536 : len;
            fel_write_raw(addr, (void *) buf, n);
            addr += n;
            buf += n;
            len -= n;
        }

        // Check current fel status, if enabled long connection, skip fel close usb
        if (fel_status != FEL_STATUS::FEL_OK) {
            fel_close_usb();
        }
    };

private:
    void fel_chip_id();

    void fel_open_usb();

    void fel_close_usb();

    void send_fel_request(int type, uint32_t addr, uint32_t length);

    void read_fel_status();

    void fel_read_raw(uint32_t addr, void *buf, uint32_t len);

    void fel_write_raw(uint32_t addr, void *buf, uint32_t len);
};

#endif// YFEL_FEL_H
