/*************************************************
**版  权：RenGu Company
**文件名: myapplication.h
**作  者: wey       Version: 1.0       Date: 2016.10.25
**描  述:
**Others: 自定义运行实例
**
**修改历史:
**20161025:wey:增加全局键盘捕获，针对Alt+左右键切换
*************************************************/
#ifndef MYAPPLICATION_H
#define MYAPPLICATION_H

#include <QApplication>

class MainWindow;

class MyApplication : public QApplication
{
    Q_OBJECT
public:
    MyApplication(int & argc, char ** argv );

    bool notify(QObject *obj, QEvent *event);

};

#endif // MYAPPLICATION_H
