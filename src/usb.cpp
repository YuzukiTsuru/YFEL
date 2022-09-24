#include <QDebug>
#include <QFuture>
#include <QtConcurrent>

#include "usb.h"
#include "x.h"

usb::usb() = default;

void usb::usb_init() {
    libusb_init(&context);
    qDebug() << "USB INIT";
}

void usb::usb_exit() {
    libusb_exit(context);
    qDebug() << "DEINIT USB";
}

void usb::close_usb() const {
    libusb_close(ctx.hdl);
}

void usb::open_usb() {
    qDebug() << "scaning target chip";

    libusb_device **list;

    size_t count = libusb_get_device_list(context, &list);
    qDebug() << "scan usb devices, count=" << count;

    bool target_found = false;
    for (size_t i = 0; i < count; ++i) {
        libusb_device *device = list[i];
        int rc = libusb_get_device_descriptor(device, &desc);
        if (rc != 0) {
            qDebug("ERROR: Can't get device list: %d\r\n", rc);
        }
        if (desc.idVendor == 0x1f3a && desc.idProduct == 0xefe8) {
            rc = libusb_open(device, &ctx.hdl);
            if (rc != 0) {
                qDebug("ERROR: Can't connect to device: %d\r\n", rc);
                throw std::runtime_error("Find FEL Device but host driver is wrong\nPlease use libusb-win32 driver instead");
            } else {
                uint8_t string_buffer_product[4096];
                libusb_get_string_descriptor_ascii(ctx.hdl, desc.iProduct,
                                                   string_buffer_product, sizeof(string_buffer_product));
                qDebug() << "Find Device: " << string_buffer_product;
                target_found = true;
                break;
            }
        }
    }
    if (target_found == 0) {
        throw std::runtime_error("Can't find target FEL device");
    }
}

void usb::usb_fel_init() {
    struct libusb_config_descriptor *config;
    const struct libusb_interface *iface;
    const struct libusb_interface_descriptor *setting;
    const struct libusb_endpoint_descriptor *ep;
    int if_idx, set_idx, ep_idx;

    if (libusb_claim_interface(ctx.hdl, 0) == 0) {
        if (libusb_get_active_config_descriptor(libusb_get_device(ctx.hdl), &config) == 0) {
            for (if_idx = 0; if_idx < config->bNumInterfaces; if_idx++) {
                iface = config->interface + if_idx;
                for (set_idx = 0; set_idx < iface->num_altsetting; set_idx++) {
                    setting = iface->altsetting + set_idx;
                    for (ep_idx = 0; ep_idx < setting->bNumEndpoints; ep_idx++) {
                        ep = setting->endpoint + ep_idx;
                        if ((ep->bmAttributes & LIBUSB_TRANSFER_TYPE_MASK) != LIBUSB_TRANSFER_TYPE_BULK)
                            continue;
                        if ((ep->bEndpointAddress & LIBUSB_ENDPOINT_DIR_MASK) == LIBUSB_ENDPOINT_IN)
                            ctx.epin = ep->bEndpointAddress;
                        else
                            ctx.epout = ep->bEndpointAddress;
                    }
                }
            }
            libusb_free_config_descriptor(config);
        }
    }
}

void usb::usb_bulk_send(int ep, uint8_t *buf, size_t len) const {
    size_t max_chunk = 128 * 1024;
    int bytes;

    while (len > 0) {
        size_t chunk = len < max_chunk ? len : max_chunk;
        int r = libusb_bulk_transfer(ctx.hdl, ep, buf, static_cast<int>(chunk), &bytes, usb_timeout);
        if (r != 0) {
            throw std::runtime_error("usb_bulk_send failed");
        }
        len -= bytes;
        buf += bytes;
    }
}

void usb::usb_bulk_recv(int ep, uint8_t *buf, size_t len) const {
    int bytes;

    while (len > 0) {
        int r = libusb_bulk_transfer(ctx.hdl, ep, buf, static_cast<int>(len), &bytes, usb_timeout);
        if (r != 0) {
            throw std::runtime_error("usb_bulk_recv failed");
        }
        len -= bytes;
        buf += bytes;
    }
}

void usb::send_usb_request(int type, size_t length) {
    usb_request_t req{};
    for (size_t i = 0; i < 8; ++i) {
        req.magic[i] = fel_send_magic[i];
    }
    req.request = cpu_to_le16(type);
    req.length = cpu_to_le32(length);
    req.unknown1 = cpu_to_le32(0x0c000000);
    req.length2 = req.length;

    usb_bulk_send(ctx.epout, (uint8_t *) &req, sizeof(struct usb_request_t));
}

void usb::read_usb_response() {
    char buf[13];
    usb_bulk_recv(ctx.epin, (uint8_t *) buf, sizeof(buf));
    for (size_t i = 0; i < 4; ++i) {
        if (buf[i] != fel_recv_magic[i]) {
            throw std::runtime_error("read usb response failed");
        }
    }
}

void usb::usb_write(const void *buf, size_t len) {
    send_usb_request(0x12, len);
    usb_bulk_send(ctx.epout, (uint8_t *) buf, len);
    read_usb_response();
}

void usb::usb_read(void *data, size_t len) {
    send_usb_request(0x11, len);
    usb_bulk_send(ctx.epin, (uint8_t *) data, len);
    read_usb_response();
}

