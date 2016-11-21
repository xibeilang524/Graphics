/*************************************************
**版  权：RenGu Company
**文件名: mywebservice.h
**作  者: wey       Version: 1.0       Date: 2016.11.10
**描  述: 服务调用者
**Others:
**
**修改历史:
**20161118:wey:修复调用服务内存一直增长问题【reply->deleteLater()】
*************************************************/
#ifndef MYWEBSERVICE_H
#define MYWEBSERVICE_H

#include <QObject>

class QNetworkAccessManager;
class QNetworkReply;

class MyWebService : public QObject
{
    Q_OBJECT
public:
    static MyWebService * instance();

    void submit(QString currUrl);

signals:
    void lastUnitProcessOver(bool,QString);

private slots:
    void replyFinshed(QNetworkReply* reply);

private:
    MyWebService();
    static MyWebService * service;

    QString parseResult(QString result, bool &hasFault);

    QNetworkAccessManager * accessManager;          //网络访问
};

#endif // MYWEBSERVICE_H
