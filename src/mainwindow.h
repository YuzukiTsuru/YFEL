#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>

#include "fel.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_scan_pushButton_clicked();

    void on_chip_chip_name_pushButton_clicked();

    void on_chip_chip_id_pushButton_2_clicked();

    void on_chip_chip_sid_pushButton_3_clicked();

    void on_chip_chip_core_pushButton_4_clicked();

    void on_chip_dflag_pushButton_5_clicked();

    void on_chip_version_pushButton_6_clicked();

private:
    Ui::MainWindow *ui;
    fel _fel;

private:
    void initMainwindowData();

    void initMenubar();

    void initButtons();

private:
    static void exitMenuClicked();

    void updateStatusBar(const QString& status);

    void copyToClipboard(const QString& data, QPushButton *button);
};
#endif // MAINWINDOW_H
