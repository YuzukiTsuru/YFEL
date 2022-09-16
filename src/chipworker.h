#ifndef CHIPWORKER_H
#define CHIPWORKER_H

#include "chips.h"
#include "libusb-1.0/libusb.h"
#include <QString>

class ChipWorker
{
public:
     ChipWorker(libusb_device_handle *hdl);

    ~ChipWorker();

    void setChipID(uint64_t chip_id);

private:
    chip_t chip = {};
    libusb_device_handle *hdl_;
};

#endif // CHIPWORKER_H
