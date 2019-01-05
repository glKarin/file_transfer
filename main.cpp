#include <QtGui/QApplication>
#include <QMetaType>
#include "mainwindow.h"
#include "karin_std.h"
#include "karin_ut.h"
#include "karin_fileengine_p.h"
#include "karin_glsplash.h"

#include <QtCore>
#include <QtGui>
#include <math.h>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    MainWindow w;
    //karin_GLSplash w;
    QSplashScreen *splash;

    Q_INIT_RESOURCE(karin);
    qRegisterMetaType<ftid_t>("ftid_t");
    app.setApplicationName(_APP_PKG);
    app.setApplicationVersion(_APP_VER);
    app.setOrganizationName(_APP_DEV);


    splash = new QSplashScreen;
    splash->show();
    splash->raise();
    splash->finish(&w);
    splash->deleteLater();

    w.show();
    //QTimer::singleShot(2000, splash, SLOT(deleteLater()));
    //QObject::connect(splash, SIGNAL(destroyed()), &w, SLOT(show()));
    
    return app.exec();
}
