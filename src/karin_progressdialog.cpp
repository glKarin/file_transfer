#include "karin_progressdialog.h"
#include "ui_karin_progressdialog.h"
#include "karin_std.h"

#include <QAbstractButton>
#include <QDebug>

#define FLOAT_TO_INT // qround

karin_ProgressDialog::karin_ProgressDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::karin_ProgressDialog)
{
    ui->setupUi(this);

    //FIXED_UI(this, ui->verticalLayoutWidget)

    init();

    connect(this, SIGNAL(finished(int)), this, SIGNAL(reqexit(int)));
    connect(btn(-1), SIGNAL(clicked()), this, SIGNAL(reqpause()));
    connect(btn(0), SIGNAL(clicked()), this, SIGNAL(reqnext()));
}

karin_ProgressDialog::~karin_ProgressDialog()
{
    delete ui;
}

void karin_ProgressDialog::progresscb(int cur, const QString &desc)
{
    ui->work_progressbar->setValue(cur < 0 || cur > 100 ? 100 : cur);
    ui->work_desc->setText(desc);
}

void karin_ProgressDialog::init()
{
    setWindowTitle(tr("Process Dialog"));
}

void karin_ProgressDialog::settitle(const QString &title)
{
    ui->title->setText(title);
}

void karin_ProgressDialog::setcurtitle(const QString &title)
{
    ui->work_label->setText(title);
}

void karin_ProgressDialog::setper(float f)
{
    if(f < 0.0 || f > 1.0)
        ui->work_progressbar->setValue(100);
    else
        ui->work_progressbar->setValue((int)FLOAT_TO_INT(f * 100));
}

void karin_ProgressDialog::setper(int i)
{
    if(i < 0 || i > 1)
        ui->work_progressbar->setValue(100);
    else
        ui->work_progressbar->setValue(i);
}

void karin_ProgressDialog::settoolstr(const QString &ok, const QString &no)
{
    QList<QAbstractButton *> btns = ui->tools->buttons();
    btns[0]->setText(ok);
    btns[1]->setText(no);
}

void karin_ProgressDialog::settoolvisible(bool ok, bool no)
{
    QList<QAbstractButton *> btns = ui->tools->buttons();
    btns[0]->setVisible(ok);
    btns[1]->setVisible(no);
}

QAbstractButton * karin_ProgressDialog::btn(int i)
{
    int index;
    QList<QAbstractButton *> btns = ui->tools->buttons();

    if(btns.size() == 0)
        return 0;

    if(i < 0)
        index = btns.size() + i;
    else
        index = i;

    if(index >= btns.size())
        index = btns.size() - 1;
    else if(index < 0)
        index = 0;
    qDebug()<<index;
    return btns[index];
}
