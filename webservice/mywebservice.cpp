#include "mywebservice.h"

#include <QNetworkRequest>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QTextCodec>
#include <QDomDocument>
#include <QStringList>
#include <QDebug>

MyWebService * MyWebService::service = NULL;

MyWebService * MyWebService::instance()
{
    if(!service)
    {
        service = new MyWebService();
    }
    return service;
}

MyWebService::MyWebService()
{
    accessManager = new QNetworkAccessManager;
    connect(accessManager,SIGNAL(finished(QNetworkReply*)),this,SLOT(replyFinshed(QNetworkReply*)));
}

//提交服务
void MyWebService::submit(QString currUrl)
{
    QNetworkRequest url;
    url.setHeader(QNetworkRequest::ContentTypeHeader,"application/x-www-form-urlencoded");
    url.setUrl(QUrl(currUrl));
    QString b("");

    qDebug()<<currUrl;

    accessManager->post(url,b.toAscii());
}

//获取访问返回信息,200是正确的返回码
void MyWebService::replyFinshed(QNetworkReply *reply)
{
    QVariant code = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
    QTextCodec * codec = QTextCodec::codecForName("utf8");
    QString replyText = codec->toUnicode(reply->readAll());

    QMap<QString,QString> result;

    if(code.toInt() != 200)
    {
        result.insert("result","执行服务失败!");
        emit lastUnitProcessOver(true,result);
    }
    else
    {
        bool hasFault = true;
        result = parseResult(replyText,hasFault);
        emit lastUnitProcessOver(hasFault,result);
    }
    reply->deleteLater();
}

//【20161130】目前服务的输出结果格式存在多种情况，但基本还是满足在有二维数组时，第1行表示变量名，第二行表示变量值
//此时也会出现变量名的集合和变量值的集合不一致，如果变量值集合的长度小于变量名集合长度，变量名对应不存在部分需要填空
QMap<QString,QString> MyWebService::parseResult(QString result,bool & hasFault)
{
    QDomDocument doc;
    doc.setContent(result);

    hasFault = true;

    QMap<QString,QString> results;

    //是否存在错误
    QDomNodeList nodeList = doc.elementsByTagName("faultstring");

    if(nodeList.size() > 0)
    {
        QDomNode node = nodeList.at(0);
        results.insert("result",node.toElement().text());
        return results;
    }

    nodeList = doc.elementsByTagName("ns:return");

    //输出参数个数为1，将其转换为二维数组
    if(nodeList.size() == 1)
    {
        hasFault = false;
        QDomNode node = nodeList.at(0);
        results.insert("result",node.toElement().text());
        return results;
    }
    //输出参数为2维数组,对2维数组进行解析
    else if(nodeList.size() == 2)
    {
        hasFault = false;

        QDomNodeList keyList = nodeList.at(0).toElement().elementsByTagName("ns:array");
        QDomNodeList valueList = nodeList.at(1).toElement().elementsByTagName("ns:array");

        /*<ns:return>
          <ns:array>结果</ns:array><ns:array>发现目标</ns:array><ns:array>飞机纬度</ns:array>
          <ns:array>飞机经度</ns:array><ns:array>飞机高度</ns:array><ns:array>飞机数量</ns:array>
          </ns:return>
        */
        if(keyList.size() > 1)
        {
            for(int i = 0; i < keyList.size(); i++)
            {
                QString key = keyList.at(i).toElement().text();
                QString value = "";
                if(i < valueList.size())
                {
                    value = valueList.at(i).toElement().text();
                }

                results.insert(key,value);
            }
        }
        //输出变量名格式：<ns:array>纬度，经度，海拔，速度，架数</ns:array>
        else if(keyList.size() == 1 && valueList.size() > 0)
        {
           QString varName =  keyList.at(0).toElement().text();
           QStringList varList = varName.split(",");
//           for(int i=0;i<valueList.size();i++)
//           {
//               qDebug()<<"var:"<<varList.at(i);
//           }

//           for(int i=0;i<valueList.size();i++){
//               qDebug()<<"val:"<<valueList.at(i).toElement().text();
//           }
           if(varList.size() == valueList.size())
           {
               for(int i = 0; i < varList.size(); i++)
               {
                   QString value = valueList.at(i).toElement().text();
                   results.insert(varList.at(i),value);
               }
           }
        }
    }

    return results;
}
