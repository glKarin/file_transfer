#include "karin_ut.h"

#include <QHash>
#include <QDir>
#include <QDate>
#include <QDateTime>
#include <QDebug>

karin_UT::karin_UT()
    : m_setting(new QSettings)
    , m_log(false)
    , m_loglvl(karin_UT::Log_All_Level)
{
    enablelog(getsetting<bool>(SETTING_ENABLE_LOG));
    setloglvl(getsetting<int>(SETTING_LOG_LEVEL));
}

karin_UT::~karin_UT()
{
    delete m_setting;
}

karin_UT * karin_UT::Instance()
{
    static karin_UT Ut;
    return &Ut;
}


QVariant karin_UT::Defsettings(const QString &key)
{
    static QHash<QString, QVariant> Def_Setting; // = {} // c++11
    if(Def_Setting.isEmpty())
    {
        Def_Setting.insert(SETTING_LEFT_PATH, QDir::homePath());
        Def_Setting.insert(SETTING_RIGHT_PATH, QDir::rootPath());
        Def_Setting.insert(SETTING_LOG_LEVEL, false);
        Def_Setting.insert(SETTING_ENABLE_LOG, karin_UT::Log_All_Level & (~karin_UT::Log_Debug_Level));
        Def_Setting.insert(SETTING_MAX_WORKING_THREAD, 1);
    }
    return Def_Setting.value(key);
}

QString karin_UT::getabout() const
{
    QString s;
    s += _APP_NAME"<br/>";
    s += QObject::tr("Version") + ": " _APP_VER "<br/>";
    s += QObject::tr("Dev") + ": <a href='mailto:" + _APP_EMAIL + "'>" _APP_DEV "</a><br/>";
    s += QObject::tr("Release") + ": " + QString::number(_APP_RELEASE) + "<br/>";
    s += QObject::tr("Code") + ": " _APP_CODE"<br/>";
    s += QObject::tr("Source") + ": <a href='" _APP_GITHUB "'>" + _APP_GITHUB "</a><br/>";
    return s;
}

void karin_UT::setlogbase(karin_UT::Log_Level_e b)
{
    int i;
    mask_t m;

    m = ~b;
    i = 0;
    while((m >> i) & 1)
    {
        m ^= (1 << i);
        i++;
    }

    m_loglvl = (m | b);
}

void karin_UT::wrlog(Log_Level_e e, const QString &content) const
{
    static const QString Fmt("[%1]%2 -> %3\n");
    static const QString Log_Lvls[] = {
        "DEBUG",
        "INFO ",
        "WARN ",
        "ERROR",
        "FATAL",
    };
    int i;

    if(!m_log)
        return;

    if(e & m_loglvl)
    {
        i = 0;
        while(((e >> i) & 1) == 0)
            i++;

        faptext(logfile(), Fmt.arg(Log_Lvls[i]).arg(QDateTime::currentDateTime().toString("yyyy.MM.dd-hh:mm:ss")).arg(content));
    }
}

QString karin_UT::logfile() const
{
    static const QString Fmt("%1/%2_%3.%4");

    return Fmt.arg(LOG_DIR).arg(LOG_FILE_PREFIX).arg(QDate::currentDate().toString("yyyyMMdd")).arg(LOG_FILE_EXT);
}

void karin_UT::enablelog(bool b)
{
    if(b)
    {
        QFileInfo info(LOG_DIR);
        if(info.exists())
        {
            m_log = info.isDir();
        }
        else
        {
            m_log = QDir(LOG_DIR).mkpath(".");
        }
    }
    else
        m_log = b;
}
