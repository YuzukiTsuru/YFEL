#ifndef FEL_H
#define FEL_H

#include <QString>

#include "libusb-1.0/libusb.h"

#include "chips.h"

class fel {
private:
    typedef struct usb_ctx {
        libusb_device_handle *hdl = 0;
        int epout = 0;
        int epin = 0;
    } _usb_ctx_t;

    struct usb_request_t {
        char magic[8];
        uint32_t length;
        uint32_t unknown1;
        uint16_t request;
        uint32_t length2;
        char pad[10];
    } __attribute__((packed));

    struct fel_request_t {
        uint32_t request;
        uint32_t address;
        uint32_t length;
        uint32_t pad;
    } __attribute__((packed));

    typedef struct _version {
        char magic[8];
        uint32_t id;
        uint32_t firmware;
        uint16_t protocol;
        uint8_t dflag;
        uint8_t dlength;
        uint32_t scratchpad;
        uint8_t pad[8];
    } version_t;

    static const uint32_t usb_timeout = 10000;
    const char fel_send_magic[8] = {'A', 'W', 'U', 'C', '\0', '\0', '\0', '\0'};
    const char fel_recv_magic[8] = {'A', 'W', 'U', 'S', '\0', '\0', '\0', '\0'};

private:
    _usb_ctx_t *ctx;
    chip_t chip_;
    version_t version;

    libusb_device **list;
    libusb_context *context = 0;
    libusb_device_descriptor desc;

public:
    fel();

    ~fel();

    void fel_open_usb();

    void fel_scan_chip();

private:
    void fel_init();

    void fel_chip_id();

private:
    void usb_bulk_send(int ep, uint8_t *buf, size_t len);

    void usb_bulk_recv(int ep, uint8_t *buf, size_t len);

    void send_usb_request(int type, size_t length);

    void read_usb_response();

    void usb_write(const void *buf, size_t len);

    void usb_read(const void *data, size_t len);

    void send_fel_request(int type, uint32_t addr, uint32_t length);

    void read_fel_status();
};

#endif// FEL_H
