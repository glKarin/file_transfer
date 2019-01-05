#include "karin_fileengine.h"
#include "karin_std.h"

#include <QMutex>
#include <QMutexLocker>
#include <QFileInfo>
#include <QDateTime>
#include <QDebug>


karin_FileEngine::karin_FileEngine(QObject *parent)
    : QObject(parent),
      m_state(karin_FileEngine::FileEngine_Ready),
      m_mutex(new QMutex),
      m_size(0),
      m_dirc(0),
      m_filec(0)
{
}

karin_FileEngine::~karin_FileEngine()
{
}


int karin_FileEngine::scan()
{
    int i;
    karin_FileScanner *scanner;
    struct file_porcess_s proc;

    if(m_state != karin_FileEngine::FileEngine_Prepare)
        return 0;

    i = 0;
    qDebug()<<"aaa"<<thread()->currentThreadId();
    Q_FOREACH(const QString &str, m_src)
    {
        scanner = new karin_FileScanner(i, this);
        scanner->setd(str);
        m_scanners.insert(i, scanner);
        connect(scanner, SIGNAL(handlefinished(ftid_t, bool)), this, SLOT(scanningfinished_slot(ftid_t, bool)));
        connect(scanner, SIGNAL(scanning(ftid_t, const QString &, quint32, quint32, quint64)), this, SLOT(scanning_slot(ftid_t, const QString &, quint32, quint32, quint64)));

        struct file_info_s info;
        info.id = i;
        proc.infos.insert(i, info);

        i++;
    }

    sets(karin_FileEngine::FileEngine_Scanning);
    proc.start_unix_timestamp = QDateTime::currentMSecsSinceEpoch();
    proc.status = PROCESS_STATUS_RUNNING;
    proc.thread = (FileThreadList_t *)(&m_scanners);
    Q_FOREACH(karin_FileScanner *s, m_scanners)
        s->start();
    m_process.insert(FileEngine_S, proc);
}

int karin_FileEngine::scan(const QStringList &dirs, const QString &dst)
{
    int c;

    if(!prepare(dirs, dst))
        return 0;

    return scan();
}

bool karin_FileEngine::prepare(const QStringList &dirs, const QString &dst)
{
    bool r;

    if(m_state != karin_FileEngine::FileEngine_Ready)
        return false;

    r = false;
    QFileInfo info(dst);
    if(dirs.isEmpty())
    {
        emit errocc(karin_FileEngine::FileEngine_SrcFilesIsEmpty);
        goto __Exit;
    }

    if(info.exists())
    {
        if(!info.isDir())
            emit errocc(karin_FileEngine::FileEngine_DstDirIsNotADir);
        else if(!info.isReadable())
            emit errocc(karin_FileEngine::FileEngine_DstDirIsNotBeReadable);
        else
            r = true;
        goto __Exit;
    }
    else
        r = true;

__Exit:
    if(r)
    {
        deinit();
        m_dst = dst;
        m_src = dirs;
        sets(karin_FileEngine::FileEngine_Prepare);
    }

    return r;
}

void karin_FileEngine::sets(FileEngine_State_e s)
{
    if(m_state != s)
    {
        m_state = s;
        emit stateChanged(m_state);
    }
}

void karin_FileEngine::deinit()
{
    m_dirs.clear();
    m_files.clear();
    m_process.clear();
}

int karin_FileEngine::mkdirs()
{
    karin_FileDirMaker *dirmker;
    struct file_porcess_s proc;

    if(m_state != karin_FileEngine::FileEngine_Scanned)
        return 0;

    qDebug()<<"mkdir";
    dirmker = new karin_FileDirMaker(0, this);
    dirmker->seteng(this);
    m_dirmkers.insert(0, dirmker);
    connect(dirmker, SIGNAL(handlefinished(ftid_t, bool)), this, SLOT(mkingdirfinished_slot(ftid_t, bool)));
    connect(dirmker, SIGNAL(mkingdir(ftid_t, const QString &, bool, quint32)), this, SLOT(mkingdir_slot(ftid_t, const QString &, bool, quint32)));

    struct file_info_s info;
    info.id = 0;
    proc.infos.insert(0, info);

    sets(karin_FileEngine::FileEngine_Mkdir);
    proc.start_unix_timestamp = QDateTime::currentMSecsSinceEpoch();
    dirmker->start();
    proc.status = PROCESS_STATUS_RUNNING;
    m_process.insert(FileEngine_M, proc);
}

int karin_FileEngine::trans()
{
    karin_FileTransfer *transfer;
    struct file_porcess_s proc;
    int i;
    if(m_state != karin_FileEngine::FileEngine_Mkdir)
        return 0;

    qDebug()<<"transfer";
    i = 0;
    for(FileList_t::const_iterator itor = m_files.begin();
        itor != m_files.end(); ++itor)
    {
        transfer = new karin_FileTransfer(i, this);
        transfer->seteng2(this, itor.key(), 0, itor.value().size());
        m_transfers.insert(i, transfer);
        connect(transfer, SIGNAL(handlefinished(ftid_t, bool)), this, SLOT(transferfinished_slot(ftid_t, bool)));
        connect(transfer, SIGNAL(transfering(ftid_t, const QString &, bool, quint32, quint64)), this, SLOT(transfer_slot(ftid_t, const QString &, bool, quint32, quint64)));

        struct file_info_s info;
        info.id = i;
        proc.infos.insert(i, info);

        i++;
    }

    sets(karin_FileEngine::FileEngine_Trans);
    proc.start_unix_timestamp = QDateTime::currentMSecsSinceEpoch();
    proc.status = PROCESS_STATUS_RUNNING;
    Q_FOREACH(karin_FileTransfer *s, m_transfers)
        s->start();
    m_process.insert(FileEngine_T, proc);
}

QString karin_FileEngine::statestr() const
{
    static QString State_Str[] = {
        tr("Ready"),
        tr("Prepare"),
        tr("Scanning files and directories"),
        tr("Scanning finished"),
        tr("Create directories"),
        tr("Copy files to dest"),
        tr("Operation done")
    };
    return State_Str[static_cast<int>(m_state)];
}

void karin_FileEngine::cancel()
{
    int p;

    p = -1;
    switch(m_state)
    {
    case FileEngine_Scanning:
        p = static_cast<FileEngine_Process_e>(karin_FileEngine::FileEngine_S);
        break;
    case FileEngine_Mkdir:
        p = static_cast<FileEngine_Process_e>(karin_FileEngine::FileEngine_M);
        break;
    case FileEngine_Trans:
        p = static_cast<FileEngine_Process_e>(karin_FileEngine::FileEngine_T);
        break;
    case FileEngine_Checking:
        p = static_cast<FileEngine_Process_e>(karin_FileEngine::FileEngine_C);
        break;
    case FileEngine_Transed:
    case FileEngine_Done:
    case FileEngine_Scanned:
    case FileEngine_Ready:
    case FileEngine_Prepare:
    default:
        break;
    }

    if(p != -1)
    {
        struct file_porcess_s &proc = m_process[p];
        for(FileThreadList_t::iterator itor = proc.thread->begin();
            itor != proc.thread->end(); ++itor)
        {
            itor.value()->pause();
        }
    }
}

void karin_FileEngine::pause()
{

}

// scanning
void karin_FileEngine::scanningfinished_slot(ftid_t id, bool suc)
{
    static const QString Fmt = tr("Scanning finished: find %1 files, %2 dirs, total size: %3(%4 bytes), using %5");
    QMutexLocker locker(m_mutex);
    Q_UNUSED(locker);

    karin_FileScanner *scanner;
    quint32 filec;

    if(m_state != karin_FileEngine::FileEngine_Scanning)
        return;

    scanner = m_scanners.take(id);
    struct file_porcess_s &proc = m_process[karin_FileEngine::FileEngine_S];

    if(scanner)
    {
        m_size += scanner->m_size;
        m_files.insert(scanner->m_dirstr, scanner->m_files);
        m_dirs += scanner->m_dirs;
        proc.infos[id].file_count = scanner->m_files.size();
        proc.infos[id].dir_count = scanner->m_dirs.size();
        proc.infos[id].size = scanner->m_size;
        scanner->deleteLater();
        if(!suc)
            emit errocc(karin_FileEngine::FileEngine_ReadDirFail);
    }

    if(m_scanners.isEmpty())
    {
        filec = 0;
        proc.status = PROCESS_STATUS_SUC;
        proc.end_unix_timestamp = QDateTime::currentMSecsSinceEpoch();
        for(FileList_t::const_iterator itor = m_files.begin();
            itor != m_files.end(); ++itor)
            filec += itor.value().size();
        emit updating(100, Fmt.arg(filec).arg(m_dirs.size()).arg(fformats(m_size)).arg(m_size).arg(formatuts(proc.end_unix_timestamp - proc.start_unix_timestamp)));
        sets(karin_FileEngine::FileEngine_Scanned);
    }
}

void karin_FileEngine::scanning_slot(ftid_t id, const QString &dir, quint32 fcount, quint32 dcount, quint64 fsize)
{
    static const QString Fmt = tr("Scanning files: %1, dirs %2, size: %3(%4 bytes), using %5 -> %6");
    QMutexLocker locker(m_mutex);
    Q_UNUSED(locker);

    quint64 sizec;
    quint32 filec;
    quint32 dirc;

    if(m_state != karin_FileEngine::FileEngine_Scanning)
        return;

    struct file_porcess_s &proc = m_process[karin_FileEngine::FileEngine_S];
    sizec = 0;
    filec = 0;
    dirc = 0;
    struct file_info_s &s = proc.infos[id];

    if(s.id != FT_INVALID_ID)
    {
        s.file_count = fcount;
        s.dir_count = dcount;
        s.size = fsize;
    }

    Q_FOREACH(const struct file_info_s &s, proc.infos)
    {
        sizec += s.size;
        filec += s.file_count;
        dirc += s.dir_count;
    }
    proc.cur_unix_timestamp = QDateTime::currentMSecsSinceEpoch();

    emit updating(-1, Fmt.arg(filec).arg(dirc).arg(fformats(sizec)).arg(sizec).arg(formatuts(proc.cur_unix_timestamp - proc.start_unix_timestamp)).arg(dir));
}

// mkdir
void karin_FileEngine::mkingdir_slot(ftid_t id, const QString &dir, bool suc, quint32 count)
{
    static const QString Fmt = tr("make directory: %1(%2) -> %3, using %4");
    QMutexLocker locker(m_mutex);
    Q_UNUSED(locker);

    quint32 dirc;
    int per;

    if(m_state != karin_FileEngine::FileEngine_Mkdir)
        return;

    struct file_porcess_s &proc = m_process[karin_FileEngine::FileEngine_M];
    dirc = 0;
    struct file_info_s &s = proc.infos[id];

    if(s.id != FT_INVALID_ID)
    {
        s.dir_count = count;
    }

    Q_FOREACH(const struct file_info_s &s, proc.infos)
    {
        dirc += s.dir_count;
    }

    per = (int)((qreal)dirc / (qreal)m_dirs.size() * 100);
    proc.cur_unix_timestamp = QDateTime::currentMSecsSinceEpoch();
    emit updating(per, Fmt.arg(dirc).arg(suc ? tr("success") : tr("fail")).arg(dir).arg(formatuts(proc.cur_unix_timestamp - proc.start_unix_timestamp)));
}

void karin_FileEngine::mkingdirfinished_slot(ftid_t id, bool suc)
{
    static const QString Fmt = tr("Make directorys finished: %1 success, %2 fail, using %3");
    QMutexLocker locker(m_mutex);
    Q_UNUSED(locker);

    quint32 dirc;
    karin_FileDirMaker *dirmker;

    if(m_state != karin_FileEngine::FileEngine_Mkdir)
        return;

    struct file_porcess_s &proc = m_process[karin_FileEngine::FileEngine_M];
    dirmker = m_dirmkers.take(id);

    if(dirmker)
    {
        struct file_info_s &s = proc.infos[id];
        s.dir_count = dirmker->m_countc;
        dirmker->deleteLater();
        if(!suc)
            emit errocc(karin_FileEngine::FileEngine_MakeDirFail);
    }

    if(m_dirmkers.isEmpty())
    {
        dirc = 0;
        Q_FOREACH(const struct file_info_s &s, proc.infos)
        {
            dirc += s.dir_count;
        }
        proc.end_unix_timestamp = QDateTime::currentMSecsSinceEpoch();
        emit updating(100, Fmt.arg(dirc).arg(m_dirs.size() - dirc).arg(formatuts(proc.end_unix_timestamp - proc.start_unix_timestamp)));

        trans();
    }
}

// transfer
void karin_FileEngine::transfer_slot(ftid_t id, QString file, bool suc, quint32 c, quint64 size)
{
    static const QString Fmt = tr("transfer file: %1, count: %2, size: %3(%4 bytes), using %5 -> %6");
    QMutexLocker locker(m_mutex);
    Q_UNUSED(locker);

    quint64 sizec;
    quint32 filec;
    int per;

    if(m_state != karin_FileEngine::FileEngine_Trans)
        return;

    struct file_porcess_s &proc = m_process[karin_FileEngine::FileEngine_T];
    sizec = 0;
    filec = 0;
    struct file_info_s &s = proc.infos[id];

    if(s.id != FT_INVALID_ID)
    {
        s.file_count = size;
        s.size = c;
    }

    Q_FOREACH(const struct file_info_s &s, proc.infos)
    {
        sizec += s.size;
        filec += s.file_count;
    }
    proc.cur_unix_timestamp = QDateTime::currentMSecsSinceEpoch();
    per = (int)((qreal)sizec / (qreal)m_dirs.size() * 100);

    emit updating(per, Fmt.arg(file).arg(filec).arg(fformats(sizec)).arg(sizec).arg(formatuts(proc.cur_unix_timestamp - proc.start_unix_timestamp)).arg(suc ? tr("success") : tr("fail")));
}

void karin_FileEngine::transferfinished_slot(ftid_t id, bool suc)
{
    qDebug()<<"finisheddddddddddddddddddddddddddddd";
}

// checkmd5
