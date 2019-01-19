#ifndef KARIN_FILECMPDIALOG_H
#define KARIN_FILECMPDIALOG_H

#include <QDialog>

namespace Ui {
class karin_FileCmpDialog;
}

class karin_FileCmpDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit karin_FileCmpDialog(QWidget *parent = 0);
    ~karin_FileCmpDialog();

private Q_SLOTS:
    void openfilechooser1();
    void openfilechooser2();
    void setfile1fromedit();
    void setfile2fromedit();
    void filecmp();
    void celldbclick(int row, int col);

private:
    void init();
    void clearres();
    void showres();
    void setfile(QString &src, const QString &f, const QString &setting);
    void mktblcell(int row, int col, const QString &sum);
    
private:
    Ui::karin_FileCmpDialog *ui;

    QString m_filepath1;
    QString m_filepath2;
};

#endif // KARIN_FILECMPDIALOG_H
