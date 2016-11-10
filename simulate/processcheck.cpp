#include "processcheck.h"

#include "../item/mygraphicsview.h"
#include "../item/myscene.h"
#include "../item/myitem.h"
#include "../item/myarrow.h"
#include "../item/draglinepoint.h"
#include "simulateutil.h"

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
 *[4]:
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
    QStack<PolygonDesc *> loopPolygons;   //推演过程中遇到的循环判断框
    QStack<ProcessUnit *> doWhileRects;   //推演过程中遇到RECT为多个的进，一个出认定为dowhile循环

    ProcessUnit * frontUnit = NULL;       //前一个控制单元

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

//        qDebug()<< currItem->getText();

        //开始点
        if(currItem == startItem)
        {
            if(arrows.size() == 0 || arrows.size() > 1)
            {
                return FLOW_ERROR;
            }

            currItem = SimulateUtil::instance()->getMyItem(arrows.at(0));

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

            if(polygons.size() > 0 && frontUnit)
            {
                if(frontUnit->ptype == PRO_JUDGE)
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
                else if(frontUnit->ptype == PRO_LOOP)
                {
                    if(loopPolygons.size() > 0)
                    {
                        PolygonDesc * currDesc = loopPolygons.at(loopPolygons.size() - 1);
                        if(currDesc->isProcLeft)
                        {
                            frontUnit->yesChild = endUnit;
                        }
                        else if(currDesc->isProcRight)
                        {
                            frontUnit->noChild = endUnit;
                        }
                    }
                }
                else
                {
                    frontUnit->nextChild = endUnit;
                }

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
            }
            else if(loopPolygons.size() >0 && frontUnit)
            {
                if(frontUnit->ptype == PRO_LOOP)
                {
                    PolygonDesc * currDesc = loopPolygons.at(loopPolygons.size() - 1);

                    if(currDesc->isProcLeft)
                    {
                        currDesc->isProcLeft = false;
                        frontUnit->yesChild = endUnit;
                        currDesc->isLeftOver = true;
                        currDesc->isProcRight = true;
                    }
                    else if(currDesc->isProcRight)
                    {
                        frontUnit->noChild = endUnit;
                        currDesc->isRightOver = true;
                    }
                }
                else
                {
                    frontUnit->nextChild = endUnit;
                }

                //左侧未处理完成，标记左侧处理完成，然后将处理权交给右控件
                while(loopPolygons.size() > 0 )
                {
                    PolygonDesc * topDesc = loopPolygons.at(loopPolygons.size() -1);
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
                        loopPolygons.pop();             //弹出自身
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
            }
            else
            {
                frontUnit->nextChild = endUnit;
            }

            if(polygons.size() == 0 && loopPolygons.size() == 0)
            {
                isAtEnd = true;
                procUnits.push_back(endUnit);
            }
        }
        else
        {
            GraphicsType gtype = currItem->getType();

            int inNum = 0,outNum = 0;               //当前控件分别作为起点和终点引出直线的个数
            SimulateUtil::instance()->getItemInOutNum(currItem,arrows,inNum,outNum);

            //输入输出框/处理框[关注箭头指向点]
            if(gtype == GRA_PARALLELOGRAM || gtype == GRA_RECT)
            {
                if(inNum !=1 || outNum < 1)
                {
                    return FLOW_ERROR;
                }

                //当前控件的下一个节点
                QList<MyItem *> outItems = SimulateUtil::instance()->getInOutItems(currItem,arrows,true);

                if(outItems.size() == 1)
                {
                    ProcessType  ptype;

                    switch (gtype)
                    {
                        case GRA_RECT:
                                        ptype = PRO_PROCESS;
                                        break;
                        case GRA_PARALLELOGRAM:
                                       ptype = PRO_INPUT;
                                       break;
                                       break;
                        case GAR_PARALLE:
                                       ptype = PRO_PARALLEL;
                                       break;
                        default:
                            break;
                    }

                    ProcessUnit  * unit = new ProcessUnit;
                    unit->gtype = gtype;
                    unit->item = currItem;
                    unit->ptype = ptype;

                    bool isDoWhile = false;

                    //遇到处理框，并且有多个输入，则暂时认定为循环处理的起点
                    if(ptype == PRO_PROCESS && outNum > 1)
                    {
                        bool isExisted = false;
                        foreach (ProcessUnit * punit, doWhileRects)
                        {
                            if(punit->item == currItem)
                            {
                                isExisted = true;
                                unit = punit;
                                break;
                            }
                        }

                        if(!isExisted)
                        {
                            doWhileRects.append(unit);
                        }

                        //判断框在yes分支
                        if(frontUnit->ptype == PRO_LOOP && isExisted &&loopPolygons.size()>0)
                        {
                            PolygonDesc * topLoop = loopPolygons.at(loopPolygons.size()-1);
                            if(topLoop->isProcLeft)
                            {
                                topLoop->isProcLeft = false;
                                topLoop->isLeftOver = true;
                                topLoop->isProcRight = true;

                                topLoop->processUnit->yesChild = unit;
                                currItem = topLoop->rightItem;
                                isDoWhile = true;
                            }
                        }
                    }

                    if(!isDoWhile)
                    {
                        //紧跟着判断框，需要将将此控件设置到判断框的左右分支上
                        if(frontUnit->gtype == GRA_POLYGON)
                        {
                            PolygonDesc * currDesc = NULL;
                            if(frontUnit->ptype == PRO_JUDGE && polygons.size() > 0)
                            {
                                currDesc = polygons.at(polygons.size() - 1);
                            }
                            else if(frontUnit->ptype == PRO_LOOP && loopPolygons.size() >0)
                            {
                                currDesc = loopPolygons.at(loopPolygons.size() - 1);
                            }

                            if(currDesc)
                            {
                                if(currDesc->isProcLeft)
                                {
                                    frontUnit->yesChild = unit;
                                }
                                else if(currDesc->isProcRight)
                                {
                                    frontUnit->noChild = unit;
                                }
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
            }
            //[1个输入(判断)/2个输入(循环)，2个输出]
            //在作为dowhile循环时，可能会出现与if条件功能重合现象，因此要先一步判断【是】的分支是否指向前面产生的doWhileRects
            else if(gtype == GRA_POLYGON)
            {
                if(inNum !=2 || outNum < 1)
                {
                    return FLOW_ERROR;
                }

                //分别获取条件成立和不成立的item，将条件成立item赋值left，不成立item赋值right
                MyItem * yesItem = SimulateUtil::instance()->getConditionItem(currItem,arrows,true);
                MyItem * noItem = SimulateUtil::instance()->getConditionItem(currItem,arrows,false);

                if(yesItem == NULL || noItem == NULL)
                {
                    return FLOW_ERROR;
                }

                bool isYesItemProcced = false;
                ProcessUnit * yesProcessUnit = NULL;

                //如果yes分支指向的rect在之前的doWhileRects存在，避免无限循环
                foreach(ProcessUnit * unit,doWhileRects)
                {
                    if(unit->item == yesItem)
                    {
                        isYesItemProcced = true;
                        yesProcessUnit = unit;
                        break;
                    }
                }

                //判断处理
                if(!isYesItemProcced && outNum == 1)
                {
                    PolygonDesc * desc = new PolygonDesc;
                    desc->currItem = currItem;

                    ProcessUnit  * unit = new ProcessUnit;
                    unit->gtype = gtype;
                    unit->item = currItem;
                    unit->ptype = PRO_JUDGE;

                    if(frontUnit->gtype == GRA_POLYGON)
                    {
                        PolygonDesc * currDesc = NULL;
                        if(frontUnit->ptype == PRO_JUDGE && polygons.size() > 0)
                        {
                            currDesc = polygons.at(polygons.size() - 1);
                        }
                        else if(frontUnit->ptype == PRO_LOOP && loopPolygons.size() >0)
                        {
                            currDesc = loopPolygons.at(loopPolygons.size() - 1);
                        }

                        if(currDesc)
                        {
                            if(currDesc->isProcLeft)
                            {
                                frontUnit->yesChild = unit;
                            }
                            else if(currDesc->isProcRight)
                            {
                                frontUnit->noChild = unit;
                            }
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

                    desc->isProcLeft = true;
                    desc->leftItem = yesItem;
                    desc->rightItem = noItem;

                    currItem = yesItem;
                }
                //循环处理
                else if(isYesItemProcced || outNum > 1)
                {
                    //判断循环栈是否包含了当前循环控件，
                    bool hasExisted = false;
                    PolygonDesc * existedDesc = NULL;
                    foreach(PolygonDesc * tmpDesc,loopPolygons)
                    {
                        if(tmpDesc->currItem == currItem)
                        {
                            hasExisted = true;
                            existedDesc = tmpDesc;

                            //前一个单元是循环[双层循环时，里层循环会先结束，外层循环未结束，此时要将结束的弹出，以免影响后面]
                            if(frontUnit->ptype == PRO_LOOP)
                            {
                                PolygonDesc * frontDesc = NULL;
                                for(int i = 0; i<loopPolygons.size();i++)
                                {
                                    if(loopPolygons.at(i)->currItem == frontUnit->item)
                                    {
                                        frontDesc = loopPolygons.at(i);
                                        break;
                                    }
                                }

                                if(frontDesc && frontDesc->isProcLeft)
                                {
                                    frontUnit->yesChild = tmpDesc->processUnit;
                                    frontDesc->isLeftOver = true;
                                    frontDesc->isProcLeft = false;

                                    frontDesc->isProcRight = true;

                                    currItem = frontDesc->rightItem;
                                }
                                else if(frontDesc && frontDesc->isProcRight)
                                {
                                    frontUnit->noChild = tmpDesc->processUnit;

                                    frontDesc->isRightOver = true;
                                    frontDesc->isProcRight = false;
                                }
                            }
                            else if(frontUnit->ptype == PRO_JUDGE )
                            {
                                if(polygons.size() >=1)
                                {
                                    PolygonDesc * topPolygon = polygons.at(polygons.size() -1);

                                    if(topPolygon->isProcLeft)
                                    {
                                        frontUnit->yesChild = tmpDesc->processUnit;
                                    }
                                    else if(topPolygon->isProcRight)
                                    {
                                        frontUnit->noChild = tmpDesc->processUnit;
                                    }
                                }
                            }
                            else
                            {
                                frontUnit->nextChild = tmpDesc->processUnit;
                            }
                            break;
                        }
                    }

                    //嵌套循环时，内层循环先结束，外层未结束，需要弹出内层循环对已经完成的弹出
                    for(int i = loopPolygons.size() - 1; i>=0;i--)
                    {
                        if(loopPolygons.at(i)->isLeftOver && loopPolygons.at(i)->isRightOver)
                        {
                            delete loopPolygons.at(i);
                            loopPolygons.remove(i);
                        }
                    }

                    //对已存在的循环状态进行修订
                    if(hasExisted)
                    {
                        //将栈顶的循环状态进行修正
                        if(existedDesc->isProcLeft)
                        {
                            //双层循环+判断
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
                                    while(loopPolygons.size() > 0)
                                    {
                                        PolygonDesc * topLoop = loopPolygons.at(loopPolygons.size() -1 );
                                        if(topLoop->isProcLeft)
                                        {
                                            topLoop->isProcLeft = false;
                                            topLoop->isLeftOver = true;
                                            topLoop->isProcRight = true;

                                            currItem = topLoop->rightItem;
                                            frontUnit = topLoop->processUnit;
                                            break;
                                        }
                                        else if(topLoop->isProcRight)
                                        {
                                            topLoop->isProcRight = false;
                                            topLoop->isRightOver = true;
                                        }

                                        PolygonDesc * parent = NULL;

                                        if(topLoop->isLeftOver && topLoop->isLeftOver)
                                        {
                                            parent = topLoop->parent;
                                            delete topLoop;
                                            loopPolygons.pop();             //弹出自身
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
                                }
                            }
                            else
                            {
                                existedDesc->isProcLeft = false;
                                existedDesc->isLeftOver =  true;
                                existedDesc->isProcRight = true;

                                frontUnit = existedDesc->processUnit;
                                currItem = existedDesc->rightItem;
                            }
                        }
                        else if(existedDesc->isProcRight)
                        {
                            existedDesc->isProcRight = false;
                            existedDesc->isRightOver = true;
                        }
                    }

                    //遇到新的循环分支
                    if(!hasExisted)
                    {
                        PolygonDesc * desc = new PolygonDesc;
                        desc->currItem = currItem;

                        ProcessUnit  * unit = new ProcessUnit;
                        unit->gtype = gtype;
                        unit->item = currItem;
                        unit->ptype = PRO_LOOP;

                        if(loopPolygons.size() > 0 && frontUnit && frontUnit->ptype == PRO_LOOP)
                        {
                            PolygonDesc * currDesc = loopPolygons.at(loopPolygons.size() - 1);

                            if(currDesc->isProcLeft)
                            {
                                frontUnit->yesChild = unit;
                            }
                            else if(currDesc->isProcRight)
                            {
                                frontUnit->noChild = unit;
                            }
                        }
                        else if(polygons.size() > 0 && frontUnit && frontUnit->ptype == PRO_JUDGE)
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
                        if(loopPolygons.size() > 0)
                        {
                            PolygonDesc * currDesc = loopPolygons.at(loopPolygons.size() - 1);
                            desc->parent = currDesc;
                        }

                        loopPolygons.push(desc);

                        desc->isProcLeft = true;
                        desc->leftItem = yesItem;
                        desc->rightItem = noItem;

                        //如果循环状态下，yes分支指向了doWhileRects集合中的一员，那么自动认定分支处理结束
                        if(isYesItemProcced && yesProcessUnit)
                        {
                            desc->isProcLeft = false;
                            desc->isLeftOver = true;
                            desc->isProcRight = true;

                            currItem = desc->rightItem;
                            desc->processUnit->yesChild = yesProcessUnit;
                        }
                        else
                        {
                            currItem = yesItem;
                        }
                    }
                }
            }
        }
    }
    //[2]验证逻辑是否正确
    return RETURN_SUCCESS;
}
