#include "karin_settingdialog.h"
#include "ui_karin_settingdialog.h"

karin_SettingDialog::karin_SettingDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::karin_SettingDialog)
{
    ui->setupUi(this);
}

karin_SettingDialog::~karin_SettingDialog()
{
    delete ui;
}
