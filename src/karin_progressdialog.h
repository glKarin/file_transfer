#ifndef KARIN_PROGRESSDIALOG_H
#define KARIN_PROGRESSDIALOG_H

#include <QDialog>

class QAbstractButton;

namespace Ui {
class karin_ProgressDialog;
}

class karin_ProgressDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit karin_ProgressDialog(QWidget *parent = 0);
    ~karin_ProgressDialog();
    void settoolstr(const QString &ok, const QString &no);
    void settoolvisible(bool ok, bool no);
    QAbstractButton * btn(int i);

Q_SIGNALS:
    void reqexit(int result);
    void reqpause();
    void reqnext();

public Q_SLOTS:
    void progresscb(int cur, const QString &desc, const QString &log);
    void settitle(const QString &title);
    void setcurtitle(const QString &title);
    void setper(float f);
    void setper(int i);

private:
    void init();
    
private:
    Ui::karin_ProgressDialog *ui;
};

#endif // KARIN_PROGRESSDIALOG_H
