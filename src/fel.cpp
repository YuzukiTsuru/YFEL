#include <QDebug>

#include "fel.h"
#include "x.h"

fel::fel() = default;

void fel::fel_scan_chip() {
    fel_open_usb();
    usb_handler.usb_fel_init();
    fel_chip_id();
    fel_close_usb();
}

void fel::fel_force_close() {
    fel_close_usb();
}

void fel::fel_open_usb(){
    usb_handler.usb_init();
    usb_handler.open_usb();
}

void fel::fel_close_usb() {
    usb_handler.close_usb();
    usb_handler.usb_exit();
}

void fel::fel_chip_id() {
    send_fel_request(0x001, 0, 0);
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
    qDebug("read_fel_status 0x%x", buf);
}

chip_version_t fel::fel_get_chip_version() const {
    return version;
}
