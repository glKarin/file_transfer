#include "gl/gl_std.h"
#include <QtGui/QApplication>
#include <QMetaType>
#include "mainwindow.h"
#include "karin_std.h"
#include "karin_ut.h"
#include "karin_fileengine_p.h"
#include "karin_glsplash.h"
#include "qt/karin_nltmainwindow.h"
#include "qt/karin_nltviewer.h"


#include <QtCore>
#include <QtGui>
#include <math.h>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    //MainWindow w;
    //karin_GLSplash w;
    karin_NLTMainWindow w;
    //karin_NLTViewer w;
    QSplashScreen *splash;

    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));

    Q_INIT_RESOURCE(karin);
    qRegisterMetaType<ftid_t>("ftid_t");

    app.setApplicationName(_APP_PKG);
    app.setApplicationVersion(_APP_VER);
    app.setOrganizationName(_APP_DEV);

    QTranslator translator;
    QString locale = QLocale::system().name();

    qDebug()<<"Load i18n -> "_APP_PKG"."<<locale.toStdString().c_str()<<".qm";

    if(translator.load(QString(_APP_PKG".") + locale, "E:\\pro\\qt\\file_transfer\\i18n"))
    {
        qDebug()<<"Done";
        app.installTranslator(&translator);
    }
    else
        qDebug()<<"Fail";

    // for test
    //karin_UT::Instance()->mktestfiles("C:/SS/tt", 1000000, 0);

    splash = new QSplashScreen;
    splash->show();
    splash->raise();
    splash->finish(&w);
    splash->deleteLater();

    w.show();
    //w.open("D:/phpStudy/PHPTutorial/WWW/tcjk/static/icon/g_conf_list.png");
    //QTimer::singleShot(2000, splash, SLOT(deleteLater()));
    //QObject::connect(splash, SIGNAL(destroyed()), &w, SLOT(show()));

    /*
    D:/prog/QtSDK/Desktop/Qt/4.8.1/msvc2010/bin/lupdate.exe -no-obsolete ./*.cpp ./*.h gl/* -ts i18n/file_transfer.zh_CN.ts
    D:/prog/QtSDK/Desktop/Qt/4.8.1/msvc2010/bin/lupdate.exe -no-obsolete ./* gl/* -ts i18n/file_transfer.ts
    */
    
    return app.exec();
}
