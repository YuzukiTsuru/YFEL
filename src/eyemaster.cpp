#include "eyemaster.h"
#include "ui_eyemaster.h"

eyemaster::eyemaster(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::eyemaster)
{
    ui->setupUi(this);
    ui->statusbar->showMessage(tr("Ready"), 5000);
}

eyemaster::~eyemaster()
{
    delete ui;
}
