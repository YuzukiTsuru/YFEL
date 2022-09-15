#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

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

private:
    Ui::MainWindow *ui;
    QAction *exitAct;

private:
    void initMainwindowData();

    void initMenubar();

    void initButtons();

private:
    void exitMenuClicked();

    void aboutMenuClicked();

    void updateStatusBar(QString status);
};
#endif // MAINWINDOW_H
