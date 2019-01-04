#include "karin_std.h"

#include <QFile>
#include <QFileInfo>
#include <QTextStream>
#include <QDateTime>
#include <QScopedPointer>
#include <QVariant>
#include <QCryptographicHash>
#include <QDebug>

QString fformatp(mask_t per)
{
#define P_R "r"
#define P_W "w"
#define P_X "x"
    QString result("---------");
    if(per & QFile::ReadOwner)
        result.replace(0, 1, P_R);
    if(per & QFile::WriteOwner)
        result.replace(1, 1, P_W);
    if(per & QFile::ExeOwner)
        result.replace(2, 1, P_X);
    if(per & QFile::ReadGroup)
        result.replace(3, 1, P_R);
    if(per & QFile::WriteGroup)
        result.replace(4, 1, P_W);
    if(per & QFile::ExeGroup)
        result.replace(5, 1, P_X);
    if(per & QFile::ReadOther)
        result.replace(6, 1, P_R);
    if(per & QFile::WriteOther)
        result.replace(7, 1, P_W);
    if(per & QFile::ExeOther)
        result.replace(8, 1, P_X);
    return result;

#undef P_R
#undef P_W
#undef P_X
}

QString fformats(qint64 size, bool isdir)
{
#define KB 1024
#define MB (KB * KB)
#define GB (MB * KB)

    static const QString P("%1%2");
    QString result;

    if(isdir)
    {
        if(size <= 4 * KB)
            result = P.arg(size).arg('b');
        else if(size > 4 * KB && size <= 64 * KB)
            result = P.arg(size / KB).arg('k');
    }
    else
    {
        QVariant var(size);
        double d = var.value<double>();
        if(size > 2 * KB && size < 2 * MB){
            result = P.arg(d / KB, 1, 'f', 1).arg('k');
        }
        else if(size >= 2 * MB && size < GB){
            result = P.arg(d / MB, 1, 'f', 1).arg('M');
        }
        else if(size <= 2 * KB){
            result = P.arg(size).arg('b');
        }else{
            result = P.arg(d / GB, 1, 'f', 1).arg('G');
        }
    }
    return result;
#undef KB
#undef MB
#undef GB
}

QString frdtext(const QString &path)
{
    QString r;
    QTextStream in;
    QFile *file;
    QScopedPointer<QFile> pointer;

    qDebug()<<QFile(path).isReadable();
    if(path.isEmpty())
        goto __Exit;
    file = new QFile(path);
    pointer.reset(file);
    if(!file->exists())
        goto __Exit;
    if(!file->open(QIODevice::ReadOnly | QIODevice::Text))
        goto __Exit;

    in.setDevice(file);
    r = in.readAll();

    file->close();

__Exit:
    return r;
}

bool faptext(const QString &path, const QString &content)
{
    bool r;
    QTextStream in;
    QFile *file;
    QScopedPointer<QFile> pointer;

    r = false;
    qDebug()<<QFile(path).isReadable();
    if(path.isEmpty())
        goto __Exit;
    file = new QFile(path);
    pointer.reset(file);
    if(!file->open(QIODevice::Append | QIODevice::Text))
        goto __Exit;

    in.setDevice(file);
    in << content;

    file->close();
    r = true;

__Exit:
    return r;
}

int fcp(const QString &dst, const QString &src)
{
    qDebug()<<dst <<"<<"<<src;
    QFileInfo dst_info(dst);
    if(dst_info.exists())
    {
        QByteArray dst_md5 = filemd5(dst);
        QByteArray src_md5 = filemd5(src);
        if(dst_md5 == src_md5)
            return FCP_FILE_EXISTS;
        else
            return FCP_FILE_EXISTS_BUT_NOT_SAME;
    }
    return QFile::copy(src, dst) ? FCP_SUCCESS : FCP_FAIL;
}


QString formatuts(qint64 d)
{
#define S_PER_H 3600
#define S_PER_M 60
#define MS_PER_S 1000
    qint64 s;
    QString r;

    s = d / MS_PER_S;
    if(s > S_PER_H)
    {
        r += QString::number(s / S_PER_H);
        r += " ";
        r += QObject::tr("hour");
        r += " ";
        s = s % S_PER_H;
    }
    {
        r += QString::number(s / S_PER_M);
        r += " ";
        r += QObject::tr("minute");
        r += " ";
        s = s % S_PER_M;
    }
    {
        r += QString::number(s);
        r += " ";
        r += QObject::tr("second");
    }
    return r;
}

int randr(int min, int max)
{
    int s = min;
    int e = max;
    if(min > max)
        qSwap(s, e);
    return qrand() % (e - s) + s;
}

QByteArray filemd5(const QString &filePath)
{
    const quint64 Buf_Size = 4096;
    QFile file(filePath);
    quint64 total = 0;
    quint64 written = 0;
    quint64 unwrite = 0;
    QByteArray buf;

    if (!file.open(QFile::ReadOnly))
    {
        return 0;
    }

    QCryptographicHash ch(QCryptographicHash::Md5);

    total = file.size();
    unwrite = total;

    while(written < total)
    {
        buf = file.read(qMin(unwrite, Buf_Size));
        ch.addData(buf);
        written += buf.length();
        unwrite -= buf.length();
        buf.resize(0);
    }

    file.close();
    QByteArray md5 = ch.result();
    return md5;
}

int fcmp(const QString &f, const QString &s)
{
    QFileInfo first(f);
    QFileInfo sec(s);

    if(!first.exists())
        return FCMP_FIRST_MISSING;
    if(!sec.exists())
        return FCMP_SECOND_MISSING;

    if(!first.isFile() || !sec.isFile())
        return FCMP_TYPE_DIFF;

    QByteArray f_md5 = filemd5(f);
    QByteArray s_md5 = filemd5(s);

    if(f_md5 == s_md5)
        return FCMP_EQUALS;
    else
        return FCMP_NOT_EQUALS;
}
