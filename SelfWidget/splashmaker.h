/*************************************************
**版  权：RenGu Company
**文件名: SplashMaker.h
**作  者: wey       Version: 1.0       Date: 2017.03.31
**描  述:
**Others:设置启动页面信息
**
**修改历史:
**
*************************************************/
#ifndef SPLASHMAKER_H
#define SPLASHMAKER_H

#include <QDialog>
#include <QList>
#include <QModelIndex>

class  MySettings;
struct SplashDesc;

namespace Ui {
class SplashMaker;
}

class SplashMaker : public QDialog
{
    Q_OBJECT
    
public:
    static SplashMaker * instance();

private slots:
    void execMe();
    void addContent();
    void removeContent();
    void modifyCurrText(QModelIndex index);

private:
    Ui::SplashMaker *ui;

    void readSettings();
    void writeSettings();

    SplashMaker(QWidget *parent = 0);
    ~SplashMaker();

    static SplashMaker * splashMaker;

    MySettings * settings;

    QList<SplashDesc *> splashlist;
};

#endif // SPLASHMAKER_H
