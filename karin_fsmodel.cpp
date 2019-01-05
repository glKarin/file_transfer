#include "karin_fsmodel.h"
#include "karin_std.h"

#include <QVariant>
#include <QDir>
#include <QFileInfo>
#include <QFileSystemWatcher>
#include <QSize>
#include <QMimeData>
#include <QApplication>
#include <QIcon>
#include <QDebug>

karin_FSModel::karin_FSModel(QObject *parent) :
    QAbstractListModel(parent),
    m_watcher(new QFileSystemWatcher(this)),
  m_path(QDir::homePath()),
  m_showHidden(false)
{
    init();
    connect(m_watcher, SIGNAL(directoryChanged(const QString &)), this, SLOT(directoryChanged_slot(const QString &)));
}

karin_FSModel::~karin_FSModel()
{
}

void karin_FSModel::init()
{
    m_filelist.clear();
    static const QDir::SortFlags flags = QDir::DirsFirst | QDir::Name | QDir::IgnoreCase;
    static const QDir::Filters qfilters = QDir::Files | QDir::AllDirs | QDir::Drives |  QDir::NoDot;
    QDir::Filters f = qfilters;
    if(m_showHidden)
        f |= QDir::Hidden;
    QFileInfoList list = QDir(m_path).entryInfoList(m_filters, f, flags);
    for(QFileInfoList::const_iterator itor = list.begin();
            itor != list.end();
            ++itor)
    {
#ifndef Q_OS_WIN
        if(m_path == "/" && itor->fileName() == "..")
            continue;
#endif
        file_info_s info;
        info.name = itor->isDir() ? itor->fileName() + "/" : itor->fileName();
        info.path = itor->absoluteFilePath();
        info.owner = itor->owner();
        info.group = itor->group();
        info.size = fformats(itor->size(), itor->isDir());
        info.permission = fformatp(itor->permissions());
        info.type = itor->isDir();
        m_filelist.push_back(info);
    }
    emit filelistChanged();
}

void karin_FSModel::setPath(const QString &path)
{
    QString cleanpath = QDir::cleanPath(path);
    if(m_path != cleanpath)
    {
        QFileInfo info(cleanpath);
        while(!info.isDir())
        {
            QString s = info.absolutePath();
            if(s.isEmpty())
                s = "/";
            info.setFile(s);
        }
        m_path = info.absoluteFilePath();
        QStringList list = m_watcher->directories();
        if(list.size() > 0)
            m_watcher->removePaths(list);
        m_watcher->addPath(m_path);
        init();
        emit pathChanged(m_path);
        emit layoutChanged();
    }
}

void karin_FSModel::setFilters(const QStringList &list)
{
    if(m_filters != list)
    {
        m_filters = list;
        init();
    }
}

void karin_FSModel::setShowHidden(bool b)
{
    if(m_showHidden != b)
    {
        m_showHidden = b;
        init();
    }
}

void karin_FSModel::directoryChanged_slot(const QString &path)
{
    if(path == m_path)
    {
        m_path.clear();
        setPath(path);
    }
}

QVariant karin_FSModel::data(const QModelIndex &index, int role) const
{
    static const QString Fmt_StatusTip("%1 %2:%3 %4 (%5)");
    static const QString Fmt_ToopTip("%1 %2:%3 %4");
    QVariant data;
    file_info_s info;
    int i;

    if(!index.isValid())
        return data;

    i = index.row();
    if(i > m_filelist.size())
        return data;

    info = m_filelist.at(i);

    switch(role)
    {
    case Qt::DisplayRole:
        data.setValue(info.name);
        break;
    case Qt::SizeHintRole:
        data.setValue(QSize(FILE_GRID_WIDTH, FILE_GRID_HEIGHT));
        break;
    case Qt::DecorationRole:
        data.setValue(info.type ? QIcon(":/resc/image/icon-l-folder-empty.png") : QIcon(":/resc/image/icon-l-notes.png"));
        break;
    case Qt::ToolTipRole:
        //data.setValue(info.path);
        data.setValue(Fmt_ToopTip.arg(info.path).arg(info.owner).arg(info.group).arg(info.permission) + (info.type ? "" : QString(" (%1)").arg(info.size)));
        break;
    case Qt::StatusTipRole:
        data.setValue(Fmt_StatusTip.arg(info.path).arg(info.owner).arg(info.group).arg(info.permission).arg(info.type ? tr("directory") : info.size));
        break;

    case Qt::TextAlignmentRole:
        data.setValue(static_cast<int>(Qt::AlignLeft | Qt::AlignVCenter));
        break;
    default:
        break;
    }
    return QVariant(data);
}

int karin_FSModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_filelist.size();
}

bool karin_FSModel::isDir(const QModelIndex &index) const
{
    int i;

    if(index.isValid())
    {
        i = index.row();
        return m_filelist.at(i).type;
    }
    return false;
}

QString karin_FSModel::filePath(const QModelIndex &index) const
{
    int i;

    if(index.isValid())
    {
        i = index.row();
        return m_filelist.at(i).path;
    }
    return QString::null;
}

Qt::DropActions karin_FSModel::supportedDropActions() const
{
    return Qt::CopyAction | Qt::MoveAction;
}

Qt::ItemFlags karin_FSModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags defaultFlags = QAbstractListModel::flags(index);

    if (index.isValid())
        return Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled | defaultFlags;
    else
        return Qt::ItemIsDropEnabled | defaultFlags;
}

QStringList karin_FSModel::mimeTypes() const
{
    QStringList types;
    types << "application/vnd.text.list";
    return types;
}

QMimeData *karin_FSModel::mimeData(const QModelIndexList &indexes) const
{
    QMimeData *mimeData;
    QByteArray encodedData;
    int i;

    mimeData = new QMimeData();
    QDataStream stream(&encodedData, QIODevice::WriteOnly);

    foreach (const QModelIndex &index, indexes) {
        if (index.isValid()) {
            i = index.row();
            QString text = m_filelist.at(i).path;
            stream << text;
        }
    }

    mimeData->setData("application/vnd.text.list", encodedData);
    return mimeData;
}

bool karin_FSModel::dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent)
{
    qApp->quit();
    qDebug()<<action<<row<<column<<Qt::IgnoreAction;
    return 1;
    if (action == Qt::IgnoreAction)
        return true;

    if (!data->hasFormat("application/vnd.text.list"))
        return false;

    if (column > 0)
        return false;

    int beginRow;

    if (row != -1)
        beginRow = row;
    else if (parent.isValid())
        beginRow = parent.row();
    else
        beginRow = rowCount(QModelIndex());


    QByteArray encodedData = data->data("application/vnd.text.list");
    QDataStream stream(&encodedData, QIODevice::ReadOnly);
    QStringList newItems;
    int rows = 0;


    while (!stream.atEnd()) {
        QString text;
        stream >> text;
        newItems << text;
        ++rows;
    }


    return false;
    insertRows(beginRow, rows, QModelIndex());
    foreach (const QString &text, newItems) {
        QModelIndex idx = index(beginRow, 0, QModelIndex());
        setData(idx, text);
        beginRow++;
    }

    return true;
}
