#include "karin_filewindow.h"
#include "ui_karin_filewindow.h"
#include "karin_fsmodel.h"
#include "karin_fileview.h"
#include "karin_std.h"

#include <QDir>
#include <QDebug>

static bool fileinfocmp(const QFileInfo &a, const QFileInfo &b)
{
    return a.absoluteFilePath() > b.absoluteFilePath();
}

karin_FileWindow::karin_FileWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::karin_FileWindow),
    m_view(new karin_FileView(this)),
    m_dirdrivercount(0)
{
    ui->setupUi(this);

    FIXED_UI(this, ui->verticalLayoutWidget)

    init();
    //connect(ui->dir_input, SIGNAL(editTextChanged(const QString &)), this, SLOT(setcurpath(const QString &)));
    connect(ui->dir_input, SIGNAL(activated(const QString &)), this, SLOT(setcurpath(const QString &)));
    connect(m_view, SIGNAL(reqtransform(const QStringList &, const QString &)), this, SIGNAL(reqtransform(const QStringList &, const QString &)));
    connect(m_view, SIGNAL(pathChanged(const QString &)), this, SLOT(settoptext(const QString &)));
}

karin_FileWindow::~karin_FileWindow()
{
    delete ui;
}

void karin_FileWindow::init()
{
    ui->main_layout->addWidget(m_view);
    QFileInfoList list = QDir::drives();
    //qSort(list.begin(), list.end(), fileinfocmp);
    m_dirdrivercount = list.count();
    Q_FOREACH(const QFileInfo &info, list)
    {
        ui->dir_input->addItem(QIcon(":/resc/image/icon-m-email-combined-mailbox.png"), info.absoluteFilePath());
    }
}

void karin_FileWindow::setcurpath(const QString &path)
{
    if(m_dir != path)
    {
        settoptext(path);
        m_view->setpath(m_dir);
    }
}

void karin_FileWindow::returnPressed_slot()
{
    //setcurpath(ui->dir_input->text());
}

void karin_FileWindow::settoptext(const QString &dir)
{
    bool isdrive;

    if(m_dir == dir)
        return;
    m_dir = dir;
    isdrive = false;

    for(int i = 0; i < ui->dir_input->count(); i++)
    {
        if(ui->dir_input->itemText(i) == m_dir)
        {
            if(i >= m_dirdrivercount)
            {
                ui->dir_input->removeItem(i);
            }
            else
            {
                ui->dir_input->setCurrentIndex(i);
                isdrive = true;
            }
            break;
        }
    }
    if(!isdrive)
    {
        ui->dir_input->insertItem(m_dirdrivercount, QIcon(":/resc/image/icon-m-content-url.png"), m_dir);
        ui->dir_input->setCurrentIndex(m_dirdrivercount);
    }
    emit curpathChanged(m_dir);
}
