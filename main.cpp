#include "mainwindow.h"
#include "myapplication.h"

#include <QTextCodec>
#include <QDesktopWidget>

#include "./manager/actionmanager.h"
#include "SelfWidget/hidesplit.h"
#include "global.h"
#include "sql/sqlproecss.h"
#include "util.h"

int main(int argc, char *argv[])
{
    MyApplication a(argc, argv);

    MyApplication::addLibraryPath("./plugins");

    QTextCodec * codec = QTextCodec::codecForName("GB2312");
    QTextCodec::setCodecForCStrings(codec);

    if(!SQLProecss::instance()->initDatabase("./config/DataBaseInfo.properties"))
    {
        Util::showWarn(SQLProecss::instance()->getLastError());
    }

    ScreenWidth = qApp->desktop()->screenGeometry().width();
    ScreenHeight = qApp->desktop()->screenGeometry().height();

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
