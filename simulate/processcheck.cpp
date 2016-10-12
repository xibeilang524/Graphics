#include "processcheck.h"

#include "../item/mygraphicsview.h"
#include "../item/myscene.h"
#include "../item/myitem.h"
#include "../item/myarrow.h"
#include "../item/draglinepoint.h"

#include <QDebug>
#include <QStack>

ProcessCheck * ProcessCheck::pcheck = NULL;

ProcessCheck::ProcessCheck()
{

}

ProcessCheck * ProcessCheck::instance()
{
    if(!pcheck)
    {
        pcheck = new ProcessCheck();
    }
    return pcheck;
}

/*!
 *【3】:判断整个链路是否完整，同时判断逻辑是否正确；并且将控件按照逻辑顺序重新排列【重要】
 *申明:
 *[1]:箭头是有方向的，按下拖出箭头是起点，松开接入箭头是终点，并不因箭头符号改变而改变。
 *[2]:此版本中只涉及顺序和简单的分支结构(20161011版)
 *!*/
ReturnType ProcessCheck::checkProcess(QList<QGraphicsItem *> &existedItems)
{
    //[1]判断流程是否完整，同时将流程重排列
    QList<MyItem * > resortItems;

    MyItem * startItem;
    MyItem * endItem;

    foreach(QGraphicsItem * item,existedItems)
    {
        if(TYPE_ID(*item) == TYPE_ID(MyItem))
        {
            MyItem * tmpItem = dynamic_cast<MyItem *>(item);
            if(tmpItem && tmpItem->getType() == GRA_ROUND_RECT)
            {
                QString text = tmpItem->getText();
                if(text == "开始" || text.toLower().contains("start"))
                {
                    startItem = tmpItem;
                }
                else if(text == "结束" || text.toLower().contains("end"))
                {
                    endItem = tmpItem;
                }
            }
        }
    }
    bool isAtEnd = false;

    MyItem * currItem = startItem;        //当前操作指针指向控件
    QStack<PolygonDesc *> polygons;       //推演过程中记录所有遇到的判断框

    while(!isAtEnd)
    {
        resortItems.append(currItem);

//        qDebug()<<currItem->getText()<<"______";

        QList<MyArrow *> arrows = currItem->getArrows();

        //开始点
        if(currItem == startItem)
        {
            if(arrows.size() == 0 || arrows.size() > 1)
            {
                return FLOW_SUCCESS;
            }

            currItem = getMyItem(arrows.at(0));
        }
        //结束点
        else if(currItem == endItem)
        {
            if(arrows.size() == 0)
            {
                return FLOW_SUCCESS;
            }

            if(polygons.size() > 0)
            {
                PolygonDesc * topDesc = polygons.at(polygons.size() -1);
                //左侧未处理完成，标记左侧处理完成，然后将处理权交给右控件
                if(topDesc->isProcLeft && !topDesc->isLeftOver)
                {
                    topDesc->isLeftOver = true;
                    topDesc->isProcLeft = false;
                    topDesc->isProcRight = true;

//                    resortItems.removeLast();
                    currItem = topDesc->rightItem;
                }
                //右侧未处理完，标记右控件处理完成
                else if(topDesc->isProcRight && !topDesc->isRightOver)
                {
                    topDesc->isRightOver = true;
                    topDesc->isProcRight = false;
//                    resortItems.removeLast();
                }

                //左右均处理完,如果有父类，则将处理权交给父类，并且将自己从栈中移除
                if(topDesc->isLeftOver && topDesc->isRightOver)
                {
                    PolygonDesc * parent = topDesc->parent;
                    if(parent && !parent->isLeftOver)
                    {
                        parent->isProcLeft = false;
                        parent->isLeftOver = true;
                        parent->isProcRight = true;

//                        resortItems.removeLast();
                        currItem = parent->rightItem;
                    }
                    else if(parent && !parent->isRightOver)
                    {
                        parent->isProcRight = false;
                        parent->isRightOver = true;
//                        resortItems.removeLast();
                    }
                    polygons.pop();

                    //如果父类左右均处理完后，整个流程结束
                    if(parent && parent->isLeftOver && parent->isRightOver)
                    {
                        isAtEnd = true;
                        polygons.pop();
                    }
                }
            }
            else
            {
                isAtEnd = true;
            }
        }
        else
        {
            GraphicsType gtype = currItem->getType();

            int inNum = 0,outNum = 0;               //当前控件分别作为起点和终点引出直线的个数
            getItemInOutNum(currItem,arrows,inNum,outNum);

            //输入输出框/处理框[关注箭头指向点]
            if(gtype == GRA_PARALLELOGRAM || gtype == GRA_RECT)
            {
                if(inNum !=1 || outNum != 1)
                {
                    return FLOW_SUCCESS;
                }

                //当前控件的下一个节点
                QList<MyItem *> outItems = getInOutItems(currItem,arrows,true);

                if(outItems.size() == 1)
                {
                    currItem = outItems.at(0);
                }
            }
            //判断[1个输入，2个输出]
            else if(gtype == GRA_POLYGON)
            {
                if(inNum !=2 || outNum != 1)
                {
                    return FLOW_SUCCESS;
                }

                //当前控件的下一个节点
                QList<MyItem *> outItems = getInOutItems(currItem,arrows,true);

                PolygonDesc * desc = new PolygonDesc;
                desc->currItem = currItem;

                //当再遇到判断框时，需要建立新判断框和栈顶层判断框的关系
                if(polygons.size() > 0)
                {
                    PolygonDesc * currDesc = polygons.at(polygons.size() - 1);
                    desc->parent = currDesc;
                }

                polygons.push(desc);

                //[自动处理第一个节点]
                desc->isProcLeft = true;
                desc->leftItem = outItems.at(0);
                desc->rightItem = outItems.at(1);
                currItem = outItems.at(0);
            }
        }
    }

    foreach(MyItem * item,resortItems)
    {
        qDebug()<<item->getText();
    }

    //[2]验证逻辑是否正确
    return RETURN_SUCCESS;
}

//获取某个控件作为箭头起点和终点个数
void ProcessCheck::getItemInOutNum(MyItem *currItem, QList<MyArrow *> &arrows, int &inputNum, int &outputNum)
{
    QString currItemId = currItem->getProperty().startItemID;

    foreach(MyArrow * arrow,arrows)
    {
        MyItem * startItem = getMyItem(arrow,false);
        MyItem * endItem = getMyItem(arrow,true);
        QString startItemId = startItem->getProperty().startItemID;
        QString endItemId = endItem->getProperty().startItemID;

        if(startItemId == currItemId)
        {
            inputNum++;
        }
        else if(endItemId == currItemId)
        {
            outputNum++;
        }
    }
}

//获取控件上作为箭头起点或终点控件集合
QList<MyItem *> ProcessCheck::getInOutItems(MyItem *currItem,QList<MyArrow *> &arrows, bool isEnd)
{
    QList<MyItem *> resultList;

    foreach (MyArrow * arrow, arrows)
    {
       MyItem * item = getMyItem(arrow,isEnd);
       if(item != currItem)
       {
           resultList.append(item);
       }
    }
    return resultList;
}

//获取箭头起始点/终点Item
MyItem * ProcessCheck::getMyItem(MyArrow * arrow, bool isEnd)
{
    if(arrow->getLineType() == LINE_MYITEM)
    {
        DragLinePoint * dragLine = NULL;
        if(isEnd)
        {
            dragLine = dynamic_cast<DragLinePoint *>(arrow->getEndItem());
        }
        else
        {
            dragLine = dynamic_cast<DragLinePoint *>(arrow->getStartItem());
        }

        if(dragLine)
        {
            MyItem * item = dynamic_cast<MyItem *>(dragLine->getParentItem());
            return item;
        }
    }
    return NULL;
}

