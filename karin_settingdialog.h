#ifndef KARIN_SETTINGDIALOG_H
#define KARIN_SETTINGDIALOG_H

#include <QDialog>

namespace Ui {
class karin_SettingDialog;
}

class karin_SettingDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit karin_SettingDialog(QWidget *parent = 0);
    ~karin_SettingDialog();
    
private:
    Ui::karin_SettingDialog *ui;
};

#endif // KARIN_SETTINGDIALOG_H
