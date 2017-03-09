#include "fileoperate.h"

#include <QGraphicsItem>

#include "./item/myitem.h"
#include "./item/myarrow.h"
#include "./item/mytextitem.h"
#include "./item/mynodeport.h"
#include "./item/mypathitem.h"
#include "util.h"
#include "global.h"

#include <QFile>
#include <QDataStream>

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

ReturnType FileOperate::saveFile(QString & fileName,const QList<QGraphicsItem *> &items)
{
    QRegExp exp("_v(\\d)+");

    if(!fileName.contains(exp))
    {
        //包含后缀名，但不包含版本号
        int hasSuffix = fileName.indexOf(SaveFileSuffix);
        if(hasSuffix > 0)
        {
            fileName = fileName.left(hasSuffix);
        }

        fileName += QString("_v"+QString::number(M_VERTION));
    }

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
    stream<<M_VERTION;
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
        else if(itemName == TYPE_ID(MyPathItem))
        {
            MyPathItem * myItem = dynamic_cast<MyPathItem *>(item);
            stream<<myItem;
        }
        else if(itemName == TYPE_ID(MyTextItem))
        {
            MyTextItem * myItem = dynamic_cast<MyTextItem *>(item);
            if(myItem->getTextExistType() == TEXT_ALONG)
            {
                stream<<myItem;
            }
        }
        else if(itemName == TYPE_ID(MyNodePort))
        {
            MyNodePort * myItem = dynamic_cast<MyNodePort *>(item);
            stream<<myItem;
        }
    }
    file.close();
    return RETURN_SUCCESS;
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

    QDataStream stream;
    stream.setDevice(&file);

    ReturnType  rtype = Util::isIllegalFile(stream);

    if(rtype != RETURN_SUCCESS)
    {
        file.close();
        return rtype;
    }

    QList<NodeWholeProperty > localNodePorts;

    qreal maxZvalue = 0;

    //文件分类读取
    while(!stream.atEnd())
    {
        int type;
        stream>>type;

        GraphicsType gtype = (GraphicsType)type;

        //item上的端口信息
        if(gtype == GRA_NODE_PORT ||gtype == GRA_NODE_HALF_CIRCLE_IN ||gtype == GRA_NODE_HALF_CIRCLE_OUT
                ||gtype == GRA_NODE_TRIANGLE_OUT ||gtype == GRA_NODE_TRIANGLE_IN ||gtype == GRA_NODE_CIRCLE)
        {
            NodeWholeProperty nodeProperty;
            nodeProperty.nodeProp.portType = (GraphicsType)type;
            stream>>nodeProperty.nodeProp;
#ifdef ADD_STATE_MODEL
            switch(gtype)
            {
                case GRA_NODE_CIRCLE:
                                    stream>>nodeProperty.stateProp;
                                    break;

                case GRA_NODE_TRIANGLE_IN:
                case GRA_NODE_TRIANGLE_OUT:
                case GRA_NODE_HALF_CIRCLE_IN:
                case GRA_NODE_HALF_CIRCLE_OUT:
                                    stream>>nodeProperty.inOutProp;
                                    break;
            }
#endif
            localNodePorts.push_back(nodeProperty);
        }
        else
        {
            CutInfo * info = new CutInfo;
            stream>>info->itemProperty;

            //折线
            if(gtype == GRA_VECTOR_LINE)
            {
                int pointSize = 0;
                stream>>pointSize;
                for(int i = 0; i < pointSize; i++)
                {
                    QPointF point;
                    stream>>point;
                    info->pathPoints.push_back(point);
                }
#ifdef ADD_STATE_MODEL
                    stream>>info->linkedProp;
#endif
            }
            else
            {
#ifdef ADD_STATE_MODEL
                if(gtype == GRA_STATE_START)
                {
                    stream>>info->stateStartProp;
                }
                else if(gtype == GRA_STATE_PROCESS)
                {
                    stream>>info->stateModelProp;
                }
                else if(gtype == GRA_LINE)
                {
                    stream>>info->linkedProp;
                }
#endif
                if(info->itemProperty.ptype == PRO_PROCESS)
                {
                    stream>>(&info->serviceProp);
                }
                else if(info->itemProperty.ptype == PRO_LOOP)
                {
                    stream>>(info->loopProp);
                }
                else if(info->itemProperty.ptype == PRO_JUDGE || info->itemProperty.ptype == PRO_INPUT)
                {
                    stream>>(info->judgeProp);
                }
            }

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
                if(items.at(j)->itemProperty.startItemID == localNodePorts.at(i).nodeProp.parentItemID)
                {
                    NodeWholeProperty prop = localNodePorts.at(i);
                    items.at(j)->nodeProperties.push_back(prop);
                }
            }
        }
    }

    return RETURN_SUCCESS;
}

//保存服务至本地磁盘
ReturnType FileOperate::saveServiceToFile(QString &fileName, bool isPreModel)
{
    QRegExp exp("_v(\\d)+");

    if(!fileName.contains(exp))
    {
        //包含后缀名，但不包含版本号
        int hasSuffix = fileName.indexOf(SaveFileSuffix);
        if(hasSuffix > 0)
        {
            fileName = fileName.left(hasSuffix);
        }

        fileName += QString("_v"+QString::number(M_VERTION));
    }

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
    stream<<M_VERTION;
    stream<<SaveFileHeadFlag;

    if(isPreModel)
    {
        for(int i=0;i<PreExeServices.size();i++)
        {
            stream<<PreExeServices.at(i);
        }
    }
    else
    {
        for(int i=0;i<PreExeServices.size();i++)
        {
            stream<<ResetExeServices.at(i);
        }
    }

    file.close();
    return RETURN_SUCCESS;
}

//从本地加载服务
ReturnType FileOperate::loadServiceFromFile(QString &fileName, bool isPreModel)
{
    QFile file(fileName);
    if(!file.open(QFile::ReadWrite))
    {
        return FILE_CANT_READ;
    }

    QDataStream stream;
    stream.setDevice(&file);

    ReturnType  rtype = Util::isIllegalFile(stream);

    if(rtype != RETURN_SUCCESS)
    {
        file.close();
        return rtype;
    }

    while(!stream.atEnd())
    {
        ServiceProperty * prop = new ServiceProperty;
        stream>>prop;
        if(isPreModel)
        {
           PreExeServices.append(prop);
        }
        else
        {
            ResetExeServices.append(prop);
        }
    }

    file.close();
    return RETURN_SUCCESS;

}
