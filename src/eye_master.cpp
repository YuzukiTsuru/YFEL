/*
 * Copyright (c) 2022, YuzukiTsuru <GloomyGhost@GloomyGhost.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * See README and LICENSE for more details.
 */

#include "eye_master.h"
#include "ui_eye_master.h"

eye_master::eye_master(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::eye_master)
{
    ui->setupUi(this);
}

eye_master::~eye_master()
{
    delete ui;
}
