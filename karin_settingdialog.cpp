#include "karin_settingdialog.h"
#include "ui_karin_settingdialog.h"
#include "karin_std.h"
#include "karin_ut.h"
#include "karin_fileengine.h"

#include <QDebug>

karin_SettingDialog::karin_SettingDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::karin_SettingDialog)
{
    ui->setupUi(this);

    //FIXED_UI(this, ui->verticalLayoutWidget)

    init();
}

karin_SettingDialog::~karin_SettingDialog()
{
    delete ui;
}

void karin_SettingDialog::init()
{
    karin_UT *ut;

    ut = karin_UT::Instance();

    setWindowTitle(tr("Settings"));

    ui->enable_log->setChecked(ut->getsetting<bool>(SETTING_ENABLE_LOG));
    ui->thread->setValue(ut->getsetting<int>(SETTING_MAX_WORKING_THREAD));
    ui->thread->setMinimum(1);
    ui->thread->setMaximum(FILE_ENGINE_MAX_WORKING_THREAD);

    ui->log_level->hide();
    ui->log_level_label->hide();
    ui->enable_log->setCheckable(false);

    connect(ui->thread, SIGNAL(valueChanged(int)), this, SLOT(thread_slot(int)));
    connect(ui->enable_log, SIGNAL(stateChanged(int)), this, SLOT(enablelog_slot(int)));
}

void karin_SettingDialog::thread_slot(int v) const
{
    karin_UT::Instance()->setsetting<int>(SETTING_MAX_WORKING_THREAD, v);
}

void karin_SettingDialog::enablelog_slot(int v) const
{
    Qt::CheckState state;

    state = static_cast<Qt::CheckState>(v);
    switch(state)
    {
    case Qt::Unchecked:
        karin_UT::Instance()->setsetting<bool>(SETTING_ENABLE_LOG, false);
        break;
    case Qt::Checked:
        karin_UT::Instance()->setsetting<bool>(SETTING_ENABLE_LOG, true);
        break;
    case Qt::PartiallyChecked:
    default:
        break;
    }
}
