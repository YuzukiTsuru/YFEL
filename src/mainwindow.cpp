/*
 * Copyright (c) 2022, YuzukiTsuru <GloomyGhost@GloomyGhost.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * See README and LICENSE for more details.
 */

#include "yfel_config.h"
#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QClipboard>
#include <QMessageBox>
#include <QTimer>
#include <qdesktopservices.h>

MainWindow::MainWindow(QWidget *parent)
        : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    initMainwindowData();
    initMenubar();
}

MainWindow::~MainWindow() {
    delete ui;
    delete chip_op;
}

void MainWindow::initMainwindowData() {
    ui->chip_label_2->setText(tr("NONE"));
    ui->statusbar->showMessage(tr("Ready, Version: ") + PROJECT_GIT_HASH, 5000);
    ui->flashfelflash_comboBox->addItem("SPI NAND");
    ui->flashfelflash_comboBox->addItem("SPI NOR");
}

void MainWindow::initMenubar() {
    // menu exit
    ui->actionExit->setShortcut(QKeySequence::Quit);
    connect(ui->actionExit, &QAction::triggered, this, &MainWindow::exitMenuClicked);

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

void MainWindow::updateStatusBar(const QString &status) {
    ui->statusbar->showMessage(status, 1000);
}

void MainWindow::on_scan_pushButton_clicked() {
    updateStatusBar(tr("Scanning..."));
    try {
        chip_op->scan_chip();
        // Set Scan Button label
        ui->chip_label_2->setText("0x" + QString::number(chip_op->get_current_chip().chip_id, 16));

        // Set CHip lines
        ui->chip_name_lineEdit->setText(chip_op->get_current_chip().chip_name);
        ui->chip_id_lineEdit->setText("0x" + QString::number(chip_op->get_current_chip().chip_id, 16));
        ui->chip_sid_lineEdit->setText("0x" + chip_op->get_current_chip().chip_sid);

        QString chip_core_names_ = chip_op->get_current_chip().chip_core_count_str + " "
                                   + chip_op->get_current_chip().chip_core;
        if (chip_op->get_current_chip().chip_type == chip_type_e::Heterogeneous) {
            for (auto const &item: chip_op->get_current_chip().chip_heterogeneous_core) {
                chip_core_names_.append(" + ");
                chip_core_names_.append(item);
            }
        }
        ui->chip_core_lineEdit->setText(chip_core_names_);

        // update status bar
        updateStatusBar(tr("Done."));
    } catch (const std::exception &e) {
        ui->chip_label_2->setText(tr("NONE"));
        ui->chip_id_lineEdit->setText("");
        ui->chip_name_lineEdit->setText("");
        ui->chip_sid_lineEdit->setText("");
        ui->chip_core_lineEdit->setText("");
        QMessageBox::warning(this, tr("Warning"), tr(e.what()));
    }
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
    qDebug() << "Scanning SPI NOR...";
}

void MainWindow::on_chip_spi_nand_scan_pushButton_clicked() {
    qDebug() << "Scanning SPI NAND...";
}

void MainWindow::enableJtag() {
    qDebug() << "Enable Chip JTAG";
    try {
        chip_op->enable_jtag();
        QMessageBox::information(this, tr("Info"), tr("JTAG Enabled"));
    } catch (const std::exception &e) {
        QMessageBox::warning(this, tr("Warning"), tr(e.what()));
    }
}

void MainWindow::chipReset() {
    qDebug() << "Reset Chip";
    try {
        chip_op->reset_chip();
        QMessageBox::information(this, tr("Info"), tr("Chip Reseted"));
    } catch (const std::exception &e) {
        QMessageBox::warning(this, tr("Warning"), tr(e.what()));
    }
}

