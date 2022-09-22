#ifndef EYEMASTER_H
#define EYEMASTER_H

#include <QMainWindow>

namespace Ui {
class eyemaster;
}

class eyemaster : public QMainWindow
{
    Q_OBJECT

public:
    explicit eyemaster(QWidget *parent = nullptr);
    ~eyemaster();

private:
    Ui::eyemaster *ui;
};

#endif // EYEMASTER_H
