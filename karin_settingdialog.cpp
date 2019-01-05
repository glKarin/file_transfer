#include "karin_settingdialog.h"
#include "ui_karin_settingdialog.h"
#include "karin_std.h"

karin_SettingDialog::karin_SettingDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::karin_SettingDialog)
{
    ui->setupUi(this);

    //FIXED_UI(this, ui->verticalLayoutWidget)
}

karin_SettingDialog::~karin_SettingDialog()
{
    delete ui;
}
