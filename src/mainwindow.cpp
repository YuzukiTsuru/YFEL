#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "yfel_config.h"

#include <QMessageBox>

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

