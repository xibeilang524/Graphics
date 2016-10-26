#include "properties.h"

#include <QFile>
#include <QTextStream>
#include <QMessageBox>

#include "util.h"

Properties::Properties(QString fileName):
    fileName(fileName)
{
    parseFile();
}

void Properties::parseFile()
{
    QFile file(fileName);
    if(!file.exists())
    {
        Util::showWarn(fileName+" 文件不存在,请重新确认!");
        return;
    }
    if(!file.open(QIODevice::ReadOnly))
    {
        Util::showWarn(fileName+" 文件不能读!");
        return;
    }
    QTextStream stream(&file);
    QString temp;
    int index;
    while(!stream.atEnd())
    {
       temp = stream.readLine();
       if((index=temp.indexOf(QRegExp("/")))>0)   //如果包含注释，则忽略注释内容
       {
            temp = temp.left(index);
       }
       QStringList list = temp.split("=");
       temp = list.at(1);         //尚未进行空格处理
       maps.insertMulti(list.at(0),temp);
    }
}

QString Properties::getValue(QString key)
{
    if(maps.contains(key))
    {
         return maps.value(key);
    }
    else
    {
         return "";
    }
}
