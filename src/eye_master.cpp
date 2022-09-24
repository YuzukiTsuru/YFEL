#include "eye_master.h"
#include "ui_eye_master.h"

eye_master::eye_master(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::eye_master)
{
    ui->setupUi(this);
}

eye_master::~eye_master()
{
    delete ui;
}
