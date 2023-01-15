/*
 * Copyright (c) 2022, YuzukiTsuru <GloomyGhost@GloomyGhost.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * See README and LICENSE for more details.
 */

#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "exceptions.h"
#include "yfel_config.h"

#include <QClipboard>
#include <QMessageBox>
#include <QTimer>
#include <QFileDialog>
#include <qdesktopservices.h>

MainWindow::MainWindow(QWidget *parent)
        : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    initMainwindowData();
    initMenubar();

    connect(chip_op, &ChipOP::release_ui, this, [=]() {
        if (!this->isVisible()) {
            this->show();
        }
        releaseUI();
        updateStatusBar(tr("Done."));
    });

    ui->flash_spi_read_hexView->addWidget(spiNandReadHexView);
    ui->flash_spi_write_hexView->addWidget(spiNandWriteHexView);
    ui->run_file_hexView->addWidget(runHexView);
    ui->dump_data_hexView->addWidget(dumpHexView);

    QByteArray initHexArr(1000, 0);
    spiNandReadHexView->setData(new QHexView::DataStorageArray(initHexArr));
    spiNandWriteHexView->setData(new QHexView::DataStorageArray(initHexArr));
    runHexView->setData(new QHexView::DataStorageArray(initHexArr));
    dumpHexView->setData(new QHexView::DataStorageArray(initHexArr));

    ui->dram_dram_tpr13_label->installEventFilter(this);

    chipStatus.setNone();
}

MainWindow::~MainWindow() {
    qDebug() << "MainWindow::~MainWindow()";
    delete ui;
    delete chip_op;
    delete spiNandWriteHexView;
    delete spiNandReadHexView;
    delete runHexView;
    delete dumpHexView;
}

void MainWindow::initMainwindowData() {
    ui->chip_label_2->setText(tr("NONE"));
    ui->statusbar->showMessage(tr("Ready, Version: ") + PROJECT_GIT_HASH, 5000);
}

void MainWindow::initMenubar() {
    // menu exit
    ui->actionExit->setShortcut(QKeySequence::Quit);
    connect(ui->actionExit, &QAction::triggered, this, &MainWindow::exitMenuClicked);

    connect(ui->actionOpen, &QAction::triggered, this, [=]() -> void {
        QMessageBox::critical(this, tr("Error"), tr("Success"));
    });

    // menu about
    connect(ui->actionAbout_YFEL, &QAction::triggered, this, [this]() {
        QMessageBox::about(this, tr("About YFEL"),
                           tr("Copyright 2022 YuzukiTsuru\n\nGNU General Public License v3.0") + "\n\tVersion: " +
                           PROJECT_GIT_HASH);
    });

    // menu web
    connect(ui->actionWeb, &QAction::triggered, this, []() {
        QString URL = "https://github.com/YuzukiTsuru/YFEL";
        QDesktopServices::openUrl(QUrl(URL.toLatin1()));
    });

    // enable jtag
    connect(ui->actionEnable_JTAG, &QAction::triggered, this, &MainWindow::enableJtag);
    connect(ui->Misc_enable_jtag, &QPushButton::clicked, this, &MainWindow::enableJtag);

    // reset chip
    connect(ui->actionReset_CPU, &QAction::triggered, this, &MainWindow::chipReset);
    connect(ui->Misc_reset_pushButton, &QPushButton::clicked, this, &MainWindow::chipReset);
}

void MainWindow::exitMenuClicked() {
    QMessageBox msgBox;
    msgBox.setText(tr("Exit YFEL?"));
    msgBox.setWindowIcon(QIcon(":/assets/img/icon.png"));
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Yes);
    int ret = msgBox.exec();
    if (ret == QMessageBox::Yes) {
        QApplication::quit();
    }
}

void MainWindow::copyToClipboard(const QString &data, QPushButton *button) {
    qDebug() << "Copy " << data << "to Clip Board";
    QClipboard *clip = QApplication::clipboard();
    clip->setText(data);
    button->setText("√");
    QTimer::singleShot(500, this, [button]() { button->setText(tr("Copy")); });
}

QString MainWindow::openFileDialog(QLineEdit *lineEdit) {
    QString fileName =
            QFileDialog::getOpenFileName(this, tr("Open Image File"), "",
                                         tr("IMAGE (*.img *.IMG);;Binary (*.bin);;All files (*.*)"));
    lineEdit->setText(fileName);
    return fileName;
}

void MainWindow::loadDramPresets() {
    if (!chipStatus.isNone()) {
        auto dram_paras = chip_op->get_dram_params();
        ui->dram_load_preset_comboBox->clear();
        if (!dram_paras.isEmpty()) {
            for (const auto &item: dram_paras) {
                ui->dram_load_preset_comboBox->addItem(item.dram_param_name);
            }
        }
    }
}

void MainWindow::updateStatusBar(const QString &status) {
    ui->statusbar->showMessage(status, 1000);
}

void MainWindow::updateStatusBar(const QString &status, int time) {
    ui->statusbar->showMessage(status, time);
}

void MainWindow::on_scan_pushButton_clicked() {
    lockUI();
    updateStatusBar(tr("Scanning..."));
    try {
        chip_op->chip_scan_chip();
        chip_op->chip_sid();
        // Set Scan Button label
        ui->chip_label_2->setText("0x" + QString::number(chip_op->get_current_chip().chip_id, 16));

        // Set CHip lines
        ui->chip_name_lineEdit->setText(chip_op->get_current_chip().chip_name);
        ui->chip_id_lineEdit->setText("0x" + QString::number(chip_op->get_current_chip().chip_id, 16));
        ui->chip_sid_lineEdit->setText("0x" + chip_op->get_current_chip().chip_sid);

        QString chip_core_names_ =
                chip_op->get_current_chip().chip_core_count_str + " " + chip_op->get_current_chip().chip_core;
        if (chip_op->get_current_chip().chip_type == chip_type_e::Heterogeneous) {
            for (auto const &item: chip_op->get_current_chip().chip_heterogeneous_core) {
                chip_core_names_.append(" + ");
                chip_core_names_.append(item);
            }
        }
        ui->chip_core_lineEdit->setText(chip_core_names_);

        // update status bar
        updateStatusBar(tr("Done."));
        chipStatus.setOK();

        // load dram presets
        loadDramPresets();
    } catch (const cannot_find_fel_device &e) {
        QMessageBox::warning(this, tr("Warning"), tr("Can't find target FEL device"));
    } catch (const usb_bulk_send_error &e) {
        chipStatus.setError();
        scanChipWarning();
    } catch (const usb_bulk_recv_error &e) {
        chipStatus.setError();
        scanChipWarning();
    } catch (const usb_driver_wrong &e) {
        if (chipStatus.isError()) {
            scanChipWarning();
        } else {
            QMessageBox::warning(this, tr("Warning"),
                                 tr("Find FEL Device but host driver is wrong\n"
                                    "Please use libusb-win32 driver instead"));
        }
    } catch (const std::exception &e) {
        clearChipInfo();
        QMessageBox::warning(this, tr("Warning"), tr(e.what()));
    }

    releaseUI();
}

void MainWindow::on_chip_chip_name_pushButton_clicked() {
    copyToClipboard(ui->chip_name_lineEdit->text(), ui->chip_chip_name_pushButton);
}

void MainWindow::on_chip_chip_id_pushButton_2_clicked() {
    copyToClipboard(ui->chip_id_lineEdit->text(), ui->chip_chip_id_pushButton_2);
}

void MainWindow::on_chip_chip_sid_pushButton_3_clicked() {
    copyToClipboard(ui->chip_sid_lineEdit->text(), ui->chip_chip_sid_pushButton_3);
}

void MainWindow::on_chip_chip_core_pushButton_4_clicked() {
    copyToClipboard(ui->chip_core_lineEdit->text(), ui->chip_chip_core_pushButton_4);
}

void MainWindow::on_chip_spi_nor_scan_pushButton_clicked() {
    scanSpiNor();
}

void MainWindow::on_chip_spi_nand_scan_pushButton_clicked() {
    scanSpiNand();
}

QString MainWindow::fixedUint32ToString(uint32_t value) {
    return "0x" + QString::number(value, 16).toUpper().rightJustified(8, '0');
}

void MainWindow::enableJtag() {
    qDebug() << "Enable Chip JTAG";
    if (chipStatus.isNone()) {
        scanChipWarning();
        return;
    }
    try {
        chip_op->chip_enable_jtag();
        QMessageBox::information(this, tr("Info"), tr("JTAG Enabled"));
    } catch (const function_not_implemented &e) {
        QMessageBox::warning(this, tr("Warning"), tr("Function is not implemented"));
    } catch (const std::exception &e) {
        QMessageBox::warning(this, tr("Warning"), tr(e.what()));
    }
}

void MainWindow::chipReset() {
    qDebug() << "Reset Chip";
    if (chipStatus.isNone()) {
        scanChipWarning();
        return;
    }
    try {
        chip_op->chip_reset_chip();
        clearChipInfo();
        QMessageBox::information(this, tr("Info"), tr("Chip Reseted"));
    } catch (const function_not_implemented &e) {
        QMessageBox::warning(this, tr("Warning"), tr("Function is not implemented"));
    } catch (const std::exception &e) {
        QMessageBox::warning(this, tr("Warning"), tr(e.what()));
    }
}

void MainWindow::clearChipInfo() {
    ui->chip_label_2->setText(tr("NONE"));
    ui->chip_id_lineEdit->setText("");
    ui->chip_name_lineEdit->setText("");
    ui->chip_sid_lineEdit->setText("");
    ui->chip_core_lineEdit->setText("");
    ui->chip_spi_nand_lineEdit->setText("");
    ui->flash_spi_erase_spi_nand_currect_nand_chip_lineEdit->setText("");
    ui->flash_spi_erase_spi_nor_currect_nor_chip_lineEdit->setText("");
}

void MainWindow::on_Misc_exec_addr_btn_clicked() {
    if (chipStatus.isNone()) {
        scanChipWarning();
        return;
    }
    // Check whether the address is entered
    if (ui->Misc_exec_addr_lineEdit->text().isEmpty()) {
        QMessageBox::warning(this, tr("Warning"), tr("Please enter address."));
        return;
    }
    try {
        // Get the address
        auto addrString = ui->Misc_exec_addr_lineEdit->text();
        bool convertStatus = false;
        uint32_t addr;

        // Check whether the input address is HEX
        if (addrString.startsWith("0x")) {
            // In case of HEX, delete the first two char
            addrString.remove(0, 2);
            addr = static_cast<uint32_t>(addrString.toInt(&convertStatus, 16));
        } else {
            addr = static_cast<uint32_t>(addrString.toInt(&convertStatus));
        }
        // Check whether the address is actually entered
        if (convertStatus) {
            chip_op->chip_exec(addr);
            // After execution, the device will disconnect the link and clear the UI
            clearChipInfo();
            QMessageBox::information(this, tr("Info"),
                                     tr("Run command sent, now device disconnected"));
            chipStatus.setError();
        } else {
            QMessageBox::warning(this, tr("Warning"), tr("Please enter the correct address"));
        }
    } catch (const function_not_implemented &e) {
        QMessageBox::warning(this, tr("Warning"), tr("Function is not implemented"));
    } catch (const std::exception &e) {
        QMessageBox::warning(this, tr("Warning"), tr(e.what()));
    }
}

void MainWindow::on_tabWidget_currentChanged(int index) {
    qDebug() << "change tabWidget to: " << index;
    if (index == uiTabWidgetIndex::tab_dram) {
        loadDramPresets();
    }
}

void MainWindow::on_dram_load_preset_comboBox_currentIndexChanged() {
    // Prevention of cross-border
    if (chip_op->get_dram_params().length() >= 1) {
        auto current_dram_param = chip_op->get_dram_params()[0].dram_param;
        for (const auto &item: chip_op->get_dram_params()) {
            if (ui->dram_load_preset_comboBox->currentText() == item.dram_param_name) {
                current_dram_param = item.dram_param;
            }
        }
        ui->dram_dram_clk_lineEdit->setText(QString::number(current_dram_param.dram_clk));
        ui->dram_dram_type_lineEdit->setText(QString::number(current_dram_param.dram_type));

        ui->dram_dram_zq_lineEdit->setText(fixedUint32ToString(current_dram_param.dram_zq));

        ui->dram_dram_odt_en_lineEdit->setText(QString::number(current_dram_param.dram_odt_en));

        ui->dram_dram_para1_lineEdit->setText(fixedUint32ToString(current_dram_param.dram_para1));
        ui->dram_dram_para2_lineEdit->setText(fixedUint32ToString(current_dram_param.dram_para2));

        ui->dram_dram_mr0_lineEdit->setText(fixedUint32ToString(current_dram_param.dram_mr0));
        ui->dram_dram_mr1_lineEdit->setText(fixedUint32ToString(current_dram_param.dram_mr1));
        ui->dram_dram_mr2_lineEdit->setText(fixedUint32ToString(current_dram_param.dram_mr2));
        ui->dram_dram_mr3_lineEdit->setText(fixedUint32ToString(current_dram_param.dram_mr3));
        ui->dram_dram_tpr0_lineEdit->setText(fixedUint32ToString(current_dram_param.dram_tpr0));
        ui->dram_dram_tpr1_lineEdit->setText(fixedUint32ToString(current_dram_param.dram_tpr1));
        ui->dram_dram_tpr2_lineEdit->setText(fixedUint32ToString(current_dram_param.dram_tpr2));
        ui->dram_dram_tpr3_lineEdit->setText(fixedUint32ToString(current_dram_param.dram_tpr3));
        ui->dram_dram_tpr4_lineEdit->setText(fixedUint32ToString(current_dram_param.dram_tpr4));
        ui->dram_dram_tpr5_lineEdit->setText(fixedUint32ToString(current_dram_param.dram_tpr5));
        ui->dram_dram_tpr6_lineEdit->setText(fixedUint32ToString(current_dram_param.dram_tpr6));
        ui->dram_dram_tpr7_lineEdit->setText(fixedUint32ToString(current_dram_param.dram_tpr7));
        ui->dram_dram_tpr8_lineEdit->setText(fixedUint32ToString(current_dram_param.dram_tpr8));
        ui->dram_dram_tpr9_lineEdit->setText(fixedUint32ToString(current_dram_param.dram_tpr9));
        ui->dram_dram_tpr10_lineEdit->setText(fixedUint32ToString(current_dram_param.dram_tpr10));
        ui->dram_dram_tpr11_lineEdit->setText(fixedUint32ToString(current_dram_param.dram_tpr11));
        ui->dram_dram_tpr12_lineEdit->setText(fixedUint32ToString(current_dram_param.dram_tpr12));
        ui->dram_dram_tpr13_lineEdit->setText(fixedUint32ToString(current_dram_param.dram_tpr13));
    }
}

void MainWindow::on_dram_init_dram_btn_clicked() {
    dram_param_t dramParam{
            .dram_clk = ui->dram_dram_clk_lineEdit->text().toUInt(),
            .dram_type = ui->dram_dram_type_lineEdit->text().toUInt(),
            .dram_zq = ui->dram_dram_zq_lineEdit->text().remove(0, 2).toUInt(nullptr, 16),
            .dram_odt_en = ui->dram_dram_odt_en_lineEdit->text().toUInt(),
            .dram_para1 = ui->dram_dram_para1_lineEdit->text().remove(0, 2).toUInt(nullptr, 16),
            .dram_para2 = ui->dram_dram_para2_lineEdit->text().remove(0, 2).toUInt(nullptr, 16),
            .dram_mr0 = ui->dram_dram_mr0_lineEdit->text().remove(0, 2).toUInt(nullptr, 16),
            .dram_mr1 = ui->dram_dram_mr1_lineEdit->text().remove(0, 2).toUInt(nullptr, 16),
            .dram_mr2 = ui->dram_dram_mr2_lineEdit->text().remove(0, 2).toUInt(nullptr, 16),
            .dram_mr3 = ui->dram_dram_mr3_lineEdit->text().remove(0, 2).toUInt(nullptr, 16),
            .dram_tpr0 = ui->dram_dram_tpr0_lineEdit->text().remove(0, 2).toUInt(nullptr, 16),
            .dram_tpr1 = ui->dram_dram_tpr1_lineEdit->text().remove(0, 2).toUInt(nullptr, 16),
            .dram_tpr2 = ui->dram_dram_tpr2_lineEdit->text().remove(0, 2).toUInt(nullptr, 16),
            .dram_tpr3 = ui->dram_dram_tpr3_lineEdit->text().remove(0, 2).toUInt(nullptr, 16),
            .dram_tpr4 = ui->dram_dram_tpr4_lineEdit->text().remove(0, 2).toUInt(nullptr, 16),
            .dram_tpr5 = ui->dram_dram_tpr5_lineEdit->text().remove(0, 2).toUInt(nullptr, 16),
            .dram_tpr6 = ui->dram_dram_tpr6_lineEdit->text().remove(0, 2).toUInt(nullptr, 16),
            .dram_tpr7 = ui->dram_dram_tpr7_lineEdit->text().remove(0, 2).toUInt(nullptr, 16),
            .dram_tpr8 = ui->dram_dram_tpr8_lineEdit->text().remove(0, 2).toUInt(nullptr, 16),
            .dram_tpr9 = ui->dram_dram_tpr9_lineEdit->text().remove(0, 2).toUInt(nullptr, 16),
            .dram_tpr10 = ui->dram_dram_tpr10_lineEdit->text().remove(0, 2).toUInt(nullptr, 16),
            .dram_tpr11 = ui->dram_dram_tpr11_lineEdit->text().remove(0, 2).toUInt(nullptr, 16),
            .dram_tpr12 = ui->dram_dram_tpr12_lineEdit->text().remove(0, 2).toUInt(nullptr, 16),
            .dram_tpr13 = ui->dram_dram_tpr13_lineEdit->text().remove(0, 2).toUInt(nullptr, 16),
    };
    if (chipStatus.isOK()) {
        try {
            chip_op->chip_init_dram(dramParam);
            updateStatusBar("DRAM init done, Check the result in the UART log");
        } catch (const usb_bulk_send_error &e) {
            chipStatus.setError();
            scanChipWarning();
        } catch (const usb_bulk_recv_error &e) {
            chipStatus.setError();
            scanChipWarning();
        } catch (const std::exception &e) {
            chipStatus.setError();
            QMessageBox::warning(this, tr("Warning"), tr(e.what()));
        }
    } else {
        scanChipWarning();
    }
}

void MainWindow::on_flash_spi_erase_spi_nand_scan_button_clicked() {
    scanSpiNand();
}

void MainWindow::scanChipWarning() {
    if (chipStatus.isNone()) {
        QMessageBox::warning(this, tr("Warning"), tr("Chip not avaliable, try scan it"));
    } else if (chipStatus.isError()) {
        QMessageBox::warning(this, tr("Warning"),
                             tr("Chip operation error, please reset the chip manually"));
    } else {
        QMessageBox::warning(this, tr("Warning"), tr("Unknown error"));
    }

    // clear the chip info
    clearChipInfo();
}

void MainWindow::lockUI() {
    this->setEnabled(false);
}

void MainWindow::releaseUI() {
    this->setEnabled(true);
}

void MainWindow::scanSpiNand() {
    qDebug() << "Scanning SPI NAND...";
    if (!chipStatus.isOK()) {
        scanChipWarning();
        return;
    }
    updateStatusBar(tr("Scanning SPI NAND..."));
    try {
        // lock UI
        lockUI();

        // get spi nand info
        auto nandInfo = chip_op->chip_scan_spi_nand();

        // update ui
        ui->chip_spi_nand_lineEdit->setText(nandInfo);
        ui->flash_spi_erase_spi_nand_currect_nand_chip_lineEdit->setText(nandInfo);
        ui->flash_device_comboBox->addItem(nandInfo);
    } catch (const cannot_find_spi_flash_device &e) {
        ui->chip_spi_nand_lineEdit->setText(tr("No supported SPI NAND found"));
        ui->flash_spi_erase_spi_nand_currect_nand_chip_lineEdit->setText(tr("No supported SPI NAND found"));
    } catch (const function_not_implemented &e) {
        QMessageBox::warning(this, tr("Warning"), tr("Function is not implemented"));
    } catch (const std::runtime_error &e) {
        chipStatus.setNone();
        QMessageBox::warning(this, tr("Warning"), tr(e.what()));
    }
    releaseUI();
}

void MainWindow::on_flash_spi_erase_spi_nand_erase_button_clicked() {
    qDebug() << "Erasing SPI NAND...";
    if (!chipStatus.isOK()) {
        scanChipWarning();
        return;
    }
    updateStatusBar(tr("Erasing SPI NAND..."), 20000);
    try {
        lockUI();

        auto addr = ui->flash_spi_erase_spi_nand_addr_lineEdit->text().toUInt(nullptr, 10);
        if (ui->flash_spi_erase_spi_nand_addr_lineEdit->text().startsWith("0x"))
            addr = ui->flash_spi_erase_spi_nand_addr_lineEdit->text().remove(0, 2).toUInt(nullptr, 16);

        auto len = ui->flash_spi_erase_spi_nand_length_lineEdit->text().toUInt(nullptr, 10);
        if (ui->flash_spi_erase_spi_nand_length_lineEdit->text().startsWith("0x"))
            len = ui->flash_spi_erase_spi_nand_length_lineEdit->text().remove(0, 2).toUInt(nullptr, 16);

        chip_op->chip_erase_spi_nand(addr, len);
        QMessageBox::information(this, tr("Information"), tr("Erase SPI NAND successfully"));
    } catch (const function_not_implemented &e) {
        QMessageBox::warning(this, tr("Warning"), tr("Function is not implemented"));
    } catch (const spi_erase_out_of_range &e) {
        QMessageBox::warning(this, tr("Warning"), tr("Erase address out of range"));
    } catch (const std::runtime_error &e) {
        chipStatus.setNone();
        QMessageBox::warning(this, tr("Warning"), tr(e.what()));
    }

    releaseUI();
}

void MainWindow::on_flash_spi_erase_spi_nand_setall_button_clicked() {
    qDebug() << "Erasing All SPI NAND...";
    if (!chipStatus.isOK()) {
        scanChipWarning();
        return;
    }
    if (ui->flash_spi_erase_spi_nand_currect_nand_chip_lineEdit->text().isEmpty()) {
        QMessageBox::warning(this, tr("Warning"), tr("Please scan the SPI NAND first"));
        return;
    }

    updateStatusBar(tr("Erasing All SPI NAND..."), 20000);
    try {
        lockUI();
        chip_op->chip_erase_all_spi_nand();
        QMessageBox::information(this, tr("Information"), tr("Erase SPI NAND successfully"));
    } catch (const function_not_implemented &e) {
        QMessageBox::warning(this, tr("Warning"), tr("Function is not implemented"));
    } catch (const std::runtime_error &e) {
        chipStatus.setNone();
        QMessageBox::warning(this, tr("Warning"), tr(e.what()));
    }

    releaseUI();
}

void MainWindow::scanSpiNor() {
    qDebug() << "Scanning SPI NOR...";
    if (chipStatus.isNone()) {
        scanChipWarning();
        return;
    }
    try {
        lockUI();
        auto norInfo = chip_op->chip_scan_spi_nor();
        // update ui
        ui->chip_spi_nor_lineEdit->setText(norInfo);
        ui->flash_spi_erase_spi_nor_currect_nor_chip_lineEdit->setText(norInfo);
    } catch (const cannot_find_spi_flash_device &e) {
        ui->chip_spi_nor_lineEdit->setText(tr("No supported SPI NOR found"));
        ui->flash_spi_erase_spi_nor_currect_nor_chip_lineEdit->setText(tr("No supported SPI NOR found"));
    } catch (const function_not_implemented &e) {
        QMessageBox::warning(this, tr("Warning"), tr("Function is not implemented"));
    } catch (const std::runtime_error &e) {
        QMessageBox::warning(this, tr("Warning"), tr(e.what()));
    }
    releaseUI();
}

void MainWindow::on_flash_spi_erase_spi_nor_scan_button_clicked() {
    scanSpiNor();
}

void MainWindow::on_dram_load_preset_pushButton_clicked() {
    if (chipStatus.isNone()) {
        scanChipWarning();
        return;
    }
    loadDramPresets();
}

void MainWindow::on_flash_spi_read_pushButton_clicked() {
    qDebug() << "READ SPI NAND...";
    if (chipStatus.isNone()) {
        scanChipWarning();
        return;
    }
    try {
        auto addr = ui->flash_spi_read_addr_lineEdit->text().toUInt(nullptr, 10);
        if (ui->flash_spi_read_addr_lineEdit->text().startsWith("0x"))
            addr = ui->flash_spi_read_addr_lineEdit->text().remove(0, 2).toUInt(nullptr, 16);
        auto len = ui->flash_spi_read_length_lineEdit->text().toUInt(nullptr, 10);
        if (ui->flash_spi_read_length_lineEdit->text().startsWith("0x"))
            len = ui->flash_spi_read_length_lineEdit->text().remove(0, 2).toUInt(nullptr, 16);

        lockUI();
        spiNandReadHexView->clear();
        QByteArray arr = chip_op->chip_read_spi_nand(addr, len);
        spiNandReadHexView->setData(new QHexView::DataStorageArray(arr));
    } catch (const function_not_implemented &e) {
        QMessageBox::warning(this, tr("Warning"), tr("Function is not implemented"));
    } catch (const std::runtime_error &e) {
        QMessageBox::warning(this, tr("Warning"), tr(e.what()));
    }
    releaseUI();
}

void MainWindow::on_flash_spi_write_fileOpen_button_clicked() {
    auto fileName = QFileDialog::getOpenFileName(this, tr("Open Image File"), "",
                                                 tr("IMAGE (*.img *.IMG);;Binary (*.bin);;All files (*.*)"));
    ui->flash_spi_write_fileName_lineEdit->setText(fileName);
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::warning(this, tr("File opening fail"),
                             tr("Problem with open file `") + fileName + tr("` for reading"));
        return;
    }

    QByteArray fileBuf = file.readAll();
    spiNandWriteHexView->clear();
    spiNandWriteHexView->setData(new QHexView::DataStorageArray(fileBuf));
}

void MainWindow::on_flash_spi_write_button_clicked() {
    qDebug() << "WRITE SPI NAND...";
    if (chipStatus.isNone()) {
        scanChipWarning();
        return;
    }
    auto fileName = ui->flash_spi_write_fileName_lineEdit->text();
    QFile file(fileName);
    uint64_t fileSize = 0;
    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::warning(this, tr("File opening fail"),
                             tr("Problem with open file `") + fileName + tr("` for reading"));
        return;
    }
    fileSize = file.size();
    const QByteArray fileBuf = file.readAll();
    try {
        lockUI();
        auto addr = ui->flash_spi_write_addr_lineEdit->text().toUInt(nullptr, 10);
        if (ui->flash_spi_write_addr_lineEdit->text().startsWith("0x"))
            addr = ui->flash_spi_write_addr_lineEdit->text().remove(0, 2).toUInt(nullptr, 16);

        if (addr <= 0)
            QMessageBox::warning(this, tr("Warning"), tr("Invalid address"));

        chip_op->chip_write_spi_nand(addr, fileBuf, fileSize);
    } catch (const function_not_implemented &e) {
        QMessageBox::warning(this, tr("Warning"), tr("Function is not implemented"));
    } catch (const std::runtime_error &e) {
        QMessageBox::warning(this, tr("Warning"), tr(e.what()));
    }
    releaseUI();
}

void MainWindow::on_flash_pushButton_1_clicked() {
    openFileDialog(ui->flash_lineEdit_1);
}

void MainWindow::on_flash_pushButton_2_clicked() {
    openFileDialog(ui->flash_lineEdit_2);
}

void MainWindow::on_flash_pushButton_3_clicked() {
    openFileDialog(ui->flash_lineEdit_3);
}

void MainWindow::on_flash_pushButton_4_clicked() {
    openFileDialog(ui->flash_lineEdit_4);
}

void MainWindow::on_flash_pushButton_5_clicked() {
    openFileDialog(ui->flash_lineEdit_5);
}

void MainWindow::on_flash_pushButton_6_clicked() {
    openFileDialog(ui->flash_lineEdit_6);
}

void MainWindow::on_flash_pushButton_7_clicked() {
    openFileDialog(ui->flash_lineEdit_7);
}

void MainWindow::on_flash_pushButton_8_clicked() {
    openFileDialog(ui->flash_lineEdit_8);
}


void MainWindow::on_flash_spi_read_button_clicked() {

}

void MainWindow::on_run_open_file_clicked() {
    auto fileName = QFileDialog::getOpenFileName(this, tr("Open Image File"), "",
                                                 tr("IMAGE (*.img *.IMG);;Binary (*.bin);;All files (*.*)"));
    ui->run_file_open_lineEdit->setText(fileName);
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::warning(this, tr("File opening fail"),
                             tr("Problem with open file `") + fileName + tr("` for reading"));
        return;
    }

    QByteArray fileBuf = file.readAll();
    runHexView->clear();
    runHexView->setData(new QHexView::DataStorageArray(fileBuf));
}

void MainWindow::on_run_run_button_clicked() {
    qDebug() << "Run Code withing BIN...";
    if (chipStatus.isNone()) {
        scanChipWarning();
        return;
    }
    auto fileName = ui->run_file_open_lineEdit->text();
    QFile file(fileName);
    uint64_t fileSize = 0;
    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::warning(this, tr("File opening fail"),
                             tr("Problem with open file `") + fileName + tr("` for reading"));
        return;
    }
    fileSize = file.size();
    const QByteArray fileBuf = file.readAll();

    auto addr = ui->run_run_address_lineEdit->text().toUInt(nullptr, 10);
    if (ui->run_run_address_lineEdit->text().startsWith("0x"))
        addr = ui->run_run_address_lineEdit->text().remove(0, 2).toUInt(nullptr, 16);

    if (addr <= 0)
        QMessageBox::warning(this, tr("Warning"), tr("Invalid address"));

    try {
        lockUI();
        chip_op->chip_write(addr, fileBuf, fileSize);
    } catch (const function_not_implemented &e) {
        QMessageBox::warning(this, tr("Warning"), tr("Function is not implemented"));
    } catch (const std::runtime_error &e) {
        QMessageBox::warning(this, tr("Warning"), tr(e.what()));
    }
    releaseUI();
}

void MainWindow::on_dump_do_dump_button_clicked() {
    qDebug() << "Dump Code from FEL..";
    if (chipStatus.isNone()) {
        scanChipWarning();
        return;
    }
    try {
        auto addr = ui->dump_address_lineEdit->text().toUInt(nullptr, 10);
        if (ui->dump_address_lineEdit->text().startsWith("0x"))
            addr = ui->dump_address_lineEdit->text().remove(0, 2).toUInt(nullptr, 16);
        auto length = ui->dump_length_lineEdit->text().toUInt(nullptr, 10);
        if (ui->dump_length_lineEdit->text().startsWith("0x"))
            length = ui->dump_length_lineEdit->text().remove(0, 2).toUInt(nullptr, 16);

        lockUI();
        auto data = chip_op->chip_read(addr, length);
        dumpHexView->clear();
        dumpHexView->setData(new QHexView::DataStorageArray(data));
    } catch (const usb_bulk_send_error &e) {
        QMessageBox::warning(this, tr("Warning"), tr("Dump error"));
    } catch (const function_not_implemented &e) {
        QMessageBox::warning(this, tr("Warning"), tr("Function is not implemented"));
    } catch (const std::runtime_error &e) {
        QMessageBox::warning(this, tr("Warning"), tr(e.what()));
    }
    releaseUI();
}

void MainWindow::on_dump_save_file_button_clicked() {
    QSaveFile file([this]() -> QString {
        return QFileDialog::getSaveFileName(this, tr("Save File"), "",
                                            tr("IMAGE (*.img *.IMG);;Binary (*.bin);;All files (*.*)"));
    }());
    if (!file.open(QIODevice::WriteOnly)) {
        QMessageBox::warning(this, tr("File opening fail"), tr("Problem with open file"));
        return;
    }
    file.write(dumpHexView->getData());
    file.commit();
}

bool MainWindow::eventFilter(QObject *obj, QEvent *event) {
    if (obj == ui->dram_dram_tpr13_label) {
        if (event->type() == QEvent::MouseButtonDblClick) {
            qDebug() << "dram_dram_tpr13_label";
        }
    }
    return true;
}

