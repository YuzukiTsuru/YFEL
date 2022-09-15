#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "yfel_config.h"

#include <QMessageBox>
#include <QClipboard>
#include <QTimer>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    initMainwindowData();
    initMenubar();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initMainwindowData()
{
    ui->chip_label_2->setText(tr("NONE"));
    ui->statusbar->showMessage(tr("Ready, Version: ") + PROJECT_GIT_HASH, 5000);
}

void MainWindow::initMenubar()
{
    // menu exit
    ui->actionExit->setShortcut(QKeySequence::Quit);
    connect(ui->actionExit, &QAction::triggered, this, &MainWindow::exitMenuClicked);

    // menu about
    connect(ui->actionAbout_YFEL, &QAction::triggered, this, &MainWindow::aboutMenuClicked);

    // menu web
}

void MainWindow::exitMenuClicked()
{
    QMessageBox msgBox;
    msgBox.setText(tr("Exit YFEL?"));
    msgBox.setWindowIcon(QIcon(":/assets/img/icon.png"));
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Yes);
    int ret = msgBox.exec();
    if(ret == QMessageBox::Yes)
    {
        QApplication::quit();
    }
}

void MainWindow::copyToClipboard(QString data, QPushButton *button)
{
    qDebug() << "Copy " << data << "to Clip Board";
    QClipboard *clip = QApplication::clipboard();
    clip->setText(data);
    button->setText("√");
    QTimer::singleShot(500, this, [button](){button->setText(tr("Copy"));});
}

void MainWindow::aboutMenuClicked()
{
    QMessageBox::about(this, tr("About YFEL"), tr("Copyright 2022 YuzukiTsuru\n\nGNU General Public License v3.0") + "\n\tVersion: " + PROJECT_GIT_HASH);
}

void MainWindow::updateStatusBar(QString status)
{
    ui->statusbar->showMessage(status);
}

void MainWindow::on_scan_pushButton_clicked()
{
    qDebug() << "scaning target chip";
}

void MainWindow::on_chip_chip_name_pushButton_clicked()
{
    copyToClipboard(ui->chip_name_lineEdit->text(), ui->chip_chip_name_pushButton);
}

void MainWindow::on_chip_chip_id_pushButton_2_clicked()
{
    copyToClipboard(ui->chip_id_lineEdit->text(), ui->chip_chip_id_pushButton_2);
}


void MainWindow::on_chip_chip_sid_pushButton_3_clicked()
{
    copyToClipboard(ui->chip_sid_lineEdit->text(), ui->chip_chip_sid_pushButton_3);
}

void MainWindow::on_chip_chip_core_pushButton_4_clicked()
{
    copyToClipboard(ui->chip_core_lineEdit->text(), ui->chip_chip_core_pushButton_4);
}

void MainWindow::on_chip_dflag_pushButton_5_clicked()
{
    copyToClipboard(ui->chip_dflag_lineEdit->text(), ui->chip_dflag_pushButton_5);
}

void MainWindow::on_chip_version_pushButton_6_clicked()
{
    copyToClipboard(ui->chip_version_lineEdit->text(), ui->chip_version_pushButton_6);
}

