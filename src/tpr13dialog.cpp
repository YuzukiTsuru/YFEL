#include "tpr13dialog.h"
#include "ui_tpr13dialog.h"

#include "utils.h"

TPR13Dialog::TPR13Dialog(QWidget *parent) : QDialog(parent), ui(new Ui::TPR13Dialog) {
    ui->setupUi(this);
}

TPR13Dialog::~TPR13Dialog() {
    delete ui;
}

void TPR13Dialog::updateTPR13UIValue() {
    if (dramTpr13Value.dram_size_auto_det != 0){
        ui->dram_size_auto_det_checkBox->setChecked(true);
    }

}

void TPR13Dialog::on_buttonBox_accepted() {
    emit GetTPR13Data(TPRConfigValue);
}

void TPR13Dialog::SetTPR13Value(uint32_t value) {
    qDebug("TPR13Dialog Get Value: 0x%x", value);
    TPRConfigValue = value;
    dramTpr13Value = *(dram_tpr13_t *) &value;
    ui->config_label_4->setText(fixedUint32ToString((value)));
    updateTPR13UIValue();
}
