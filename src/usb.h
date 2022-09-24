#ifndef USB_H
#define USB_H

#include <QString>
#include <QObject>
#include <QThread>
#include <QList>
#include <QMultiMap>

#include "libusb-1.0/libusb.h"

#include "chipdb.h"

class usb : public QObject {
Q_OBJECT
public:
    usb();

    void usb_init();

    void usb_exit();

    void open_usb();

    void close_usb() const;

    void usb_fel_init();

    void usb_bulk_send(int ep, uint8_t *buf, size_t len) const;

    void usb_bulk_recv(int ep, uint8_t *buf, size_t len) const;

    void send_usb_request(int type, size_t length);

    void read_usb_response();

    void usb_write(const void *buf, size_t len);

    void usb_read(void *data, size_t len);

private:
    static const uint32_t usb_timeout = 1000;
    const char fel_send_magic[8] = {'A', 'W', 'U', 'C', '\0', '\0', '\0', '\0'};
    const char fel_recv_magic[8] = {'A', 'W', 'U', 'S', '\0', '\0', '\0', '\0'};

private:
    typedef struct usb_ctx {
        libusb_device_handle *hdl = nullptr;
        int epout = 0;
        int epin = 0;
    } usb_ctx_t;

    struct usb_request_t {
        char magic[8];
        uint32_t length;
        uint32_t unknown1;
        uint16_t request;
        uint32_t length2;
        char pad[10];
    } __attribute__((packed));

private:
    usb_ctx_t ctx;

    libusb_context *context{};
    libusb_device_descriptor desc{};
};

#endif // USB_H
