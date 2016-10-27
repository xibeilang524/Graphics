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
 *【4】:从头开始提取控件，将其转换成处理单元，压入处理队列【重要】
 *申明:
 *[1]:箭头是有方向的，按下拖出箭头是起点，松开接入箭头是终点，并不因箭头符号改变而改变。
 *[2]:此版本中只涉及顺序和简单的分支结构(20161011版)
 *[3]:在处理判断框时，默认先处理yes，后处理no分支。
 *!*/
ReturnType ProcessCheck::checkProcess(QList<QGraphicsItem *> &existedItems,QList<MyItem *> &resortedItems,QList<ProcessUnit *> & procUnits)
{
    //[1]判断流程是否完整，同时将流程重排列
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

    ProcessUnit * frontUnit = NULL;

    ProcessUnit * startUnit = NULL;
    ProcessUnit * endUnit = NULL;

    while(!isAtEnd)
    {
        if(!currItem)
        {
            return RETURN_ERROR;
        }
        resortedItems.append(currItem);

        QList<MyArrow *> arrows = currItem->getArrows();

        //开始点
        if(currItem == startItem)
        {
            if(arrows.size() == 0 || arrows.size() > 1)
            {
                return FLOW_ERROR;
            }

            currItem = getMyItem(arrows.at(0));

            ProcessUnit  * unit = new ProcessUnit;
            unit->gtype = startItem->getType();
            unit->ptype = PRO_START;
            unit->item =  startItem;

            startUnit = unit;
            frontUnit = unit;
            procUnits.push_back(unit);
        }
        //结束点
        else if(currItem == endItem)
        {
            if(arrows.size() == 0)
            {
                return FLOW_ERROR;
            }

            if(!endUnit)
            {
                ProcessUnit  * unit = new ProcessUnit;
                unit->gtype = endItem->getType();
                unit->item =  endItem;
                unit->ptype = PRO_END;

                endUnit = unit;
            }

            if(polygons.size() > 0 && frontUnit && frontUnit->ptype == PRO_JUDGE)
            {
                PolygonDesc * currDesc = polygons.at(polygons.size() - 1);

                if(currDesc->isProcLeft)
                {
                    frontUnit->yesChild = endUnit;
                }
                else if(currDesc->isProcRight)
                {
                    frontUnit->noChild = endUnit;
                }
            }
            else
            {
                frontUnit->nextChild = endUnit;
            }

            if(polygons.size() > 0)
            {
                //左侧未处理完成，标记左侧处理完成，然后将处理权交给右控件
                while(polygons.size() > 0 )
                {
                    PolygonDesc * topDesc = polygons.at(polygons.size() -1);
                    PolygonDesc * parent = NULL;
                    //设置自身的状态
                    if(topDesc->isProcLeft && !topDesc->isLeftOver)
                    {
                        topDesc->isLeftOver = true;
                        topDesc->isProcLeft = false;
                        topDesc->isProcRight = true;

                        currItem = topDesc->rightItem;
                        frontUnit = topDesc->processUnit;

                        break;
                    }
                    //右侧未处理完，标记右控件处理完成
                    else if(topDesc->isProcRight && !topDesc->isRightOver)
                    {
                        topDesc->isRightOver = true;
                        topDesc->isProcRight = false;
                    }

                    if(topDesc->isLeftOver && topDesc->isLeftOver)
                    {
                        parent = topDesc->parent;
                        delete topDesc;
                        polygons.pop();             //弹出自身
                    }

                    //设置父控件的状态
                    if(parent && !parent->isLeftOver)
                    {
                        parent->isProcLeft = false;
                        parent->isLeftOver = true;
                        parent->isProcRight = true;

                        currItem = parent->rightItem;

                        frontUnit = parent->processUnit;
                        break;
                    }
                    else if(parent && !parent->isRightOver)
                    {
                        parent->isProcRight = false;
                        parent->isRightOver = true;
                    }
                }

                if(polygons.size() == 0)
                {
                    isAtEnd = true;
                    procUnits.push_back(endUnit);
                }
            }
            else
            {
                isAtEnd = true;
                procUnits.push_back(endUnit);
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
                    return FLOW_ERROR;
                }

                //当前控件的下一个节点
                QList<MyItem *> outItems = getInOutItems(currItem,arrows,true);

                if(outItems.size() == 1)
                {
                    ProcessUnit  * unit = new ProcessUnit;
                    unit->gtype = gtype;
                    unit->item = currItem;

                    ProcessType  ptype;

                    switch (gtype)
                    {
                        case GRA_RECT:
                                       ptype = PRO_PROCESS;
                                       break;
                        case GRA_PARALLELOGRAM:
                                       ptype = PRO_INPUT;
                                       break;
                        case GRA_LOOP_UP:
                                       ptype = PRO_LOOP_UP;
                                       break;
                        case GRA_LOOP_DOWN:
                                       ptype = PRO_LOOP_DOWN;
                                       break;
                        case GAR_PARALLE:
                                       ptype = PRO_PARALLEL;
                                       break;
                        default:
                            break;
                    }

                    unit->ptype = ptype;

                    //紧跟着判断框，需要将将此控件设置到判断框的左右分支上
                    if(polygons.size() > 0 && frontUnit && frontUnit->ptype == PRO_JUDGE)
                    {
                        PolygonDesc * currDesc = polygons.at(polygons.size() - 1);

                        if(currDesc->isProcLeft)
                        {
                            frontUnit->yesChild = unit;
                        }
                        else if(currDesc->isProcRight)
                        {
                            frontUnit->noChild = unit;
                        }
                    }
                    else
                    {
                        frontUnit->nextChild = unit;
                    }

                    frontUnit = unit;

                    procUnits.push_back(unit);

                    currItem = outItems.at(0);
                }
            }
            //判断[1个输入，2个输出]
            else if(gtype == GRA_POLYGON)
            {
                if(inNum !=2 || outNum != 1)
                {
                    return FLOW_ERROR;
                }

                PolygonDesc * desc = new PolygonDesc;
                desc->currItem = currItem;

                ProcessUnit  * unit = new ProcessUnit;
                unit->gtype = gtype;
                unit->item = currItem;
                unit->ptype = PRO_JUDGE;

                if(polygons.size() > 0 && frontUnit && frontUnit->ptype == PRO_JUDGE)
                {
                    PolygonDesc * currDesc = polygons.at(polygons.size() - 1);

                    if(currDesc->isProcLeft)
                    {
                        frontUnit->yesChild = unit;
                    }
                    else if(currDesc->isProcRight)
                    {
                        frontUnit->noChild = unit;
                    }
                }
                else
                {
                    frontUnit->nextChild = unit;
                }

                frontUnit = unit;

                procUnits.push_back(unit);

                desc->processUnit = unit;

                //当再遇到判断框时，需要建立新判断框和栈顶层判断框的关系
                if(polygons.size() > 0)
                {
                    PolygonDesc * currDesc = polygons.at(polygons.size() - 1);
                    desc->parent = currDesc;
                }

                polygons.push(desc);

                //当前控件的下一个节点
//                QList<MyItem *> outItems = getInOutItems(currItem,arrows,true);

                //分别获取条件成立和不成立的item，将条件成立item赋值left，不成立item赋值right
                MyItem * yesItem = getConditionItem(currItem,arrows,true);
                MyItem * noItem = getConditionItem(currItem,arrows,false);

                desc->isProcLeft = true;
                desc->leftItem = yesItem;
                desc->rightItem = noItem;
                currItem = yesItem;
            }
        }
    }
    //[2]验证逻辑是否正确
    return RETURN_SUCCESS;
}

//分别获取条件成立和不成立的item
MyItem * ProcessCheck::getConditionItem(MyItem * currItem,QList<MyArrow *> &arrows, bool isYes)
{
    foreach(MyArrow * arrow,arrows)
    {
        MyItem * item = getMyItem(arrow,true);
        if(item != currItem && isYes && (arrow->getText().toLower()== "yes" || arrow->getText() == "是"))
        {
            return item;
        }
        else if(item != currItem && !isYes && (arrow->getText().toLower()== "no" || arrow->getText() == "否"))
        {
            return item;
        }
    }
    return NULL;
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

