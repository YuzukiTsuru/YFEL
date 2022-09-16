#ifndef FEL_H
#define FEL_H

#include <cstdint>
#include <libusb-1.0/libusb.h>

#define R32(reg)		fel_read32(ctx, reg)
#define W32(reg, val)	fel_write32(ctx, reg, val)

enum SPI_CMD {
    END				= 0x00,
    INIT			= 0x01,
    SELECT			= 0x02,
    DESELECT		= 0x03,
    FAST			= 0x04,
    TXBUF			= 0x05,
    RXBUF			= 0x06,
    SPINOR_WAIT		= 0x07,
    SPINAND_WAIT	= 0x08,
};

class fel
{
public:
    fel();

private:
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

    typedef struct _fel_ctx_t {
        libusb_device_handle * hdl;
        int epout;
        int epin;
    } fel_ctx_t;
private:
};

#endif // FEL_H
