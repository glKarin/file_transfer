#include "karin_fileengine_p.h"
#include "karin_fileengine.h"
#include "karin_std.h"

#include <QMutex>
#include <QMutexLocker>
#include <QFileInfo>
#include <QDir>
#include <QDebug>

// karin_FileThread
karin_FileThread::karin_FileThread(ftid_t id, QObject *parent)
    : QThread(parent),
      m_id(id),
      m_state(karin_FileThread::FileThread_None)
{
    connect(this, SIGNAL(finished()), this, SLOT(finished_slot()));
    connect(this, SIGNAL(terminated()), this, SLOT(terminated_slot()));
}

karin_FileThread::~karin_FileThread()
{

}

void karin_FileThread::clear()
{
    m_state = karin_FileThread::FileThread_None;
}

void karin_FileThread::start(QThread::Priority p)
{
    if(m_state != karin_FileThread::FileThread_Ready)
        return;

    QThread::start(p);
}

void karin_FileThread::run()
{
    qDebug()<<currentThreadId();
    sets(karin_FileThread::FileThread_Doing);
    handle();
    sets(karin_FileThread::FileThread_Done);
}

void karin_FileThread::sets(FileThread_State_e s)
{
    if(m_state != s)
    {
        m_state = s;
        emit stateChanged(m_state);
    }
}

void karin_FileThread::finished_slot()
{
    sets(karin_FileThread::FileThread_Done);
    emit handlefinished(m_id, true);
    qDebug()<<"FFFFFFFFFFFFFFF";
}

void karin_FileThread::terminated_slot()
{
    sets(karin_FileThread::FileThread_Fail);
    emit handlefinished(m_id, false);
    qDebug()<<"TTTTTTTTTTTTTT";
}



// karin_FileScanner
karin_FileScanner::karin_FileScanner(ftid_t id, QObject *parent)
    : karin_FileThread(id, parent),
      m_dir(0),
      m_size(0)
{
}

karin_FileScanner::~karin_FileScanner()
{
    delete m_dir;
}

void karin_FileScanner::setd(const QString &s)
{
    if(m_state != karin_FileThread::FileThread_None)
        return;
    m_dirstr = s;
    sets(karin_FileThread::FileThread_Ready);
}

void karin_FileScanner::reset()
{
    m_dirs.clear();
    m_files.clear();
    m_size = 0;
    karin_FileThread::reset();
}

void karin_FileScanner::clear()
{

}

void karin_FileScanner::store(QIODevice *out)
{

}

void karin_FileScanner::load(QIODevice *in)
{

}

QString karin_FileScanner::log()
{

}

void karin_FileScanner::handle()
{
    reset();
    getallfiles();
}

int karin_FileScanner::getallfiles_r(const QString &path, QStringList *r, QStringList *r_dir)
{
    int c;

    if(!r)
        return 0;

    c = 0;
    QDir dir(path);

    if(!dir.isReadable())
        return 0;

    if(r_dir)
        r_dir->push_back(m_dir->relativeFilePath(path));

    QFileInfoList list = dir.entryInfoList();
    Q_FOREACH(const QFileInfo &info, list)
    {
        if(info.fileName() == "." || info.fileName() == "..")
            continue;

        if(info.isDir())
            c += getallfiles_r(info.absoluteFilePath(), r, r_dir);
        else
        {
            r->push_back(m_dir->relativeFilePath(info.absoluteFilePath()));
            c++;
        }
        m_size += info.size();
    }

    emit scanning(m_id, m_dir->absolutePath(), m_files.size(), m_dirs.size(), m_size);

    return c;
}

int karin_FileScanner::getallfiles()
{
    int c;

    c = 0;
    delete m_dir;
    QFileInfo info(m_dirstr);
    if(info.isDir())
    {
        m_dir = new QDir(m_dirstr);
        m_dirstr = info.absoluteFilePath();
        c = getallfiles_r(m_dir->absolutePath(), &m_files, &m_dirs);
    }
    else
    {
        m_dirstr = info.absolutePath();
        m_dir = new QDir(m_dirstr);
        m_files.push_back(info.fileName());
        m_size += info.size();
        c++;
    }

    delete m_dir;
    m_dir = 0;

    return c;
}



// karin_FileDirMaker
karin_FileDirMaker::karin_FileDirMaker(ftid_t id, QObject *parent)
    : karin_FileThread(id, parent),
      m_dir(0),
      m_countc(0)
{
}

karin_FileDirMaker::~karin_FileDirMaker()
{
    delete m_dir;
}

void karin_FileDirMaker::reset()
{
    m_dirs.clear();
    m_fails.clear();
    m_countc = 0;
    karin_FileThread::reset();
}

void karin_FileDirMaker::clear()
{

}

void karin_FileDirMaker::store(QIODevice *out)
{

}

void karin_FileDirMaker::load(QIODevice *in)
{

}

QString karin_FileDirMaker::log()
{

}

void karin_FileDirMaker::handle()
{
    reset();
    mkdir();
}

int karin_FileDirMaker::mkdir()
{
    qDebug()<<"mkdir2";
    int c;

    Q_FOREACH(const QString &s, m_dirs)
    {
        if(mkdir_p(s) >= 0)
            c++;
    }

    return c;
}

int karin_FileDirMaker::mkdir_p(const QString &s)
{
    bool b;
    int r;
    QFileInfo dir(m_dir->absoluteFilePath(s));

    r = -3;
    m_countc++;

    if(s.isEmpty())
    {
        r = -3;
        goto __Exit;
    }

    qDebug()<<m_dir->absoluteFilePath(s);
    if(dir.exists())
    {
        if(dir.isDir()/* && dir.isReadable()*/)
            r = 1;
        else
        {
            m_fails.push_back(s);
            r = -1;
        }
    }
    else
    {
        b = m_dir->mkpath(s);
        if(!b)
            m_fails.push_back(s);
        r = b ? 0 : -2;
    }

__Exit:
    emit mkingdir(m_id, s, r >= 0, m_countc);
    return r;
}

void karin_FileDirMaker::seteng(karin_FileEngine *e)
{
    if(m_state != karin_FileThread::FileThread_None)
        return;
    if(!e)
        return;
    m_dir = new QDir(e->m_dst);
    m_dirs = e->m_dirs;
    sets(karin_FileThread::FileThread_Ready);
}




// karin_FileTransfer
karin_FileTransfer::karin_FileTransfer(ftid_t id, QObject *parent)
    : karin_FileThread(id, parent),
      m_srcdir(0),
      m_dstdir(0),
      m_count(0),
      m_size(0),
      m_countc(0),
      m_sizec(0)
{
}

karin_FileTransfer::~karin_FileTransfer()
{
    delete m_srcdir;
    delete m_dstdir;
}

void karin_FileTransfer::reset()
{
    m_fails.clear();
    m_countc = 0;
    m_sizec = 0;
    karin_FileThread::reset();
}

void karin_FileTransfer::clear()
{

}

void karin_FileTransfer::store(QIODevice *out)
{

}

void karin_FileTransfer::load(QIODevice *in)
{

}

QString karin_FileTransfer::log()
{

}

void karin_FileTransfer::handle()
{
    reset();
    trans();
}

int karin_FileTransfer::trans()
{
    int r;
    QString src;
    QString dst;

    Q_FOREACH(const QString &file, m_files)
    {
        dst = m_dstdir->absoluteFilePath(file);
        src = m_srcdir->absoluteFilePath(file);
        r = fcp(dst, src);
        qDebug()<<r;
        if(r >= 0)
        {
            m_countc++;
            m_sizec += QFileInfo(src).size();
        }
        else
        {
            m_fails.push_back(file);
        }
        emit transfering(m_id, dst, r >= 0, m_countc, m_sizec);
    }
}

void karin_FileTransfer::seteng2(karin_FileEngine *e, const QString &srcdir, const int start, int count)
{
    if(m_state != karin_FileThread::FileThread_None)
        return;
    if(!e)
        return;
    m_dstdir = new QDir(e->m_dst);
    m_srcdir = new QDir(srcdir);

    for(int i = 0; i < count; i++)
    {
        m_files.push_back(e->m_files[srcdir][start + i]);
    }
    sets(karin_FileThread::FileThread_Ready);
}



// karin_FileChecker
karin_FileChecker::karin_FileChecker(ftid_t id, QObject *parent)
    : karin_FileThread(id, parent),
    m_srcdir(0),
    m_dstdir(0),
    m_countc(0)
{

}

karin_FileChecker::~karin_FileChecker()
{
    delete m_srcdir;
    delete m_dstdir;
}

void karin_FileChecker::reset()
{
    m_diffs.clear();
    m_missings.clear();
    m_countc = 0;
    karin_FileThread::reset();
}

void karin_FileChecker::clear()
{

}

void karin_FileChecker::store(QIODevice *out)
{

}

void karin_FileChecker::load(QIODevice *in)
{

}

QString karin_FileChecker::log()
{

}

void karin_FileChecker::handle()
{
    reset();
    check();
}

int karin_FileChecker::check()
{
    int r;
    QString src;
    QString dst;

    m_countc = 0;

    Q_FOREACH(const QString &file, m_files)
    {
        dst = m_dstdir->absoluteFilePath(file);
        src = m_srcdir->absoluteFilePath(file);
        r = fcmp(dst, src);
        qDebug()<<r;
        switch(r)
        {
        case FCMP_TYPE_DIFF:
        case FCMP_NOT_EQUALS:
            m_diffs.push_back(file);
            break;
        case FCMP_FIRST_MISSING:
            m_missings.push_back(file);
            break;
        case FCMP_SECOND_MISSING:
            break;
        case FCMP_EQUALS:
            default:
            break;
        }
        m_countc++;
        emit checking(m_id, dst, r >= 0, m_countc, m_diffs.size() + m_missings.size());
    }

    return m_countc;
}

void karin_FileChecker::seteng2(karin_FileEngine *e, const QString &srcdir, const int start, int count)
{
    if(m_state != karin_FileThread::FileThread_None)
        return;
    if(!e)
        return;
    m_dstdir = new QDir(e->m_dst);
    m_srcdir = new QDir(srcdir);

    for(int i = 0; i < count; i++)
    {
        m_files.push_back(e->m_files[srcdir][start + i]);
    }
    sets(karin_FileThread::FileThread_Ready);
}
