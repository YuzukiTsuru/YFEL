#ifndef FEL_H
#define FEL_H

#include <QString>
#include <QObject>
#include <QThread>
#include <QList>
#include <QMultiMap>

#include "libusb-1.0/libusb.h"

#include "usb.h"
#include "chipdb.h"
#include "chips/chip_version.h"

class fel : QObject {
    Q_OBJECT
private:
    enum FEL_COMMAND {
        FEL_VERSION = 0x001,
        FEL_WRITERAW = 0x101,
        FEL_EXEC = 0x102,
        FEL_READRAW = 0x103,
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

public:
    fel();

    void fel_scan_chip();

    [[nodiscard]] chip_version_t fel_get_chip_version() const;

    uint32_t fel_read32(uint32_t addr);

    void fel_write32(uint32_t addr, uint32_t val);

    void fel_exec(uint32_t addr);


public:
    template<typename T>
    void fel_read(uint32_t addr, T *buf, size_t len) {
        fel_open_usb();
        size_t n;
        while (len > 0) {
            n = len > 65536 ? 65536 : len;
            fel_read_raw(addr, (void *) buf, n);
            addr += n;
            buf += n;
            len -= n;
        }
        fel_close_usb();
    };

    template<typename T>
    void fel_write(uint32_t addr, T *buf, size_t len) {
        fel_open_usb();
        size_t n;
        while (len > 0) {
            n = len > 65536 ? 65536 : len;
            fel_write_raw(addr, (void *) buf, n);
            addr += n;
            buf += n;
            len -= n;
        }
        fel_close_usb();
    };

private:
    void fel_chip_id();

    void fel_open_usb();

    void fel_close_usb();

    void send_fel_request(int type, uint32_t addr, uint32_t length);

    void read_fel_status();

    void fel_read_raw(uint32_t addr, void *buf, size_t len);

    void fel_write_raw(uint32_t addr, void *buf, size_t len);
};

#endif// FEL_H
