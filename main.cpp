#include "mainwindow.h"
#include <QApplication>

#include <QTextCodec>
#include <QDesktopWidget>

#include <QDebug>

#include "./manager/actionmanager.h"
#include "SelfWidget/hidesplit.h"
#include "global.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QApplication::addLibraryPath("./plugins");

    QTextCodec * codec = QTextCodec::codecForName("GB2312");
    QTextCodec::setCodecForCStrings(codec);

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
