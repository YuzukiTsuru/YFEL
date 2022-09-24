#ifndef EYE_MASTER_H
#define EYE_MASTER_H

#include <QWidget>

namespace Ui {
class eye_master;
}

class eye_master : public QWidget
{
    Q_OBJECT

public:
    explicit eye_master(QWidget *parent = nullptr);
    ~eye_master();

private:
    Ui::eye_master *ui;
};

#endif // EYE_MASTER_H
