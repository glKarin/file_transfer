#include "karin_fileview.h"
#include "karin_fsmodel.h"
#include "karin_std.h"

#include <QApplication>
#include <QUrl>
#include <QDebug>

karin_FileView::karin_FileView(QWidget *parent) :
    QListView(parent),
    m_model(new karin_FSModel(this))
{
    init();
    connect(this, SIGNAL(doubleClicked(const QModelIndex &)), this, SLOT(doubleClicked_slot(const QModelIndex &)));
    connect(m_model, SIGNAL(pathChanged(const QString &)), this, SIGNAL(pathChanged(const QString &)));
}

karin_FileView::~karin_FileView()
{

}


void karin_FileView::doubleClicked_slot(const QModelIndex &index)
{
    if(m_model->isDir(index))
    {
        m_model->setPath(m_model->filePath(index));
    }
}

void karin_FileView::init()
{
    setModel(m_model);
    setWrapping(true);
    setFlow(QListView::LeftToRight);
    setGridSize(QSize(FILE_GRID_WIDTH, FILE_GRID_HEIGHT));
    setDragEnabled(true);
    setDragDropMode(QAbstractItemView::InternalMove);
    setAcceptDrops(true);
    setDropIndicatorShown(true);
    //setSelectionMode(QAbstractItemView::ExtendedSelection);
}

void karin_FileView::dragEnterEvent(QDragEnterEvent *event)
{
    //qDebug()<<"drag enter" << event->source<<this;
    event->setDropAction(Qt::CopyAction);
    event->accept();
}

void karin_FileView::dragMoveEvent(QDragMoveEvent *event)
{
    //qDebug()<<"drag move" << event->source<<this;
    event->accept();
}

void karin_FileView::dropEvent(QDropEvent *event)
{
    QWidget *source;
    const QMimeData *mime;

    source = event->source();
    mime = event->mimeData();
    //qDebug()<<source;
    if (source) // from other widget
    {
        QString dst = getdropdst(event->pos());
        if(mime->text() != dst)
        {
            QStringList urls;
            urls << mime->text();
            qDebug() << "[Drop]: From other FileView urls: " << urls << " -> " << dst;
            emit reqtransform(urls, dst);
            event->setDropAction(source != this ? Qt::CopyAction : Qt::MoveAction);
            event->accept();
        }
        else
            qWarning() << "[Drop]: can not move to self";
    }
    else // from external
    {
        if(mime->hasFormat("text/uri-list"))
        {
            QString dst = getdropdst(event->pos());
            QList<QUrl> urls = mime->urls();
            qDebug() << "[Drop]: From external urls: " << urls << " -> " << dst;
            if(!urls.isEmpty())
            {
                QStringList r;
                Q_FOREACH(const QUrl &url, urls)
                    r.push_back(url.toLocalFile());
                event->setDropAction(Qt::CopyAction);
                emit reqtransform(r, dst);
            }
            event->accept();
        }
    }
}

void karin_FileView::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
        m_startpos = event->pos();
    QListView::mousePressEvent(event);
}

void karin_FileView::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton) {
        int distance = (event->pos() - m_startpos).manhattanLength();
        if (distance >= QApplication::startDragDistance())
            handledrag(event->pos());
    }
    QListView::mouseMoveEvent(event);
}

bool karin_FileView::handledrag(const QPoint &pos)
{
    bool is_cur;
    QMimeData *mimeData;
    QDrag *drag;

    is_cur = hastarget(pos);
    if(!is_cur)
    {
        //qWarning() << "[MouseMove]: mouse click position has not a valid target.";
        return false;
    }

    QModelIndex index = currentIndex();
    if (index.isValid()) {
        QString target = m_model->filePath(index);
        mimeData = new QMimeData;
        mimeData->setText(target);

        qDebug() << "[MouseMove]: target path -> " << target;

        drag = new QDrag(this);
        drag->setMimeData(mimeData);
        drag->exec();
        //drag->setPixmap(QPixmap(":/images/person.png"));
        return true;
    }

    return false;
}

void karin_FileView::setpath(const QString &path)
{
    m_model->setPath(path);
}

QString karin_FileView::getdropdst(const QPoint &pos, bool *cur) const
{
    bool b;

    QModelIndex index = indexAt(pos);
    b = index.isValid();
    if(cur)
        *cur = b;

    if(b)
        return m_model->filePath(index);
    return m_model->path();
}

bool karin_FileView::getdropdst(QString &r, const QPoint &pos, bool cur) const
{
    bool b;

    QModelIndex index = indexAt(pos);
    b = index.isValid();

    if(cur && !b)
        return false;

    if(b)
        r = m_model->filePath(index);
    r = m_model->path();
    return true;
}

bool karin_FileView::hastarget(const QPoint &pos) const
{
    return indexAt(pos).isValid();
}
