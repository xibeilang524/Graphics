#include "mainwindow.h"
#include "myapplication.h"

#include <QTextCodec>
#include <QSplashScreen>
#include <QDesktopWidget>

#include "./manager/actionmanager.h"
#include "SelfWidget/hidesplit.h"
#include "global.h"
#include "sql/sqlproecss.h"
#include "util.h"

int main(int argc, char *argv[])
{    
    MyApplication a(argc, argv);

    ScreenWidth = qApp->desktop()->screenGeometry().width();
    ScreenHeight = qApp->desktop()->screenGeometry().height();

    MyApplication::addLibraryPath("./plugins");

    QTextCodec * codec = QTextCodec::codecForName("GB2312");
    QTextCodec::setCodecForCStrings(codec);

    QPixmap pixmap(":/images/startup.png");
    QSplashScreen screen(pixmap);
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

    if(!SQLProecss::instance()->initDatabase("./config/DataBaseInfo.properties"))
    {
        Util::showWarn(SQLProecss::instance()->getLastError());
    }

    SaveFileSuffix = ".bin";
    SaveFileHeadFlag = "RGVISIO";
    GlobalRotateStep = 5;
    GlobalMinMumSize = 15;

    ActionManager actionManager;
    SplitManager splitManager;
    MainWindow w;
    w.show();
    
    return a.exec();
}
