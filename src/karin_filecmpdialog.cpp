#include "karin_filecmpdialog.h"
#include "ui_karin_filecmpdialog.h"
#include "karin_ut.h"
#include "karin_std.h"

#include <QFileInfo>
#include <QFileDialog>
#include <QApplication>
#include <QClipboard>
#include <QMessageBox>

karin_FileCmpDialog::karin_FileCmpDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::karin_FileCmpDialog)
{
    ui->setupUi(this);

    init();

    connect(ui->filechooser_lineedit1, SIGNAL(returnPressed()), this, SLOT(setfile1fromedit()));
    connect(ui->filechooser_lineedit2, SIGNAL(returnPressed()), this, SLOT(setfile2fromedit()));
    connect(ui->filechooser_btn1, SIGNAL(clicked()), this, SLOT(openfilechooser1()));
    connect(ui->filechooser_btn2, SIGNAL(clicked()), this, SLOT(openfilechooser2()));
    connect(ui->cmp_btn, SIGNAL(clicked()), this, SLOT(filecmp()));
    connect(ui->sum_table, SIGNAL(cellDoubleClicked(int, int)), this, SLOT(celldbclick(int, int)));
}

karin_FileCmpDialog::~karin_FileCmpDialog()
{
    delete ui;
}

void karin_FileCmpDialog::init()
{
    setFixedSize(size());
    setWindowTitle(tr("File Compare"));

    m_filepath1 = karin_UT::Instance()->getsetting<QString>(SETTING_LAST_OPEN_CMP_FILE1_PATH);
    m_filepath2 = karin_UT::Instance()->getsetting<QString>(SETTING_LAST_OPEN_CMP_FILE2_PATH);
    ui->filechooser_lineedit1->setText(m_filepath1);
    ui->filechooser_lineedit2->setText(m_filepath2);

    ui->result_frame->setVisible(false);
}

void karin_FileCmpDialog::setfile(QString &src, const QString &f, const QString &setting)
{
    if(src == f)
        return;

    src = f;
    ui->filechooser_lineedit1->setText(src);
    karin_UT::Instance()->setsetting<QString>(setting, src);
}

void karin_FileCmpDialog::setfile1fromedit()
{
    setfile(m_filepath1, ui->filechooser_lineedit1->text(), SETTING_LAST_OPEN_CMP_FILE1_PATH);
}

void karin_FileCmpDialog::setfile2fromedit()
{
    setfile(m_filepath2, ui->filechooser_lineedit2->text(), SETTING_LAST_OPEN_CMP_FILE2_PATH);
}

void karin_FileCmpDialog::openfilechooser1()
{
    QFileInfo fileinfo(karin_UT::Instance()->getsetting<QString>(SETTING_LAST_OPEN_CMP_FILE1_PATH));
    QString dir = fileinfo.isDir() ? fileinfo.absoluteFilePath() : fileinfo.absolutePath();
    QString file = QFileDialog::getOpenFileName(this, tr("Open first file"), dir);

    if(file.isEmpty())
        return;

    setfile(m_filepath1, file, SETTING_LAST_OPEN_CMP_FILE1_PATH);

}

void karin_FileCmpDialog::openfilechooser2()
{
    QFileInfo fileinfo(karin_UT::Instance()->getsetting<QString>(SETTING_LAST_OPEN_CMP_FILE2_PATH));
    QString dir = fileinfo.isDir() ? fileinfo.absoluteFilePath() : fileinfo.absolutePath();
    QString file = QFileDialog::getOpenFileName(this, tr("Open second file"), dir);

    if(file.isEmpty())
        return;

    setfile(m_filepath2, file, SETTING_LAST_OPEN_CMP_FILE2_PATH);
}

void karin_FileCmpDialog::filecmp()
{
    if(m_filepath1.isEmpty() || m_filepath2.isEmpty())
        return;

    setCursor(QCursor(Qt::WaitCursor));
    setfile1fromedit();
    setfile2fromedit();
    clearres();

    QByteArray md5_1 = filemd5(m_filepath1);
    QByteArray md5_2 = filemd5(m_filepath2);
    QByteArray sha1_1 = filesha1(m_filepath1);
    QByteArray sha1_2 = filesha1(m_filepath2);

    QString md5str_1(md5_1.toHex());
    QString md5str_2(md5_2.toHex());
    QString sha1str_1(sha1_1.toHex());
    QString sha1str_2(sha1_2.toHex());

    mktblcell(0, 0, md5str_1);
    mktblcell(0, 1, md5str_2);
    mktblcell(1, 0, sha1str_1);
    mktblcell(1, 1, sha1str_2);

    mktblcell(2, 0, md5_1 == md5_2 ? tr("Equal") : tr("Not equal"));
    mktblcell(2, 1, sha1_1 == sha1_2 ? tr("Equal") : tr("Not equal"));

    ui->result_label->setText((md5_1 == md5_2 && sha1_1 == sha1_2) ? tr("File is same") : tr("File is not same"));

    showres();

    setCursor(QCursor(Qt::ArrowCursor));
}

void karin_FileCmpDialog::clearres()
{
    ui->result_frame->setVisible(false);
    ui->cmp_btn->setText(tr("Comparing..."));
    ui->cmp_btn->setCheckable(false);
    ui->filechooser_btn1->setCheckable(false);
    ui->filechooser_btn2->setCheckable(false);
}

void karin_FileCmpDialog::showres()
{
    ui->result_frame->setVisible(true);
    ui->cmp_btn->setText(tr("Compare"));
    ui->cmp_btn->setCheckable(true);
    ui->filechooser_btn1->setCheckable(true);
    ui->filechooser_btn2->setCheckable(true);
}

void karin_FileCmpDialog::mktblcell(int row, int col, const QString &sum)
{
    QTableWidgetItem *item;

    item = new QTableWidgetItem(sum);

    if(row <= 1 && col <= 1)
    {
        item->setStatusTip(item->text());
        item->setToolTip(item->text());
        item->setWhatsThis(tr("File %1 %2 %3: %4").arg(row).arg(row == 0 ? m_filepath1 : m_filepath2).arg(col == 0 ? tr("MD5") : tr("SHA1")).arg(item->text()));
    }
    item->setTextAlignment(Qt::AlignCenter);
    ui->sum_table->setItem(row, col, item);
}

void karin_FileCmpDialog::celldbclick(int row, int col)
{
    QTableWidgetItem *item;

    if(row >= 2 || col >= 2)
        return;

    item = ui->sum_table->item(row, col);

    if(item)
    {
        QApplication::clipboard()->setText(item->text());
        QMessageBox::information(this, tr("Info"), tr("File %1 sum has copy to clipboard").arg(col == 0 ? tr("MD5") : tr("SHA1")));
    }
}
