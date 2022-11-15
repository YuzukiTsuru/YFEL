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
#include <QLineEdit>

#include <lib/QHexView/QHexView.h>

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

    void on_tabWidget_currentChanged(int index);

    void on_dram_load_preset_comboBox_currentIndexChanged();

    void on_dram_init_dram_btn_clicked();

    void on_flash_spi_erase_spi_nand_scan_button_clicked();

    void on_flash_spi_erase_spi_nand_erase_button_clicked();

    void on_flash_spi_erase_spi_nand_setall_button_clicked();

    void on_flash_spi_erase_spi_nor_scan_button_clicked();

    void on_dram_load_preset_pushButton_clicked();

    void on_flash_spi_read_pushButton_clicked();

    void on_flash_spi_write_fileOpen_button_clicked();

    void on_flash_spi_write_button_clicked();

    void on_flash_pushButton_1_clicked();

    void on_flash_pushButton_2_clicked();

    void on_flash_pushButton_3_clicked();

    void on_flash_pushButton_4_clicked();

    void on_flash_pushButton_5_clicked();

    void on_flash_pushButton_6_clicked();

    void on_flash_pushButton_7_clicked();

    void on_flash_pushButton_8_clicked();

    void on_flash_spi_read_button_clicked();

private:
    Ui::MainWindow *ui;

    ChipOP *chip_op = new ChipOP();

    QHexView *spiNandReadHexView = new QHexView();

    QHexView *spiNandWriteHexView = new QHexView();

    chip_status chipStatus;

private:
    enum uiTabWidgetIndex {
        tab_chip = 0,
        tab_flash = 1,
        tab_run = 2,
        tab_dump = 3,
        tab_data = 4,
        tab_dram = 5,
        tab_misc = 6,
    };

private:
    void initMainwindowData();

    void initMenubar();

private:
    static void exitMenuClicked();

    static QString fixedUint32ToString(uint32_t value);

    void scanChipWarning();

    void updateStatusBar(const QString &status);

    void updateStatusBar(const QString &status, int time);

    void copyToClipboard(const QString &data, QPushButton *button);

    QString openFileDialog(QLineEdit *lineEdit);

    void loadDramPresets();

    void enableJtag();

    void chipReset();

    void clearChipInfo();

    void lockUI();

    void releaseUI();

    void scanSpiNand();

    void scanSpiNor();
};

#endif // YFEL_MAINWINDOW_H
