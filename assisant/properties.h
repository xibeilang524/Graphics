/*************************************************
**版  权：RenGu Company
**文件名: initdatamanager.h
**作  者: wey       Version: 1.0       Date: 2016.07.09
**描  述:解析.properties类型的配置文件
**Others:
**      1.可以返回解析到的键-值对
*       2.可以通过键的访问或对应的值
**修改历史:
**
*************************************************/
#ifndef PROPERTIES_H
#define PROPERTIES_H

#include <QMap>
#include <QString>

class Properties
{
public:
    Properties(QString fileName);
    QString getValue(QString key);

private:
    void parseFile();                         //从本地解析文件，放入到map中

    QString fileName;
    QMap<QString , QString> maps;
};

#endif // PROPERTIES_H
