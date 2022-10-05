#include "processbar.h"

#include <utility>
#include "ui_processbar.h"

processbar::processbar(QWidget *parent) :
        QDialog(parent),
        ui(new Ui::processbar) {
    ui->setupUi(this);
}

processbar::~processbar() {
    delete ui;
}

void processbar::initWork(uint64_t count, QString name) {
    count_ = count;
    name_ = std::move(name);
    ui->working_name_label->setText(name_);
}
