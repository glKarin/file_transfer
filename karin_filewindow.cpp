#include "karin_filewindow.h"
#include "ui_karin_filewindow.h"
#include "karin_fsmodel.h"
#include "karin_fileview.h"

#include <QDir>
#include <QDebug>

karin_FileWindow::karin_FileWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::karin_FileWindow),
    m_view(new karin_FileView(this))
{
    ui->setupUi(this);
    init();
    connect(ui->dir_input, SIGNAL(editTextChanged(const QString &)), this, SLOT(setcurpath(const QString &)));
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
    if(m_dir == dir)
        return;
    m_dir = dir;
    for(int i = 0; i < ui->dir_input->count(); i++)
    {
        if(ui->dir_input->itemText(i) == m_dir)
        {
            ui->dir_input->removeItem(i);
            break;
        }
    }
    ui->dir_input->insertItem(0, m_dir);
    ui->dir_input->setCurrentIndex(0);
    emit curpathChanged(m_dir);
}
