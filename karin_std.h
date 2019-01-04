#ifndef KARIN_STD_H
#define KARIN_STD_H

#include <QString>
#include <QVector3D>

#define FCP_FILE_EXISTS 1
#define FCP_FILE_EXISTS_BUT_NOT_SAME -2
#define FCP_SUCCESS 0
#define FCP_FAIL -1

#define FCMP_EQUALS 0
#define FCMP_NOT_EQUALS -1
#define FCMP_FIRST_MISSING -2
#define FCMP_SECOND_MISSING -3
#define FCMP_TYPE_DIFF -4

#define FILE_GRID_WIDTH 128
#define FILE_GRID_HEIGHT 128

#define K_MASK(x) (1 << (x))
#define K_MASK_ALL (~0)

#define countof(x) (sizeof(x) / sizeof(x[0]))

#define CUR_UTS() QDateTime::currentMSecsSinceEpoch()

#define QRC_FILE "karin.qrc"

typedef unsigned long mask_t;
typedef unsigned enum_t;
typedef float clampf_t;

QString fformatp(mask_t per);
QString fformats(qint64 size, bool isdir = false);
QString frdtext(const QString &path);
int fcp(const QString &dst, const QString &src);
bool faptext(const QString &path, const QString &content);
QString formatuts(qint64 d);
int randr(int min, int max);
QByteArray filemd5(const QString &file);
int fcmp(const QString &f, const QString &s);


#endif // KARIN_STD_H
