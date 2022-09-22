#include "eyemaster.h"
#include "ui_eyemaster.h"

eyemaster::eyemaster(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::eyemaster)
{
    ui->setupUi(this);
}

eyemaster::~eyemaster()
{
    delete ui;
}
