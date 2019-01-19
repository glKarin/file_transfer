#include "karin_fileengine.h"
#include "karin_std.h"
#include "karin_ut.h"

#include <QMutex>
#include <QMutexLocker>
#include <QFileInfo>
#include <QDateTime>
#include <QDir>
#include <QDebug>

const QString Task_Names[] = {
    QObject::tr("Scan"),
    QObject::tr("Mkdir"),
    QObject::tr("Transfer"),
    QObject::tr("Check"),
};

const QString Task_State[] = {
    QObject::tr("ready"),
    QObject::tr("start"),
    QObject::tr("pause"),
    QObject::tr("doing"),
    QObject::tr("success"),
    QObject::tr("fail"),
};

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

// 准备
bool karin_FileEngine::prepare(const QStringList &dirs, const QString &dst)
{
    bool r;

    if(m_state != karin_FileEngine::FileEngine_Ready)
        return false;

    m_filec = 0;
    m_dirc = 0;
    m_size = 0;

    qDebug() << "[_FileEngine]: prepare...";
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

int karin_FileEngine::transfer(const QStringList &dirs, const QString &dst)
{
    if(!prepare(dirs, dst))
        return 0;

    return scan();
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

QString karin_FileEngine::statestr() const
{
    static QString State_Str[] = {
        tr("Ready"),
        tr("Prepare"),
        tr("Scanning files and directories"),
        tr("Scanning finished"),
        tr("Create directories"),
        tr("Copy files to dest"),
        tr("Copy files finished"),
        tr("MD5 checking"),
        tr("Operation done"),
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

// 扫描文件
int karin_FileEngine::scan()
{
    static const QString Fmt = tr("Scan %1: %2");
    int i;
    karin_FileScanner *scanner;
    struct file_porcess_s proc;

    if(m_state != karin_FileEngine::FileEngine_Prepare)
        return 0;

    i = 0;
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

    qDebug() << QString("[_FileEngine]: scan...(%1)").arg(i);
    sets(karin_FileEngine::FileEngine_Scanning);
    proc.start_unix_timestamp = QDateTime::currentMSecsSinceEpoch();
    proc.status = PROCESS_STATUS_RUNNING;
    proc.thread = (FileThreadList_t *)(&m_scanners);
    Q_FOREACH(karin_FileScanner *s, m_scanners)
    {
        s->start();
        emit updating(0, Fmt.arg(s->m_id).arg(s->m_dirstr),
                      karin_FileEngine::FileEngine_S, s->m_dirstr, karin_FileEngine::FileEngine_Task_Start, 0);
    }
    m_process.insert(FileEngine_S, proc);

    return i;
}

void karin_FileEngine::scanning_slot(ftid_t id, const QString &dir, quint32 fcount, quint32 dcount, quint64 fsize)
{
    static const QString Fmt = tr("Scanning: %1 files, %2 dirs <br/> %3 size: %4(%5 bytes)");
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

    emit updating(-1, Fmt.arg(filec).arg(dirc).arg(dir).arg(fformats(sizec)).arg(sizec),
                  -1, QString::null, -1, proc.cur_unix_timestamp - proc.start_unix_timestamp);
}

void karin_FileEngine::scanningfinished_slot(ftid_t id, bool suc)
{
    static const QString Fmt = tr("Find %1 files, %2 dirs <br/> Total size: %3(%4 bytes)");
    QMutexLocker locker(m_mutex);
    Q_UNUSED(locker);

    karin_FileScanner *scanner;

    if(m_state != karin_FileEngine::FileEngine_Scanning)
        return;

    scanner = m_scanners.take(id);
    struct file_porcess_s &proc = m_process[karin_FileEngine::FileEngine_S];

    if(scanner)
    {
        m_size += scanner->m_size;
        m_files.insert(scanner->m_dirstr, scanner->m_files);
        m_dirs += scanner->m_dirs;
        m_filec += scanner->m_files.size();
        proc.infos[id].file_count = scanner->m_files.size();
        proc.infos[id].dir_count = scanner->m_dirs.size();
        proc.infos[id].size = scanner->m_size;
        scanner->deleteLater();
        if(!suc)
            emit errocc(karin_FileEngine::FileEngine_ReadDirFail);
    }

    if(m_scanners.isEmpty())
    {
        m_filec = 0;
        proc.status = PROCESS_STATUS_SUC;
        proc.end_unix_timestamp = QDateTime::currentMSecsSinceEpoch();
        for(FileList_t::const_iterator itor = m_files.begin();
            itor != m_files.end(); ++itor)
            m_filec += itor.value().size();
        emit updating(100, Fmt.arg(m_filec).arg(m_dirs.size()).arg(fformats(m_size)).arg(m_size),
                      karin_FileEngine::FileEngine_S, tr("files"), karin_FileEngine::FileEngine_Task_Success, proc.end_unix_timestamp - proc.start_unix_timestamp);
        sets(karin_FileEngine::FileEngine_Scanned);
    }
}

// mkdir
int karin_FileEngine::mkdirs()
{
    static const QString Fmt = tr("Mkdir %1: count : %2");
    karin_FileDirMaker *dirmker;
    struct file_porcess_s proc;

    if(m_state != karin_FileEngine::FileEngine_Scanned)
        return 0;

    qDebug() << "[_FileEngine]: mkdir...(1)";
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
    emit updating(0, Fmt.arg(dirmker->m_id).arg(dirmker->m_dirs.size()),
                  karin_FileEngine::FileEngine_M, dirmker->m_dir->absolutePath(), karin_FileEngine::FileEngine_Task_Start, 0);
    proc.status = PROCESS_STATUS_RUNNING;
    m_process.insert(FileEngine_M, proc);

    return 1;
}

void karin_FileEngine::mkingdir_slot(ftid_t id, const QString &dir, bool suc, quint32 count)
{
    static const QString Fmt = tr("Make directory count: %1");
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
    emit updating(per, Fmt.arg(dirc),
                  karin_FileEngine::FileEngine_M, dir, suc ? karin_FileEngine::FileEngine_Task_Success : karin_FileEngine::FileEngine_Task_Fail, proc.cur_unix_timestamp - proc.start_unix_timestamp);
}

void karin_FileEngine::mkingdirfinished_slot(ftid_t id, bool suc)
{
    static const QString Fmt = tr("Total make directorys %1 success, %2 fail");
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
        emit updating(100, Fmt.arg(dirc).arg(m_dirs.size() - dirc),
                      -1, QString::null, -1, proc.end_unix_timestamp - proc.start_unix_timestamp);

        trans();
    }
}

// 传输
int karin_FileEngine::trans()
{
    static const QString Fmt = tr("Transfer %1: count : %2");
    karin_FileTransfer *transfer;
    struct file_porcess_s proc;
    int i;
    int m;
    int size;
    int thread_count;
    int part;
    if(m_state != karin_FileEngine::FileEngine_Mkdir)
        return -1;

    thread_count = karin_UT::Instance()->getsetting<int>(SETTING_MAX_WORKING_THREAD);
    if(thread_count <= 0 || thread_count > FILE_ENGINE_MAX_WORKING_THREAD)
        thread_count = 1;
    part = m_filec / thread_count + (m_filec % thread_count ? 1 : 0);

    i = 0;
    for(FileList_t::const_iterator itor = m_files.begin();
        itor != m_files.end(); ++itor)
    {
        m = 0;
        while(m < itor.value().size())
        {
            size = qMin(part, itor.value().size() - m);
            transfer = new karin_FileTransfer(i, this);
            transfer->seteng2(this, itor.key(), m, size);
            m_transfers.insert(i, transfer);
            connect(transfer, SIGNAL(handlefinished(ftid_t, bool)), this, SLOT(transferfinished_slot(ftid_t, bool)));
            connect(transfer, SIGNAL(transfering(ftid_t, const QString &, bool, quint32, quint64)), this, SLOT(transfer_slot(ftid_t, const QString &, bool, quint32, quint64)));

            struct file_info_s info;
            info.id = i;
            proc.infos.insert(i, info);

            i++;
            m += size;
        }
    }

    qDebug() << QString("[_FileEngine]: transfer...(%1)").arg(i);

    sets(karin_FileEngine::FileEngine_Trans);
    proc.start_unix_timestamp = QDateTime::currentMSecsSinceEpoch();
    proc.status = PROCESS_STATUS_RUNNING;
    Q_FOREACH(karin_FileTransfer *s, m_transfers)
    {
        s->start();
        emit updating(0, Fmt.arg(s->m_id).arg(s->m_files.size()),
                      karin_FileEngine::FileEngine_T, s->m_dstdir->absolutePath(), karin_FileEngine::FileEngine_Task_Start, 0);
    }
    m_process.insert(FileEngine_T, proc);

    return i;
}

void karin_FileEngine::transfer_slot(ftid_t id, const QString &file, bool suc, quint32 c, quint64 size)
{
    static const QString Fmt = tr("Transfer count: %1 <br/> Size: %2(%3 bytes)");
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
        s.file_count = c;
        s.size = size;
    }

    Q_FOREACH(const struct file_info_s &s, proc.infos)
    {
        sizec += s.size;
        filec += s.file_count;
    }
    proc.cur_unix_timestamp = QDateTime::currentMSecsSinceEpoch();
    per = (int)((qreal)filec / (qreal)m_filec * 100);

    emit updating(per, Fmt.arg(filec).arg(fformats(sizec)).arg(sizec),
                  karin_FileEngine::FileEngine_T, file, suc ? karin_FileEngine::FileEngine_Task_Success : karin_FileEngine::FileEngine_Task_Fail, proc.cur_unix_timestamp - proc.start_unix_timestamp);
}

void karin_FileEngine::transferfinished_slot(ftid_t id, bool suc)
{
    static const QString Fmt = tr("Total transfer: %1 success, %2 fail <br/> Total size: %3(%4 bytes)");
    QMutexLocker locker(m_mutex);
    Q_UNUSED(locker);

    karin_FileTransfer *transfer;
    quint32 succ, failc;
    quint64 size;

    if(m_state != karin_FileEngine::FileEngine_Trans)
        return;

    transfer = m_transfers.take(id);
    struct file_porcess_s &proc = m_process[karin_FileEngine::FileEngine_T];

    if(transfer)
    {
        proc.infos[id].file_count = transfer->m_countc;
        proc.infos[id].dir_count = transfer->m_fails.size();
        proc.infos[id].size = transfer->m_sizec;
        transfer->deleteLater();
        if(!suc)
            emit errocc(karin_FileEngine::FileEngine_TransferFail);
    }

    if(m_transfers.isEmpty())
    {
        succ = 0;
        failc = 0;
        size = 0;
        proc.status = PROCESS_STATUS_SUC;
        proc.end_unix_timestamp = QDateTime::currentMSecsSinceEpoch();
        for(FileInfoList_t::const_iterator itor = proc.infos.begin();
            itor != proc.infos.end(); ++itor)
        {
            succ += itor.value().file_count;
            failc += itor.value().dir_count;
            size += itor.value().size;
        }
        emit updating(100, Fmt.arg(succ).arg(failc).arg(fformats(size)).arg(size),
                      -1, QString::null, -1, proc.end_unix_timestamp - proc.start_unix_timestamp);
        sets(karin_FileEngine::FileEngine_Transed);
    }
}

// MD5
int karin_FileEngine::check()
{
    static const QString Fmt = tr("Transfer %1: count : %2");
    karin_FileChecker *checker;
    struct file_porcess_s proc;
    int i;
    int m;
    int size;
    int thread_count;
    int part;
    if(m_state != karin_FileEngine::FileEngine_Transed)
        return -1;

    thread_count = karin_UT::Instance()->getsetting<int>(SETTING_MAX_WORKING_THREAD);
    if(thread_count <= 0 || thread_count > FILE_ENGINE_MAX_WORKING_THREAD)
        thread_count = 1;
    part = m_filec / thread_count + (m_filec % thread_count ? 1 : 0);

    i = 0;
    for(FileList_t::const_iterator itor = m_files.begin();
        itor != m_files.end(); ++itor)
    {
        m = 0;
        while(m < itor.value().size())
        {
            size = qMin(part, itor.value().size() - m);
            checker = new karin_FileChecker(i, this);
            checker->seteng2(this, itor.key(), m, size);
            m_checkers.insert(i, checker);
            connect(checker, SIGNAL(handlefinished(ftid_t, bool)), this, SLOT(checkfinished_slot(ftid_t, bool)));
            connect(checker, SIGNAL(checking(ftid_t, const QString &, bool, quint32, quint32)), this, SLOT(check_slot(ftid_t, const QString &, bool, quint32, quint32)));

            struct file_info_s info;
            info.id = i;
            proc.infos.insert(i, info);

            i++;
            m += size;
        }
    }

    qDebug() << QString("[_FileEngine]: check...(%1)").arg(i);

    sets(karin_FileEngine::FileEngine_Checking);
    proc.start_unix_timestamp = QDateTime::currentMSecsSinceEpoch();
    proc.status = PROCESS_STATUS_RUNNING;
    Q_FOREACH(karin_FileChecker *s, m_checkers)
    {
        s->start();
        emit updating(0, Fmt.arg(s->m_id).arg(s->m_files.size()),
                      karin_FileEngine::FileEngine_T, s->m_dstdir->absolutePath(), karin_FileEngine::FileEngine_Task_Start, 0);
    }
    m_process.insert(FileEngine_C, proc);

    return i;
}

void karin_FileEngine::check_slot(ftid_t id, const QString &file, bool suc, quint32 sc, quint32 fc)
{
    static const QString Fmt = tr("Check: %1 success, %2 fail");
    QMutexLocker locker(m_mutex);
    Q_UNUSED(locker);

    quint32 succ;
    quint32 failc;
    int per;

    if(m_state != karin_FileEngine::FileEngine_Checking)
        return;

    struct file_porcess_s &proc = m_process[karin_FileEngine::FileEngine_C];
    succ = 0;
    failc = 0;
    struct file_info_s &s = proc.infos[id];

    if(s.id != FT_INVALID_ID)
    {
        s.file_count = sc;
        s.dir_count = fc;
    }

    Q_FOREACH(const struct file_info_s &s, proc.infos)
    {
        succ += s.file_count;
        failc += s.dir_count;
    }
    proc.cur_unix_timestamp = QDateTime::currentMSecsSinceEpoch();
    per = (int)((qreal)(succ + failc) / (qreal)m_filec * 100);

    emit updating(per, Fmt.arg(succ).arg(failc),
                  karin_FileEngine::FileEngine_C, file, suc ? karin_FileEngine::FileEngine_Task_Success : karin_FileEngine::FileEngine_Task_Fail, proc.cur_unix_timestamp - proc.start_unix_timestamp);
}

void karin_FileEngine::checkfinished_slot(ftid_t id, bool suc)
{
    static const QString Fmt = tr("Check: %1 success, %2 fail");
    QMutexLocker locker(m_mutex);
    Q_UNUSED(locker);

    karin_FileChecker *checker;
    quint32 succ, failc;

    if(m_state != karin_FileEngine::FileEngine_Checking)
        return;

    checker = m_checkers.take(id);
    struct file_porcess_s &proc = m_process[karin_FileEngine::FileEngine_C];

    if(checker)
    {
        proc.infos[id].file_count = checker->m_files.size();
        proc.infos[id].dir_count = checker->m_diffs.size() + checker->m_missings.size();
        proc.infos[id].size = 0;
        checker->deleteLater();
        if(!suc)
            emit errocc(karin_FileEngine::FileEngine_CheckFail);
    }

    if(m_checkers.isEmpty())
    {
        succ = 0;
        failc = 0;
        proc.status = PROCESS_STATUS_SUC;
        proc.end_unix_timestamp = QDateTime::currentMSecsSinceEpoch();
        for(FileInfoList_t::const_iterator itor = proc.infos.begin();
            itor != proc.infos.end(); ++itor)
        {
            succ += itor.value().file_count;
            failc += itor.value().dir_count;
        }
        emit updating(100, Fmt.arg(succ).arg(failc),
                      -1, QString::null, -1, proc.end_unix_timestamp - proc.start_unix_timestamp);
        sets(karin_FileEngine::FileEngine_Done);
    }
}
