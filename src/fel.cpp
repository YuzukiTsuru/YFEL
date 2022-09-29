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

fel::~fel(){
    usb_handler.usb_exit();
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

void fel::fel_chip_id() {
    send_fel_request(FEL_COMMAND::FEL_VERSION, 0, 0);
    usb_handler.usb_read(&version, sizeof(version));
    read_fel_status();
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
    usb_handler.usb_write(&req, sizeof(struct fel_request_t));
}

void fel::read_fel_status() {
    uint8_t buf[8];
    usb_handler.usb_read(buf, sizeof(buf));
    qDebug("read_fel_status 0x%hhn", buf);
}

void fel::fel_read_raw(uint32_t addr, void *buf, size_t len) {
    send_fel_request(FEL_COMMAND::FEL_READRAW, addr, len);
    usb_handler.usb_read(buf, len);
    read_fel_status();
}

void fel::fel_write_raw(uint32_t addr, void *buf, size_t len) {
    send_fel_request(FEL_COMMAND::FEL_WRITERAW, addr, len);
    usb_handler.usb_write(buf, len);
    read_fel_status();
}

uint32_t fel::fel_read32(uint32_t addr) {
    uint32_t val = 0;
    fel_open_usb();
    fel_read_raw(addr, &val, sizeof(uint32_t));
    fel_close_usb();
    return val;
}

void fel::fel_write32(uint32_t addr, uint32_t val) {
    fel_open_usb();
    fel_write_raw(addr, &val, sizeof(uint32_t));
    fel_close_usb();
}

void fel::fel_exec(uint32_t addr) {
    fel_open_usb();
    send_fel_request(FEL_COMMAND::FEL_EXEC, addr, 0);
    read_fel_status();
    fel_close_usb();
}

uint32_t fel::payload_arm_read32(uint32_t addr) {
    // Scan chip info, get scratchpad
    fel_scan_chip();

    // load asm code
    uint32_t payload[] = {
            cpu_to_le32(0xe59f000c), /* ldr r0, [pc, #12] */
            cpu_to_le32(0xe28f100c), /* add r1, pc, #12   */
            cpu_to_le32(0xe4902000), /* ldr r2, [r0], #0  */
            cpu_to_le32(0xe4812000), /* str r2, [r1], #0  */
            cpu_to_le32(0xe12fff1e), /* bx lr             */
            cpu_to_le32(addr),
    };

    uint32_t val;
    fel_write(version.scratchpad, payload, sizeof(payload));
    fel_exec(version.scratchpad);
    fel_read(version.scratchpad + sizeof(payload), &val, sizeof(val));
    return le32_to_cpu(val);
}

