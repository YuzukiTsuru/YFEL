/*
 * Copyright (c) 2022, YuzukiTsuru <GloomyGhost@GloomyGhost.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 *  See README and LICENSE for more details.
 */

//
// Created by YuzukiTsuru on 2022/10/3.
//

#ifndef YFEL_CHIP_STATUS_H
#define YFEL_CHIP_STATUS_H


class chip_status {
private:
    enum chip_status_e {
        chip_none,
        chip_ok,
        chip_error,
        chip_dram_inited,
    };

public:
    chip_status() = default;

    bool isOK();

    bool isError();

    bool isNone();

    void setOK();

    void setNone();

    void setError();

private:
    chip_status_e chipStatus = chip_status_e::chip_none;
};


#endif //YFEL_CHIP_STATUS_H
