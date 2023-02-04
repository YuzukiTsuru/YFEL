/*
 * Copyright (c) 2022, YuzukiTsuru <GloomyGhost@GloomyGhost.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 *  See README and LICENSE for more details.
 */   

#include "chip_status.h"

bool chip_status::isOK() {
    return chipStatus == chip_status_e::chip_ok;
}

bool chip_status::isError() {
    return chipStatus == chip_status_e::chip_error;
}

bool chip_status::isNone() {
    return chipStatus == chip_status_e::chip_none;
}

void chip_status::setOK() {
    chipStatus = chip_status_e::chip_ok;
}

void chip_status::setNone() {
    chipStatus = chip_status_e::chip_none;
}

void chip_status::setError() {
    chipStatus = chip_status_e::chip_error; 
}
