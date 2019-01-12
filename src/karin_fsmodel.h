#ifndef KARIN_FSMODEL_H
#define KARIN_FSMODEL_H

#include <QAbstractListModel>
#include <QVector>
#include <QFile>
#include <QStringList>

class QFileSystemWatcher;

struct file_info_s
{
    QString path;
    QString name;
    QString owner;
    QString group;
    QString permission;
    QString size;
    bool type;
};

class karin_FSModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit karin_FSModel(QObject *parent = 0);
    virtual ~karin_FSModel();
    inline QString path() const
    {
        return m_path;
    }
    void setPath(const QString &path);
    void setFilters(const QStringList &list);
    inline QStringList filters() const
    {
        return m_filters;
    }
    void setShowHidden(bool b);
    inline bool showHidden() const
    {
        return m_showHidden;
    }
    inline const QVector<file_info_s> * filelist() const
    {
        return &m_filelist;
    }

    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

    bool isDir(const QModelIndex &index) const;
    QString filePath(const QModelIndex &index) const;
    virtual Qt::DropActions supportedDropActions() const;
    virtual Qt::ItemFlags flags(const QModelIndex &index) const;
    virtual QStringList mimeTypes() const;
    virtual QMimeData * mimeData(const QModelIndexList &indexes) const;
    virtual bool dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent);

Q_SIGNALS:
    void filelistChanged();
    void pathChanged(const QString &path);
    
public slots:

private:
    void reset();
    void init();

    private Q_SLOTS:
        void directoryChanged_slot(const QString &);

private:
    QFileSystemWatcher *m_watcher;
    QString m_path;
    QVector<file_info_s> m_filelist;
    QStringList m_filters;
    bool m_showHidden;
    
};
#endif
