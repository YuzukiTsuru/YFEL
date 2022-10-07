#ifndef PROCESSBAR_H
#define PROCESSBAR_H

#include <QDialog>

namespace Ui {
    class processbar;
}

class processbar : public QDialog {
Q_OBJECT

public:
    explicit processbar(QWidget *parent = nullptr);

    ~processbar() override;

    void initWork(uint64_t count, QString name);

private:
    Ui::processbar *ui;
    uint64_t count_ = 0;
    QString name_ = {};
};

#endif // PROCESSBAR_H
