/*
 * Copyright (c) 2022, YuzukiTsuru <GloomyGhost@GloomyGhost.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * See README and LICENSE for more details.
 */

#ifndef YFEL_MAINWINDOW_H
#define YFEL_MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>

#include "chipop.h"
#include "chip_status.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

    ~MainWindow() override;

private slots:

    void on_scan_pushButton_clicked();

    void on_chip_chip_name_pushButton_clicked();

    void on_chip_chip_id_pushButton_2_clicked();

    void on_chip_chip_sid_pushButton_3_clicked();

    void on_chip_chip_core_pushButton_4_clicked();

    void on_chip_spi_nor_scan_pushButton_clicked();

    void on_chip_spi_nand_scan_pushButton_clicked();

    void on_Misc_exec_addr_btn_clicked();

private:
    Ui::MainWindow *ui;

    QAction *exitAct{};

    QFutureWatcher<QString> spi_nand_watcher;

    ChipOP *chip_op = new ChipOP();

    chip_status chipStatus;

private:
    void initMainwindowData();

    void initMenubar();

private:
    static void exitMenuClicked();

    void scanChipWarning();

    void updateStatusBar(const QString &status);

    void copyToClipboard(const QString &data, QPushButton *button);

    void enableJtag();

    void chipReset();

    void clearChipInfo();
};

#endif // YFEL_MAINWINDOW_H
