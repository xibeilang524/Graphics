#include "fileoperate.h"

#include <QGraphicsItem>

#include "./item/myitem.h"
#include "./item/myarrow.h"
#include "./item/mytextitem.h"
#include "./item/mynodeport.h"
#include "global.h"

#include <QFile>
#include <QDataStream>
#include <QDebug>

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
        QString itemName = TYPE_ID(*item);

        if(itemName == TYPE_ID(MyItem))
        {
            MyItem * myItem = dynamic_cast<MyItem *>(item);
            stream<<myItem;
        }
        else if(itemName == TYPE_ID(MyArrow))
        {
            MyArrow * myItem = dynamic_cast<MyArrow *>(item);
            stream<<myItem;
        }
        else if(itemName == TYPE_ID(MyTextItem))
        {
            MyTextItem * myItem = dynamic_cast<MyTextItem *>(item);
            stream<<myItem;
        }
        else if(itemName == TYPE_ID(MyNodePort))
        {
            MyNodePort * myItem = dynamic_cast<MyNodePort *>(item);
            stream<<myItem;
        }
    }
    file.close();
    return RETURN_OK;
}

/**
 *本地解析文件,因端口NodePort的属性NodePortProperty是单独保存，需要依照类型来判断读取
*读取完成后，直接将端口进行装配至每个Item中,使得调用层无需知晓具体的细节部分
*同时记录加载文件中最大的Zvalue，再新添加item时，会置于所有层的顶部
*/
ReturnType FileOperate::openFile(QString fileName,QList<CutInfo *> &items)
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

    QList<NodePortProperty > localNodePorts;

    qreal maxZvalue = 0;

    //文件分类读取
    while(!stream.atEnd())
    {
        int type;
        stream>>type;

        GraphicsType gtype = (GraphicsType)type;

        if(gtype == GRA_NODE_PORT)
        {
            NodePortProperty nodeProperty;
            stream>>nodeProperty;
            localNodePorts.push_back(nodeProperty);
        }
        else
        {
            CutInfo * info = new CutInfo;
            stream>>info->itemProperty;
            info->graphicsType = (GraphicsType)type;

            if(info->itemProperty.zValue > maxZvalue)
            {
                maxZvalue = info->itemProperty.zValue;
            }

            items.push_back(info);
        }
    }

    //【记录打开文件中最大的深度】
    GlobalItemZValue = maxZvalue;

    //端口装配
    if(localNodePorts.size() > 0)
    {
        for(int i = 0;i<localNodePorts.size();i++)
        {
            for(int j = 0; j < items.size() ;j++)
            {
                if(items.at(j)->itemProperty.startItemID == localNodePorts.at(i).parentItemID)
                {

                    NodePortProperty prop = localNodePorts.at(i);
                    items.at(j)->nodeProperties.push_back(prop);
                }
            }
        }
    }

    return RETURN_OK;
}
