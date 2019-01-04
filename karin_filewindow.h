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
    void reqtransform(const QStringList &src, const QString &dst);

private Q_SLOTS:
    void returnPressed_slot();
    void settoptext(const QString &dir);

private:
    void init();
    
private:
    Ui::karin_FileWindow *ui;
    QString m_dir;
    karin_FileView *m_view;
};

#endif // KARIN_FILEWINDOW_H
