#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "karin_filewindow.h"
#include "karin_fileengine.h"
#include "karin_progressdialog.h"
#include "karin_ut.h"
#include "karin_std.h"
#include "karin_settingdialog.h"
#include "karin_glsplash.h"

#include <QTimer>
#include <QMessageBox>
#include <QDebug>

#define WRITE_SETTING_DECL(T, t) \
    void MainWindow::write_##t(const T &v) \
{ \
    karin_UT::Instance()->setsetting<T>(t, v); \
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_left(new karin_FileWindow(this)),
    m_right(new karin_FileWindow(this)),
    m_file(0),
    m_dialog(0)
{
    ui->setupUi(this);
    init();
    connect(m_left, SIGNAL(reqtransform(const QStringList &, const QString &)), this, SLOT(reqtransform_slot(const QStringList &, const QString &)));
    connect(m_right, SIGNAL(reqtransform(const QStringList &, const QString &)), this, SLOT(reqtransform_slot(const QStringList &, const QString &)));
    connect(m_left, SIGNAL(curpathChanged(const QString &)), this, SLOT(write_SETTING_LEFT_PATH(const QString &)));
    connect(m_right, SIGNAL(curpathChanged(const QString &)), this, SLOT(write_SETTING_RIGHT_PATH(const QString &)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::init()
{
    QMenu *menu;
    QAction *action;
    {
        menu = new QMenu(tr("&File"));

        action = new QAction(tr("&Setting"), this);
        connect(action, SIGNAL(triggered()), this, SLOT(opensettingdialog()));
        menu->addAction(action);

        action = new QAction(tr("&Exit"), this);
        connect(action, SIGNAL(triggered()), qApp, SLOT(quit()));
        menu->addAction(action);

        menuBar()->addMenu(menu);
    }
    {
        menu = new QMenu(tr("&Other"));

        action = new QAction(tr("&GL"), this);
        connect(action, SIGNAL(triggered()), this, SLOT(gl()));
        menu->addAction(action);

        action = new QAction(tr("&Changelog"), this);
        connect(action, SIGNAL(triggered()), this, SLOT(changelog()));
        menu->addAction(action);

        action = new QAction(tr("&About"), this);
        connect(action, SIGNAL(triggered()), this, SLOT(showabout()));
        menu->addAction(action);

        menuBar()->addMenu(menu);
    }

    ui->horizontalLayout->addWidget(m_left);
    ui->horizontalLayout->addWidget(m_right);
    m_left->setcurpath(karin_UT::Instance()->getsetting<QString>(SETTING_LEFT_PATH));
    m_right->setcurpath(karin_UT::Instance()->getsetting<QString>(SETTING_RIGHT_PATH));

    setWindowTitle(_APP_NAME);
}

void MainWindow::reqtransform_slot(const QStringList &src, const QString &dst)
{
    if(!m_file)
        m_file = new karin_FileEngine(this);
    else
    {
        if(m_file->state() != karin_FileEngine::FileEngine_Ready)
            return;
    }

    if(!m_dialog)
        m_dialog = new karin_ProgressDialog(this);
    m_file->prepare(src, dst);
    m_dialog->settitle(tr("Progress Dialog"));
    connect(m_file, SIGNAL(updating(int, const QString &)), this, SLOT(updating_slot(int, const QString &)));
    connect(m_dialog, SIGNAL(reqexit(int)), this, SLOT(reqexit_slot(int)));
    connect(m_dialog, SIGNAL(reqpause()), this, SLOT(reqpause_slot()));
    connect(m_dialog, SIGNAL(reqnext()), this, SLOT(reqnext_slot()));
    connect(m_file, SIGNAL(stateChanged(FileEngine_State_e)), this, SLOT(stateChanged_slot()));
    QTimer::singleShot(0, m_file, SLOT(scan()));
    m_dialog->exec();
    m_dialog->deleteLater();
    m_dialog = 0;
}

void MainWindow::updating_slot(int per, const QString &msg)
{
    m_dialog->progresscb(per, msg);
}

void MainWindow::showabout()
{
    QMessageBox::about(this, tr("About application"), karin_UT::Instance()->getabout());
}

void MainWindow::stateChanged_slot()
{
    karin_FileEngine::FileEngine_State_e s;

    s = m_file->state();

    m_dialog->setcurtitle(m_file->statestr());
    if(s == karin_FileEngine::FileEngine_Scanning)
    {
        m_dialog->settoolvisible(false, false);
        m_dialog->settoolstr("", tr("Cancel"));
    }
    else if(s == karin_FileEngine::FileEngine_Scanned)
    {
        m_dialog->settoolvisible(true, true);
        m_dialog->settoolstr(tr("Copy"), tr("Cancel"));
    }
    else if(s == karin_FileEngine::FileEngine_Mkdir)
    {
        m_dialog->settoolvisible(false, false);
        m_dialog->settoolstr("", tr("Cancel"));
    }
    else if(s == karin_FileEngine::FileEngine_Trans)
    {
        m_dialog->settoolvisible(false, false);
        m_dialog->settoolstr("", tr("Cancel"));
    }
    else if(s == karin_FileEngine::FileEngine_Transed)
    {
        m_dialog->settoolvisible(true, true);
        m_dialog->settoolstr(tr("Check"), tr("Done"));
    }
    else if(s == karin_FileEngine::FileEngine_Checking)
    {
        m_dialog->settoolvisible(false, false);
        m_dialog->settoolstr("", tr("Cancel"));
    }
    else if(s == karin_FileEngine::FileEngine_Done)
    {
        m_dialog->settoolvisible(true, false);
        m_dialog->settoolstr(tr("Cancel"), "");
    }
}

void MainWindow::changelog()
{
    QMessageBox::about(this, tr("Changelog"), frdtext(":/debian/changelog"));
}

void MainWindow::opensettingdialog()
{
    karin_SettingDialog *dialog;

    dialog = new karin_SettingDialog(this);

    dialog->exec();
}

void MainWindow::reqexit_slot(int result)
{
    //Q_UNUSED(result);

    m_file->cancel();
    m_file->deleteLater();
    m_file = 0;
    m_dialog = 0;
}

void MainWindow::reqpause_slot()
{
    karin_FileEngine::FileEngine_State_e s;

    s = m_file->state();

    if(s == karin_FileEngine::FileEngine_Transed)
    {
        m_dialog->done(0);
    }
}

void MainWindow::reqnext_slot()
{
    karin_FileEngine::FileEngine_State_e s;

    s = m_file->state();

    if(s == karin_FileEngine::FileEngine_Scanned)
    {
        m_file->mkdirs();
    }
    else if(s == karin_FileEngine::FileEngine_Transed)
    {
        m_file->check();
    }
}

void MainWindow::gl()
{
    QDialog *dialog;
    karin_GLSplash *splash;

    dialog = new QDialog(this);
    dialog->setWindowTitle("OpenGL Shadow Volume");
    dialog->setAttribute(Qt::WA_DeleteOnClose, true);
    splash = new karin_GLSplash(dialog);
    dialog->resize(splash->size());

    dialog->exec();
}

WRITE_SETTING_DECL(QString, SETTING_LEFT_PATH)
WRITE_SETTING_DECL(QString, SETTING_RIGHT_PATH)
