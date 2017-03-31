/*************************************************
**版  权：RenGu Company
**文件名: mysplashscreen.h
**作  者: wey       Version: 1.0       Date: 2017.03.30
**描  述: 开机启动画面
**Others:
**
**修改历史:
**20170330:wey:支持设置多组标签信息
*************************************************/
#ifndef MYSPLASHSCREEN_H
#define MYSPLASHSCREEN_H

#include <QSplashScreen>
#include <QList>
#include <QApplication>

class QLabel;
class MySettings;

struct SplashDesc;

class MySplashScreen : public QSplashScreen
{
    Q_OBJECT
public:
    explicit MySplashScreen(const QPixmap & pixmap = QPixmap(), Qt::WindowFlags f = 0 );
    ~MySplashScreen();

    void setSettings(MySettings * settings);
    void setStartText(QString text);

private:
    void setValue();

    MySettings * settings;

    QList<SplashDesc *> splashDescs;
};

#endif // MYSPLASHSCREEN_H
