#ifndef KARIN_FILEENGINE_H
#define KARIN_FILEENGINE_H

#include "karin_fileengine_p.h"

#include <QStringList>
#include <QObject>
#include <QHash>

#define FILE_ENGINE_MAX_WORKING_THREAD 32

class karin_FileScanner;
class karin_FileTransfer;
class karin_FileDirMaker;
class QMutex;

extern const QString Task_Names[];
extern const QString Task_State[];

struct file_info_s
{
    //QString dir;
    ftid_t id;
    quint32 file_count;
    quint32 dir_count;
    quint64 size;

    file_info_s()
        : id(FT_INVALID_ID), file_count(0), dir_count(0), size(0)
    {}
};

typedef QHash<ftid_t, struct file_info_s> FileInfoList_t;
typedef QHash<ftid_t, karin_FileThread *> FileThreadList_t;
typedef QHash<ftid_t, karin_FileScanner *> FileScannerList_t;
typedef QHash<ftid_t, karin_FileTransfer *> FileTransferList_t;
typedef QHash<ftid_t, karin_FileDirMaker *> FileDirMkerList_t;
typedef QHash<ftid_t, karin_FileChecker *> FileCheckList_t;
typedef QHash<int, struct file_porcess_s> ProcessList_t;
typedef QHash<QString, QStringList> FileList_t;


#define PROCESS_STATUS_READY -1
#define PROCESS_STATUS_SUC 0
#define PROCESS_STATUS_FAIL 1
#define PROCESS_STATUS_RUNNING 2
struct file_porcess_s
{
    FileThreadList_t *thread;
    FileInfoList_t infos;
    qint64 start_unix_timestamp; // 起始时间戳
    qint64 cur_unix_timestamp; // 当前时间戳
    qint64 end_unix_timestamp; // 结束时间戳
    int status; // 结束状态 0 - 正常
    file_porcess_s()
        : thread(0), start_unix_timestamp(0), cur_unix_timestamp(0), end_unix_timestamp(0), status(PROCESS_STATUS_READY)
    {}
};

class karin_FileEngine : public QObject
{
    Q_OBJECT
public:
    enum FileEngine_State_e
    {
        FileEngine_Ready = 0,
        FileEngine_Prepare,
        FileEngine_Scanning,
        FileEngine_Scanned,
        FileEngine_Mkdir,
        FileEngine_Trans,
        FileEngine_Transed,
        FileEngine_Checking,
        FileEngine_Done,
    };
    enum FileEngine_Process_e
    {
        FileEngine_S = 0,
        FileEngine_M,
        FileEngine_T,
        FileEngine_C,
    };
    enum FileEngine_Task_State_e
    {
        FileEngine_Task_Ready = 0,
        FileEngine_Task_Start,
        FileEngine_Task_Pause,
        FileEngine_Task_Doing,
        FileEngine_Task_Success,
        FileEngine_Task_Fail,
    };

public:
    enum FileEngine_Err_e
    {
        FileEngine_NoError = 0,
        FileEngine_DstDirIsNotADir,
        FileEngine_DstDirIsNotBeReadable,
        FileEngine_SrcFilesIsEmpty,
        FileEngine_ReadDirFail,
        FileEngine_MakeDirFail,
        FileEngine_TransferFail,
        FileEngine_CheckFail,
    };

public:
    karin_FileEngine(QObject *parent = 0);
    virtual ~karin_FileEngine();

    bool prepare(const QStringList &dirs, const QString &dst);
    FileEngine_State_e state() const { return m_state; }
    QString statestr() const;

public Q_SLOTS:
    int transfer(const QStringList &dirs, const QString &dst);
    int scan();
    int mkdirs();
    int trans();
    int check();

    void cancel();
    void pause();

Q_SIGNALS:
    void updating(int per, const QString &msg, int proc, const QString &file, int suc, quint64 time);
    void errocc(int err);
    void stateChanged(FileEngine_State_e s);

private Q_SLOTS:
    void scanningfinished_slot(ftid_t id, bool suc);
    void mkingdirfinished_slot(ftid_t id, bool suc);
    void transferfinished_slot(ftid_t id, bool suc);
    void checkfinished_slot(ftid_t id, bool suc);
    void sets(FileEngine_State_e s);
    void scanning_slot(ftid_t id, const QString &dir, quint32 fcount, quint32 dcount, quint64 fsize);
    void mkingdir_slot(ftid_t id, const QString &dir, bool suc, quint32 count);
    void transfer_slot(ftid_t id, const QString &file, bool suc, quint32 c, quint64 size);
    void check_slot(ftid_t id, const QString &file, bool suc, quint32 c, quint32 size);

private:
    void deinit();

private:
    FileEngine_State_e m_state;
    QMutex *m_mutex;
    ProcessList_t m_process; // 任务信息
    QString m_dst; // 目标路径
    QStringList m_src; // 源路径列表
    QStringList m_dirs; // 文件夹列表, 相对路径
    FileList_t m_files; // 文件列表, 目录=>相对路径
    quint64 m_size; // 文件大小
    quint32 m_dirc; // 当前文件夹数量
    quint32 m_filec; // 当前文件数量
    FileScannerList_t m_scanners; // 扫描任务
    FileDirMkerList_t m_dirmkers; // 文件夹生成任务
    FileTransferList_t m_transfers; // 文件复制任务
    FileCheckList_t m_checkers; // 文件检查任务

    friend class karin_FileThread;
    friend class karin_FileScanner;
    friend class karin_FileDirMaker;
    friend class karin_FileTransfer;
    friend class karin_FileChecker;
};

#endif // KARIN_FILEENGINE_H
