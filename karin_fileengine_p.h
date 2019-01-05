#ifndef KARIN_FILEENGINE_P_H
#define KARIN_FILEENGINE_P_H

#include <QThread>
#include <QStringList>

#define FT_INVALID_ID -1
#define FE_IN private:
#define FE_OUT private:

typedef qint8 ftid_t;

class karin_FileEngine;
class QDir;

class karin_FileThread : public QThread
{
    Q_OBJECT
public:
    enum FileThread_State_e
    {
        FileThread_None = 0,
        FileThread_Ready,
        FileThread_Doing,
        FileThread_Done,
        FileThread_Fail
    };

public:
    karin_FileThread(ftid_t id = FT_INVALID_ID, QObject *parent = 0);
    virtual ~karin_FileThread();
    FileThread_State_e state() const { return m_state; }

    virtual void run();
    virtual void start(QThread::Priority p = QThread::InheritPriority);
    virtual void pause();
    virtual void store(QIODevice *out) = 0;
    virtual void load(QIODevice *in) = 0;
    virtual QString log() = 0;

Q_SIGNALS:
    void handlefinished(ftid_t m_id, bool suc);
    void stateChanged(FileThread_State_e s);

private Q_SLOTS:
    virtual void finished_slot();
    virtual void terminated_slot();

protected:
    virtual void handle() = 0;
    void sets(FileThread_State_e s);
    virtual void seteng(karin_FileEngine *e) { Q_UNUSED(e); }
    virtual void reset() {}
    virtual void clear();

protected:
    ftid_t m_id;
    FileThread_State_e m_state;
    //karin_FileEngine *m_engine;
    int m_pause;

    friend class karin_FileEngine;
};

// 文件夹扫描器
class karin_FileScanner : public karin_FileThread
{
    Q_OBJECT
public:
    karin_FileScanner(ftid_t id, QObject *parent = 0);
    virtual ~karin_FileScanner();
    void setd(const QString &s);
    virtual void pause();
    virtual void store(QIODevice *out);
    virtual void load(QIODevice *in);
    virtual QString log();

Q_SIGNALS:
    void scanning(ftid_t id, const QString &dir, quint32 fcount, quint32 dcount, quint64 fsize);

protected:
    virtual void handle();
    virtual void reset();
    virtual void clear();

private:
    int getallfiles();
    int getallfiles_r(const QString &path, QStringList *r, QStringList *r_dir = 0);

FE_IN
    QString m_dirstr; // 扫描的目录
    QDir *m_dir;
FE_OUT
    QStringList m_dirs; // 文件夹列表, 相对路径
    QStringList m_files; // 文件列表, 相对路径
    quint64 m_size; // 文件大小

    friend class karin_FileEngine;
};

// 文件夹创建器
class karin_FileDirMaker : public karin_FileThread
{
    Q_OBJECT

public:
    karin_FileDirMaker(ftid_t id, QObject *parent = 0);
    virtual ~karin_FileDirMaker();
    const QStringList & fails() const { return m_fails; }
    virtual void pause();
    virtual void store(QIODevice *out);
    virtual void load(QIODevice *in);
    virtual QString log();

Q_SIGNALS:
    void mkingdir(ftid_t id, QString m_dir, bool suc, quint32 c);

protected:
    virtual void handle();
    virtual void seteng(karin_FileEngine *e);
    virtual void reset();
    virtual void clear();

private:
    int mkdir();
    int mkdir_p(const QString &s);

FE_IN
    QDir *m_dir; // 目标文件夹
    QStringList m_dirs; // 文件夹相对路径
FE_OUT
    QStringList m_fails; // 失败的
    quint32 m_countc; // 已完成数量

    friend class karin_FileEngine;
};

// 文件复制器
class karin_FileTransfer : public karin_FileThread
{
    Q_OBJECT

public:
    karin_FileTransfer(ftid_t id, QObject *parent = 0);
    virtual ~karin_FileTransfer();
    virtual void pause();
    virtual void store(QIODevice *out);
    virtual void load(QIODevice *in);
    virtual QString log();

Q_SIGNALS:
    void transfering(ftid_t id, QString file, bool suc, quint32 c, quint64 size);

protected:
    virtual void handle();
    virtual void reset();
    virtual void clear();
    void seteng2(karin_FileEngine *e, const QString &srcdir, const int start, int count);

private:
    int trans();

FE_IN
    QDir *m_srcdir; // 源文件夹
    QDir *m_dstdir; // 目标文件夹
    QStringList m_files; // 待复制文件, 相对路径
FE_OUT
    QStringList m_fails; // 复制失败的
    quint32 m_count; // 总数
    quint64 m_size; // 总大小
    quint32 m_countc; // 完成数量
    quint64 m_sizec; // 完成大小

    friend class karin_FileEngine;
};

// 文件检查器
class karin_FileChecker : public karin_FileThread
{
    Q_OBJECT

public:
    karin_FileChecker(ftid_t id, QObject *parent = 0);
    virtual ~karin_FileChecker();
    virtual void pause();
    virtual void store(QIODevice *out);
    virtual void load(QIODevice *in);
    virtual QString log();

Q_SIGNALS:
    void checking(ftid_t id, QString file, bool suc, quint32 c, quint32 f);

protected:
    virtual void handle();
    virtual void reset();
    virtual void clear();
    void seteng2(karin_FileEngine *e, const QString &srcdir, const int start, int count);

private:
    int check();

FE_IN
    QDir *m_srcdir; // 源文件夹
    QDir *m_dstdir; // 目标文件夹
    QStringList m_files; // 待检查文件, 相对路径
FE_OUT
    QStringList m_diffs; // 内容不同(时间不同不计)
    QStringList m_missings; // 没有的文件
    quint32 m_countc; // 当前进度

    friend class karin_FileEngine;
};

#endif // KARIN_FILEENGINE_P_H
