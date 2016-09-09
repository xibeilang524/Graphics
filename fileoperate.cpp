#include "fileoperate.h"

#include <QGraphicsItem>

#include "./item/myitem.h"
#include "./item/myarrow.h"
#include "./item/mytextitem.h"
#include "global.h"

#include <QFile>
#include <QDataStream>
#include <QDebug>

#include "typeinfo.h"

FileOperate::FileOperate()
{

}

FileOperate * FileOperate::instance()
{
    if(!fileOperate)
    {
        fileOperate = new FileOperate();
    }
    return fileOperate;
}

FileOperate * FileOperate::fileOperate =  NULL;

ReturnType FileOperate::saveFile(QString fileName,const QList<QGraphicsItem *> &items)
{
    if(!fileName.contains(SaveFileSuffix))
    {
        fileName += SaveFileSuffix;
    }

    QFile file(fileName);
    if(!file.open(QFile::ReadWrite))
    {
        return FILE_CANT_WRITE;
    }

    QDataStream stream(&file);

    //写入文件标识头
    stream<<SaveFileHeadFlag;

    foreach(QGraphicsItem * item,items)
    {
        QString itemName = typeid(*item).name();
        if(itemName == typeid(MyItem).name())
        {
            MyItem * myItem = dynamic_cast<MyItem *>(item);
            stream<<myItem;
        }
        else if(itemName == typeid(MyArrow).name())
        {
            MyArrow * myItem = dynamic_cast<MyArrow *>(item);
            stream<<myItem;
        }
        else if(itemName == typeid(MyTextItem).name())
        {
            MyTextItem * myItem = dynamic_cast<MyTextItem *>(item);
            stream<<myItem;
        }
    }
    file.close();
    return RETURN_OK;
}

ReturnType FileOperate::openFile(QString fileName,QList<CutInfo> &items)
{
    QFile file(fileName);
    if(!file.open(QFile::ReadWrite))
    {
        return FILE_CANT_READ;
    }

    QDataStream stream(&file);

    QString fileFlag;
    stream>>fileFlag;
    if(fileFlag != SaveFileHeadFlag)
    {
        file.close();
        return FILE_ILLEGAL;
    }

    while(!stream.atEnd())
    {
        CutInfo info;
        int type;
        stream>>type>>info.itemProperty;
        info.graphicsType = (GraphicsType)type;
        items.push_back(info);
    }
    return RETURN_OK;
}
