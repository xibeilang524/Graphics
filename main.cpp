#include "mainwindow.h"
#include <QApplication>

#include <QTextCodec>

#include "actionmanager.h"
#include "SelfWidget/hidesplit.h"
#include "global.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTextCodec * codec = QTextCodec::codecForName("GB2312");
    QTextCodec::setCodecForCStrings(codec);

    SaveFileSuffix = ".bin";
    SaveFileHeadFlag = "RGVISIO";

    ActionManager actionManager;
    SplitManager splitManager;
    MainWindow w;
    w.show();
    
    return a.exec();
}
