#include "mainwindow.h"
#include "./ui_mainwindow.h"

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
    ui->statusbar->showMessage(tr("Ready"));
}

void MainWindow::initMenubar()
{
    ui->actionExit->setShortcut(QKeySequence::Quit);
    connect(ui->actionExit, &QAction::triggered, this, &MainWindow::exitMenuClicked);
}

void MainWindow::exitMenuClicked()
{
    QMessageBox msgBox;
    msgBox.setText(tr("Exit YFEL?"));
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Yes);
    int ret = msgBox.exec();
    if(ret == QMessageBox::Yes)
    {
        QApplication::quit();
    }
}

void MainWindow::updateStatusBar(QString status)
{
    ui->statusbar->showMessage(status);
}

