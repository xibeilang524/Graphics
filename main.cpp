#include "mainwindow.h"
#include "myapplication.h"

#include <QTextCodec>
#include <QDesktopWidget>
#include <QDir>

#include "./manager/actionmanager.h"
#include "SelfWidget/hidesplit.h"
#include "SelfWidget/mysplashscreen.h"
#include "global.h"
#include "sql/sqlproecss.h"
#include "mysettings.h"
#include "util.h"

const char APP_NAME[] = "Graphics";
const char CONFIG_PATH[] = "Graphics.ini";

MySettings * createSettings()
{
    return new MySettings(QString("%1/%2").arg(APP_NAME).arg(CONFIG_PATH),QSettings::IniFormat);
}

int main(int argc, char *argv[])
{    
    MyApplication a(argc, argv);
    a.setApplicationVersion(QString::number(M_VERTION));
    a.setOrganizationName(M_AUTHOR);
    a.setApplicationName(APP_NAME);

    ScreenWidth = qApp->desktop()->screenGeometry().width();
    ScreenHeight = qApp->desktop()->screenGeometry().height();

    MyApplication::addLibraryPath("./plugins");

    QTextCodec * codec = QTextCodec::codecForName("GB2312");
    QTextCodec::setCodecForCStrings(codec);

    MySettings * settings = createSettings();
    settings->setIniCodec(codec);

    QFileInfo fileInfo(settings->fileName());
    IsConfigIniExisted = fileInfo.exists();

    QPixmap pixmap(":/images/startup.png");
    MySplashScreen screen(pixmap);
    screen.setSettings(settings);
    screen.setGeometry((ScreenWidth - pixmap.width())/2,(ScreenHeight - pixmap.height())/2,pixmap.width(),pixmap.height());
    screen.show();
    a.processEvents();

    QDateTime n = QDateTime::currentDateTime();
    QDateTime now;
    do{
        now = QDateTime::currentDateTime();
    }
    while(n.secsTo(now)<=3);

    screen.hide();

#ifndef REMOVE_SQL_MODEL
    if(!SQLProecss::instance()->initDatabase("./config/DataBaseInfo.properties"))
    {
        Util::showWarn(SQLProecss::instance()->getLastError());
    }
#endif

    SaveFileSuffix = ".bin";
    SaveFileHeadFlag = "RGVISIO";
    GlobalRotateStep = 5;
    GlobalMinMumSize = 15;

    ActionManager actionManager;
    SplitManager splitManager;
    MainWindow w(settings);
    w.show();
    
    return a.exec();
}
