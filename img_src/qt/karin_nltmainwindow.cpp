#include "karin_nltmainwindow.h"
#include "ui_karin_nltmainwindow.h"
#include "karin_std.h"
#include "karin_ut.h"
#include "karin_nltviewer.h"

#include <QFileDialog>

karin_NLTMainWindow::karin_NLTMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::karin_NLTMainWindow),
    m_viewer(0)
{
    ui->setupUi(this);

    init();

    connect(ui->file_chooser_btn, SIGNAL(clicked()), this, SLOT(open()));
    connect(ui->tools->button(QDialogButtonBox::Open), SIGNAL(clicked()), this, SLOT(view()));
    connect(ui->tools->button(QDialogButtonBox::Save), SIGNAL(clicked()), this, SLOT(save()));
}

karin_NLTMainWindow::~karin_NLTMainWindow()
{
    delete ui;
}

void karin_NLTMainWindow::init()
{
    m_viewer = new karin_NLTViewer(this);
    ui->horizontalLayout_2->addWidget(m_viewer);

    m_filepath = karin_UT::Instance()->getsetting<QString>(SETTING_LAST_OPEN_IMAGE_PATH);
    ui->file_chooser_line->setText(m_filepath);

    setWindowTitle(tr("Image Converter"));
}

void karin_NLTMainWindow::open()
{
    QFileInfo fileinfo(karin_UT::Instance()->getsetting<QString>(SETTING_LAST_OPEN_IMAGE_PATH));
    QString dir = fileinfo.isDir() ? fileinfo.absoluteFilePath() : fileinfo.absolutePath();
    QString file = QFileDialog::getOpenFileName(this, tr("Open a image file"), dir);

    if(file.isEmpty() || m_filepath == file)
        return;

    m_filepath = file;

    ui->file_chooser_line->setText(m_filepath);
    karin_UT::Instance()->setsetting<QString>(SETTING_LAST_OPEN_IMAGE_PATH, m_filepath);
}

void karin_NLTMainWindow::view()
{
    m_viewer->open(m_filepath);
}

void karin_NLTMainWindow::save()
{
    QFileInfo fileinfo(karin_UT::Instance()->getsetting<QString>(SETTING_LAST_SAVE_IMAGE_PATH));
    QString dir = fileinfo.isDir() ? fileinfo.absoluteFilePath() : fileinfo.absolutePath();
    QString file = QFileDialog::getSaveFileName(this, tr("Save texture file"), dir);

    karin_UT::Instance()->setsetting<QString>(SETTING_LAST_SAVE_IMAGE_PATH, file);

    m_viewer->tonlt(file, m_filepath);
}
