#ifndef KARIN_UT_H
#define KARIN_UT_H

#include <QSettings>
#include <QDebug>
#include "karin_std.h"


#define SETTING_LEFT_PATH "left_path"
#define SETTING_RIGHT_PATH "right_path"
#define SETTING_LOG_LEVEL "log_level"
#define SETTING_ENABLE_LOG "enable_log"
#define SETTING_LAST_OPEN_IMAGE_PATH "last_open_image_path"
#define SETTING_LAST_SAVE_IMAGE_PATH "last_save_image_path"
#define SETTING_MAX_WORKING_THREAD "max_working_thread"

#define LOG_DIR "./.logs"
#define LOG_FILE_PREFIX "log"
#define LOG_FILE_EXT "txt"

#define LOCAL_SETTING_FILE "./settings.ini"

class karin_UT
{
public:
    enum Log_Level_e
    {
        Log_Debug_Level = K_MASK(0),
        Log_Info_Level = K_MASK(1),
        Log_Warning_Level = K_MASK(2),
        Log_Error_Level = K_MASK(3),
        Log_Fatal_Level = K_MASK(4),
        Log_All_Level = K_MASK_ALL
    };

public:
    virtual ~karin_UT();

    static karin_UT * Instance();
    template <class T> T getsetting(const QString &key);
    template <class T> void setsetting(const QString &key, const T &value);
    QString getabout() const;
    void enablelog(bool b);
    void setloglvl(mask_t m) { m_loglvl = m; }
    void setlogbase(Log_Level_e b);
    void wrlog(Log_Level_e e, const QString &content) const;
    QString logfile() const;

    _DEVELOPER_USING quint32 mktestfiles(const QString &path, quint32 count, int s);

private:
    QVariant Defsettings(const QString &key);

private:
    karin_UT();
    QSettings *m_setting;
    bool m_log;
    mask_t m_loglvl;
};

template <class T> T karin_UT::getsetting(const QString &key)
{
    QVariant var = m_setting->value(key, Defsettings(key));
    if(var.canConvert<T>())
        return var.value<T>();
    return T();
}

template <class T> void karin_UT::setsetting(const QString &key, const T &value)
{
    m_setting->setValue(key, value);
}

#endif // KARIN_UT_H
