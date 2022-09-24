#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "yfel_config.h"

#include "eyemaster.h"
#include "./ui_eyemaster.h"

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
                           tr("Copyright 2022 YuzukiTsuru\n\nGNU General Public License v3.0") + "\n\tVersion: " + PROJECT_GIT_HASH);
    });

    // menu web
    connect(ui->actionWeb, &QAction::triggered, this, []() {
        QString URL = "https://github.com/YuzukiTsuru/YFEL";
        QDesktopServices::openUrl(QUrl(URL.toLatin1()));
    });
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
        _fel.fel_scan_chip();
        ChipDB chipdb(_fel.fel_get_chip_version());
        // Set Scan Button label
        ui->chip_label_2->setText("0x" + QString::number(chipdb.get_currect_chip().chip_id, 16));

        // Set CHip lines
        ui->chip_name_lineEdit->setText(chipdb.get_currect_chip().chip_name);
        ui->chip_id_lineEdit->setText("0x" + QString::number(chipdb.get_currect_chip().chip_id, 16));

        QString chip_core_names_ = chipdb.get_currect_chip().chip_core_count_str + chipdb.get_currect_chip().chip_core;
        if (chipdb.get_currect_chip().chip_type == chip_type_e::Heterogeneous) {
            for (auto const &item: chipdb.get_currect_chip().chip_heterogeneous_core) {
                chip_core_names_.append(" + ");
                chip_core_names_.append(item);
            }
        }
        ui->chip_core_lineEdit->setText(chip_core_names_);

        // update status bar
        updateStatusBar(tr("Done."));
    } catch (const std::exception &e) {
        _fel.fel_force_close();
        ui->chip_label_2->setText(tr("NONE"));
        ui->chip_id_lineEdit->setText("");
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

void MainWindow::on_Misc_eyemaster_button_clicked() {
    auto *e = new eyemaster();
    e->setWindowTitle(tr("EYE Master") + " - " + PROJECT_NAME);
    e->setWindowIcon(QIcon(":/assets/img/icon.png"));
    e->show();
}

