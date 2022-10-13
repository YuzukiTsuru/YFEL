/*
 * Copyright (c) 2022, YuzukiTsuru <GloomyGhost@GloomyGhost.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * See README and LICENSE for more details.
 */

#include <QDebug>

#include "fel.h"
#include "x.h"

fel::fel() {
    usb_handler.usb_init();
};

fel::~fel() {
    usb_handler.usb_exit();
}

void fel::fel_open_connection() {
    fel_open_usb();
    fel_status = FEL_STATUS::FEL_OK;
}

void fel::fel_close_connection() {
    fel_close_usb();
    fel_status = FEL_STATUS::FEL_NONE;
}

void fel::fel_scan_chip() {
    fel_open_usb();
    fel_chip_id();
    fel_close_usb();
}

void fel::fel_open_usb() {
    usb_handler.open_usb();
    usb_handler.usb_fel_init();
}

void fel::fel_close_usb() {
    usb_handler.close_usb();
}

// Add long connect
void fel::fel_chip_id() {
    send_fel_request(FEL_COMMAND::FEL_VERSION, 0, 0);
    usb_handler.usb_read(&version, sizeof(version));
    read_fel_status();

    // byte order convert
    version.id = le32_to_cpu(version.id);
    version.firmware = le32_to_cpu(version.firmware);
    version.protocol = le16_to_cpu(version.protocol);
    version.scratchpad = le32_to_cpu(version.scratchpad);
    version.dlength = le32_to_cpu(version.dlength);
    version.dflag = le32_to_cpu(version.dflag);
    // Debug
    qDebug("chip id: 0x%x", version.id);
    qDebug("chip firmware: 0x%x", version.firmware);
    qDebug("chip protocol: 0x%x", version.protocol);
    qDebug("chip scratchpad: 0x%x", version.scratchpad);
    qDebug("chip dlength: 0x%x", version.dlength);
    qDebug("chip dflag: 0x%x", version.dflag);
}

chip_version_t fel::fel_get_chip_version() const {
    return version;
}

void fel::send_fel_request(int type, uint32_t addr, uint32_t length) {
    struct fel_request_t req = {
            .request = cpu_to_le32(type),
            .address = cpu_to_le32(addr),
            .length = cpu_to_le32(length)
    };
    qDebug("send_fel_request: type: %d, addr: 0x%x, length: 0x%x", type, addr, length);
    usb_handler.usb_write(&req, sizeof(struct fel_request_t));
}

void fel::read_fel_status() {
    uint8_t buf[8];
    usb_handler.usb_read(buf, sizeof(buf));
    qDebug("read_fel_status 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x",
           buf[0], buf[1], buf[2], buf[3], buf[4], buf[5], buf[6], buf[7]);
}

void fel::fel_read_raw(uint32_t addr, void *buf, size_t len) {
    qDebug("fel_read_raw: addr: 0x%x, len: 0x%x", addr, len);

    send_fel_request(FEL_COMMAND::FEL_READRAW, addr, len);
    usb_handler.usb_read(buf, len);
    read_fel_status();
}

void fel::fel_write_raw(uint32_t addr, void *buf, size_t len) {
    qDebug("fel_write_raw: addr: 0x%x, len: 0x%x", addr, len);

    send_fel_request(FEL_COMMAND::FEL_WRITERAW, addr, len);
    usb_handler.usb_write(buf, len);
    read_fel_status();
}

uint32_t fel::fel_read32(uint32_t addr) {
    uint32_t val = 0;
    // Check current fel status, if enabled long connection, skip fel open usb
    if (fel_status != FEL_STATUS::FEL_OK) {
        fel_open_usb();
    }
    fel_read_raw(addr, &val, sizeof(uint32_t));
    // Check current fel status, if enabled long connection, skip fel close usb
    if (fel_status != FEL_STATUS::FEL_OK) {
        fel_close_usb();
    }
    return val;
}

void fel::fel_write32(uint32_t addr, uint32_t val) {
    // Check current fel status, if enabled long connection, skip fel open usb
    if (fel_status != FEL_STATUS::FEL_OK) {
        fel_open_usb();
    }
    fel_write_raw(addr, &val, sizeof(uint32_t));
    // Check current fel status, if enabled long connection, skip fel close usb
    if (fel_status != FEL_STATUS::FEL_OK) {
        fel_close_usb();
    }
}

void fel::fel_exec(uint32_t addr) {
    // Check current fel status, if enabled long connection, skip fel open usb
    if (fel_status != FEL_STATUS::FEL_OK) {
        fel_open_usb();
    }
    send_fel_request(FEL_COMMAND::FEL_EXEC, addr, 0);
    read_fel_status();
    // Check current fel status, if enabled long connection, skip fel close usb
    if (fel_status != FEL_STATUS::FEL_OK) {
        fel_close_usb();
    }
}

uint32_t fel::payload_arm_read32(uint32_t addr) {
    // Scan chip info, get scratchpad
    fel_scan_chip();

    // load precompiled machine code
    uint32_t payload[] = {
            cpu_to_le32(0b11100101100111110000000000001100), /* ldr r0, [pc, #12] */
            cpu_to_le32(0b11100010100011110001000000001100), /* add r1, pc, #12   */
            cpu_to_le32(0b11100100100100000010000000000000), /* ldr r2, [r0], #0  */
            cpu_to_le32(0b11100100100000010010000000000000), /* str r2, [r1], #0  */
            cpu_to_le32(0b11100001001011111111111100011110), /* bx lr             */
            cpu_to_le32(addr),
    };

    uint32_t val;
    fel_write(version.scratchpad, payload, sizeof(payload));
    fel_exec(version.scratchpad);
    fel_read(version.scratchpad + sizeof(payload), &val, sizeof(val));
    return le32_to_cpu(val);
}

