#ifndef KARIN_FILEVIEW_H
#define KARIN_FILEVIEW_H

#include <QListView>
#include <QDragEnterEvent>
#include <QDragMoveEvent>
#include <QDropEvent>

class karin_FSModel;

class karin_FileView : public QListView
{
    Q_OBJECT
public:
    explicit karin_FileView(QWidget *parent = 0);
    virtual ~karin_FileView();
    
signals:
    void filedblclicked(const QString &path);
    void pathChanged(const QString &path);
    void reqtransform(const QStringList &src, const QString &dst);
    
public slots:
    void setpath(const QString &path);

protected:
    virtual void dragEnterEvent(QDragEnterEvent *event);
    virtual void dragMoveEvent(QDragMoveEvent *event);
    virtual void dropEvent(QDropEvent *event);
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);

private Q_SLOTS:
    void doubleClicked_slot(const QModelIndex &index);

private:
    void init();
    void handledrag();
    QString getdropdst(const QPoint &pos);

private:
    karin_FSModel *m_model;
    QPoint m_startpos;
};

#endif // KARIN_FILEVIEW_H
