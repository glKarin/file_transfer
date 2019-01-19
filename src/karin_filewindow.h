#ifndef KARIN_FILEWINDOW_H
#define KARIN_FILEWINDOW_H

#include <QWidget>

class QModelIndex;
class karin_FileView;

namespace Ui {
class karin_FileWindow;
}

class karin_FileWindow : public QWidget
{
    Q_OBJECT
    
public:
    explicit karin_FileWindow(QWidget *parent = 0);
    ~karin_FileWindow();

public Q_SLOTS:
    void setcurpath(const QString &path);
    QString curpath() const { return m_dir; }

Q_SIGNALS:
    void curpathChanged(const QString &path);
    void reqtransfer(const QStringList &src, const QString &dst);
    void reqtransto(const QStringList &src, karin_FileWindow *self);

private Q_SLOTS:
    void returnPressed_slot();
    void settoptext(const QString &dir);
    void filedblclicked_slot(const QString &path);

private:
    void init();
    
private:
    Ui::karin_FileWindow *ui;
    QString m_dir;
    karin_FileView *m_view;
    int m_dirdrivercount;
};

#endif // KARIN_FILEWINDOW_H
