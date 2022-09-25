/*
 * Copyright (c) 2022, YuzukiTsuru <GloomyGhost@GloomyGhost.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * See README and LICENSE for more details.
 */

#ifndef EYE_MASTER_H
#define EYE_MASTER_H

#include <QWidget>

namespace Ui {
class eye_master;
}

class eye_master : public QWidget
{
    Q_OBJECT

public:
    explicit eye_master(QWidget *parent = nullptr);
    ~eye_master();

private:
    Ui::eye_master *ui;
};

#endif // EYE_MASTER_H
