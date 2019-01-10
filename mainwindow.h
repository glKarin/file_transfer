#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class karin_FileEngine;
class karin_ProgressDialog;

namespace Ui {
class MainWindow;
}

class karin_FileWindow;

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    void init();

private Q_SLOTS:
    void reqtransform_slot(const QStringList &src, const QString &dst);
    void updating_slot(int per, const QString &dir);
    void showabout();
    void changelog();
    void stateChanged_slot();
    void opensettingdialog();
    void write_SETTING_LEFT_PATH(const QString &);
    void write_SETTING_RIGHT_PATH(const QString &);
    void reqexit_slot(int result);
    void reqpause_slot();
    void reqnext_slot();
    void gl();
    
private:
    Ui::MainWindow *ui;
    karin_FileWindow *m_left;
    karin_FileWindow *m_right;
    karin_FileEngine *m_file;
    karin_ProgressDialog *m_dialog;
};

#endif // MAINWINDOW_H
