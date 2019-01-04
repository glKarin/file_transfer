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
    QWidget *source;

    qDebug()<<"drag enter";
    source = event->source();
    if (source) {
        event->setDropAction(source != this ? Qt::CopyAction : Qt::MoveAction);
    }
    else
    {
        event->setDropAction(Qt::CopyAction);
    }
    event->accept();
}

void karin_FileView::dragMoveEvent(QDragMoveEvent *event)
{
    QWidget *source;

    //qDebug()<<"drag move";
    source = event->source();
    if (source) {
        event->setDropAction(source != this ? Qt::CopyAction : Qt::MoveAction);
    }
    else
    {
        event->setDropAction(Qt::CopyAction);
    }
    event->accept();
}

void karin_FileView::dropEvent(QDropEvent *event)
{
    QWidget *source;
    const QMimeData *mime;

    qDebug()<<"drop";
    source = event->source();
    mime = event->mimeData();
    qDebug()<<source;
    if (source) // from other widget
    {
        QString dst = getdropdst(event->pos());
        if(source != this || !dst.isEmpty())
        {
            qDebug()<<mime->text();
            emit reqtransform(QStringList() << mime->text(), dst);
            event->setDropAction(source != this ? Qt::CopyAction : Qt::MoveAction);
            event->accept();
        }
    }
    else
    {
        if(mime->hasFormat("text/uri-list"))
        {
            QList<QUrl> urls = mime->urls();
            if(!urls.isEmpty())
            {
                QStringList r;
                Q_FOREACH(const QUrl &url, urls)
                    r.push_back(url.toLocalFile());
                event->setDropAction(Qt::CopyAction);
                emit reqtransform(r, getdropdst(event->pos()));
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
            handledrag();
    }
    QListView::mouseMoveEvent(event);
}

void karin_FileView::handledrag()
{
    QModelIndex index = currentIndex();
    if (index.isValid()) {
        QMimeData *mimeData = new QMimeData;
        mimeData->setText(m_model->filePath(index));
        qDebug()<<"move "<<m_model->filePath(index);

        QDrag *drag = new QDrag(this);
        drag->setMimeData(mimeData);
        drag->exec();
        //drag->setPixmap(QPixmap(":/images/person.png"));
    }
}

void karin_FileView::setpath(const QString &path)
{
    m_model->setPath(path);
}

QString karin_FileView::getdropdst(const QPoint &pos)
{
    QModelIndex index = indexAt(pos);
    if(index.isValid())
        return m_model->filePath(index);
    return m_model->path();
}
