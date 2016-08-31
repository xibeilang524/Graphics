#include "mainwindow.h"
#include <QApplication>

#include <QTextCodec>

#include "actionmanager.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTextCodec * codec = QTextCodec::codecForName("GB2312");
    QTextCodec::setCodecForCStrings(codec);

    ActionManager actionManager;

    MainWindow w;
    w.show();
    
    return a.exec();
}
