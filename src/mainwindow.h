#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>

#include "chipdb.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
Q_OBJECT
private:
    enum chip_fel_e {
        fel_chip_none,
        fel_chip_ok,
    };

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

    void on_Misc_eyemaster_button_clicked();

    void on_Misc_reset_pushButton_clicked();

private:
    Ui::MainWindow *ui;
    QAction *exitAct{};

    chip_fel_e fel_status = fel_chip_none;

    ChipDB *chipdb = nullptr;

private:
    void initMainwindowData();

    void initMenubar();

private:
    static void exitMenuClicked();

    void updateStatusBar(const QString &status);

    void copyToClipboard(const QString &data, QPushButton *button);
};

#endif // MAINWINDOW_H
