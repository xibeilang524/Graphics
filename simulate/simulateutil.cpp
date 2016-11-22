#include "simulateutil.h"

#include "../item/myitem.h"
#include "../item/myarrow.h"
#include "../item/draglinepoint.h"

#include <QStack>

#include <QDebug>

SimulateUtil * SimulateUtil::util = NULL;

SimulateUtil::SimulateUtil()
{
    relationList<<">"<<">="<<"=="<<"!="<<"<"<<"<=";
    logicalList<<"&&"<<"||"<<"!";
}

SimulateUtil * SimulateUtil::instance()
{
    if(!util)
    {
        util = new SimulateUtil;
    }
    return util;
}

/*!
 *获取当前控件的父控件，只考虑父控件为处理框(GRA_RECT).
 *【1】20161101增加对循环条件的引单功能
 *【2】20161107修复因dowhile产生的rect无限循环问题(尚未对判断框功能的区分(判断/循环))
 *【3】20161117增加对判断循环框的引单功能
 *!*/
QList<MyItem *> SimulateUtil::getCurrParentItem(MyItem * item)
{
    QList<MyItem *> list;

    MyItem * pItem = item;
    bool isFirst = true;                  //是否第一次循环，避免因pItem==Item导致判断出错问题

    QStack<MultiPathDesc * > descs;       //记录推演过程中某个控件因多个输入产生的多路径
    QStack<MyItem * > polygons;           //记录推到过程中遇到的多边形(可能是判断，也可能是循环)
    MultiPathDesc * currMPath = NULL;

    bool isSearchOver = false;

    if(item->getType())
    {
        while(pItem&&!isSearchOver)
        {
            QList<MyArrow *> arrows = pItem->getArrows();
            QList<MyItem *> pItems = getInOutItems(pItem,arrows,false);

//            qDebug()<<pItem->getText();

            bool hasAdded = false;
            bool skipCurrLoop = false;

            if(pItem->getType() == GRA_POLYGON)
            {
                bool hasAddPolygon = false;

                if(!isFirst)
                {
                    if(pItem == item)
                    {
                        hasAddPolygon = true;
                    }

                    foreach(MyItem * tmpItem,polygons)
                    {
                        if(tmpItem == pItem)
                        {
                            hasAddPolygon = true;
                            break;
                        }
                    }
                }

                if(!hasAddPolygon)
                {
                    polygons.append(pItem);
                }
                //已经加入，则说明产生了循环
                else
                {
                    hasAdded = true;
                }
            }
            //为了防止支线中第一个item为rect，导致漏掉处理
            else if (pItem->getType() == GRA_RECT)
            {
                if(!isFirst)
                {
                    bool hasAddRect = false;

                    if(pItem == item)
                    {
                        hasAddRect = true;
                    }

                    foreach(MyItem * tmpItem ,list)
                    {
                        if(tmpItem == item)
                        {
                            hasAddRect = true;
                            break;
                        }
                    }

                    if(!hasAddRect)
                    {
                        if(descs.size() > 0)
                        {
                            MultiPathDesc * lastDesc = descs.at(descs.size()-1);
                            SignalPathDesc * spathDesc = lastDesc->pathes.at(lastDesc->currPathNum);
                            spathDesc->hasAddItem++;
                        }

//                        qDebug()<<"===add:"<<pItem->getText();
                        list.append(pItem);
                    }
                    else
                    {
                        while(descs.size() > 0 )
                        {
                            MultiPathDesc * lastDesc = descs.at(descs.size()-1);
                            SignalPathDesc * spathDesc = NULL;

                            if(lastDesc->currPathNum < lastDesc->totalPathNum)
                            {
                                spathDesc = lastDesc->pathes.at(lastDesc->currPathNum);
                            }

                            if(spathDesc)
                            {
                                for(int i=0;i<spathDesc->hasAddItem;i++)
                                {
//                                    qDebug()<<"===remve:"<<list.last()->getText();
                                    list.removeLast();
                                }

                                spathDesc->isSearching = false;
                                spathDesc->hasSearched = true;

                                lastDesc->currPathNum += 1;
                                //当前已经搜索结束,
                                if(lastDesc->currPathNum >= lastDesc->totalPathNum)
                                {
                                    foreach(SignalPathDesc* tmpSignalPath,lastDesc->pathes)
                                    {
                                        delete tmpSignalPath;
                                    }
                                    lastDesc->pathes.clear();
                                    descs.pop();
                                }
                                else
                                {
                                    SignalPathDesc * spathDesc = lastDesc->pathes.at(lastDesc->currPathNum);
                                    pItem = spathDesc->startItem;
                                    skipCurrLoop = true;
//                                    qDebug()<<"+++"<<pItem->getText();
                                    break;
                                }
                            }

                            if(descs.size() > 0)
                            {
                                MultiPathDesc  *  parentLastDesc = descs.at(descs.size()-1);
                                parentLastDesc->currPathNum++;
                                if(parentLastDesc->currPathNum >= parentLastDesc->totalPathNum)
                                {
                                    delete parentLastDesc;
                                    descs.pop();
                                }
                                else
                                {
                                    SignalPathDesc * spathDesc = parentLastDesc->pathes.at(parentLastDesc->currPathNum);
                                    pItem = spathDesc->startItem;
                                    break;
                                }
                            }

                            if(descs.size() == 0)
                            {
                                isSearchOver = true;
                                break;
                            }
                       }
                    }
                }
            }

            isFirst = false;

            if(!skipCurrLoop)
            {
                if(!hasAdded)
                {
                    if(pItems.size() > 1)
                    {
                        for(int i = 0;i < descs.size() ; i++)
                        {
                            if(descs.at(i)->currItem == pItem)
                            {
                                hasAdded = true;
                            }
                            break;
                        }

                        if(!hasAdded)
                        {
                            MultiPathDesc * mpath = new MultiPathDesc;
                            mpath->totalPathNum = pItems.size();
                            mpath->currItem = pItem;
                            foreach(MyItem * tmp,pItems)
                            {
                                SignalPathDesc * path = new SignalPathDesc;
                                path->startItem = tmp;
                                mpath->pathes.append(path);
                            }

                            descs.append(mpath);
                            currMPath = mpath;

                            pItem = pItems.at(0);
                        }
                    }
                    else if(pItems.size() == 1)
                    {
                        pItem = pItems.at(0);
                    }
                    else
                    {
                        isSearchOver = true;
                    }
                }
                //判断当前是否为开始
                if(getIsItemStart(pItem))
                {
                    if(descs.size() > 0)
                    {
                        MultiPathDesc * lastDesc = descs.at(descs.size()-1);
                        SignalPathDesc * spathDesc = lastDesc->pathes.at(lastDesc->currPathNum);
                        spathDesc->isSearching = false;
                        spathDesc->hasSearched = true;

                        lastDesc->currPathNum += 1;

                        //当前已经搜索结束
                        if(lastDesc->currPathNum >= lastDesc->totalPathNum)
                        {
                            foreach(SignalPathDesc* tmpSignalPath,lastDesc->pathes)
                            {
                                delete tmpSignalPath;
                            }
                            lastDesc->pathes.clear();
                            descs.pop();
                        }
                        else
                        {
                            SignalPathDesc * spathDesc = lastDesc->pathes.at(lastDesc->currPathNum);
                            pItem = spathDesc->startItem;
                        }
                    }

                    if(descs.size() == 0)
                    {
                        isSearchOver = true;
                    }
                }

                //当存在循环时，循环后指向了自己
                if(hasAdded)
                {
                    //如果有多路径存在，则要对当前路径进行更新；并将已经加入的反向弹出
                    while(descs.size() > 0 )
                    {
                        MultiPathDesc * lastDesc = descs.at(descs.size()-1);
                        SignalPathDesc * spathDesc = NULL;

                        if(lastDesc->currPathNum < lastDesc->totalPathNum)
                        {
                            spathDesc = lastDesc->pathes.at(lastDesc->currPathNum);
                        }

                        if(spathDesc)
                        {
                            for(int i=0;i<spathDesc->hasAddItem;i++)
                            {
//                                qDebug()<<"===remve:"<<list.last()->getText();
                                list.removeLast();
                            }

                            spathDesc->isSearching = false;
                            spathDesc->hasSearched = true;

                            lastDesc->currPathNum += 1;
                            //当前已经搜索结束,
                            if(lastDesc->currPathNum >= lastDesc->totalPathNum)
                            {
                                delete lastDesc;
                                descs.pop();
                            }
                            else
                            {
                                SignalPathDesc * spathDesc = lastDesc->pathes.at(lastDesc->currPathNum);
                                pItem = spathDesc->startItem;
                                break;
                            }
                        }

                        if(descs.size() > 0)
                        {
                            MultiPathDesc  *  parentLastDesc = descs.at(descs.size()-1);
                            parentLastDesc->currPathNum++;
                            if(parentLastDesc->currPathNum >= parentLastDesc->totalPathNum)
                            {
                                delete parentLastDesc;
                                descs.pop();
                            }
                            else
                            {
                                SignalPathDesc * spathDesc = parentLastDesc->pathes.at(parentLastDesc->currPathNum);
                                pItem = spathDesc->startItem;
                                break;
                            }
                        }

                        if(descs.size() == 0)
                        {
                            isSearchOver = true;
                            break;
                        }
                    }
                 }
            }
        }
    }

//    foreach(MyItem * tmpItem,list)
//    {
//        qDebug()<<"result:"<<tmpItem->getText();
//    }

    return list;
}

//获取箭头起始点/终点Item
MyItem * SimulateUtil::getMyItem(MyArrow * arrow, bool isEnd)
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

//获取控件上作为箭头起点或终点控件集合
QList<MyItem *> SimulateUtil::getInOutItems(MyItem *currItem,QList<MyArrow *> &arrows, bool isEnd)
{
    QList<MyItem *> resultList;

    foreach (MyArrow * arrow, arrows)
    {
       MyItem * item = SimulateUtil::instance()->getMyItem(arrow,isEnd);
       if(item != currItem)
       {
           resultList.append(item);
       }
    }
    return resultList;
}

//获取某个控件作为箭头起点和终点个数
void SimulateUtil::getItemInOutNum(MyItem *currItem, QList<MyArrow *> &arrows, int &inputNum, int &outputNum)
{
    QString currItemId = currItem->getProperty().startItemID;

    foreach(MyArrow * arrow,arrows)
    {
        MyItem * startItem = SimulateUtil::instance()->getMyItem(arrow,false);
        MyItem * endItem = SimulateUtil::instance()->getMyItem(arrow,true);
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

//分别获取条件成立和不成立的item
MyItem * SimulateUtil::getConditionItem(MyItem * currItem,QList<MyArrow *> &arrows, bool isYes)
{
    foreach(MyArrow * arrow,arrows)
    {
        MyItem * item = SimulateUtil::instance()->getMyItem(arrow,true);
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

//判断当前是否为开始框
bool SimulateUtil::getIsItemStart(MyItem *item)
{
    QString text = item->getText();
    if(text == "开始" || text.toLower().contains("start"))
    {
        return true;
    }

    return false;
}

//判断当前是否为结束框
bool SimulateUtil::getIsItemEnd(MyItem *item)
{
    QString text = item->getText();
    if(text == "结束" || text.toLower().contains("end"))
    {
        return true;
    }

    return false;
}

//是否为关系运算符
bool SimulateUtil::isRelationOperation(QString text)
{
    return relationList.contains(text);
}

//是否为逻辑运算符
bool SimulateUtil::isLogicalOperation(QString text)
{
    return logicalList.contains(text);
}

/**
 *利用状态机词法分析判断条件
 *【1】替换存在的空格
 *【2】逐个对字符串进行分析
 */
bool SimulateUtil::parseText(QString text,QStringList & result)
{
    //【1】
    QRegExp exp("\\s+");
    QString nonWhileSpace = text.replace(exp,"");

    //【2】
    CharacterType ctype = CHAR_START;        //上一次的字符类型状态
    QString hasChar;
    QChar ch;
    bool isRight = true;
    for(int index = 0;  index < nonWhileSpace.length() && isRight; index ++)
    {
        ch = nonWhileSpace.at(index);

        switch(ctype)
        {
            case CHAR_START:
                            {
                                if(ch == '>' || ch == '=' || ch == '<' || ch == '!' || ch == '&' || ch == '|' || ch =='!' || ch == ')')
                                {
                                    ctype = CHAR_ILLEGAL;
                                }
                                else if(ch == '(')
                                {
                                    ctype = CHAR_LEFT_AGGRE;
                                }
                                else
                                {
                                    hasChar.append(ch);
                                    ctype = CHAR_NORMAL;
                                }
                            }
                            break;
            case CHAR_NORMAL:
                            {
                                if(ch == '>' || ch == '=' || ch == '<' )
                                {
                                    result.append(hasChar);
                                    hasChar.clear();
                                    hasChar.append(ch);
                                    ctype = CHAR_RELATION;
                                }
                                else if( ch == '!' || ch == '&' || ch == '|')
                                {
                                    result.append(hasChar);
                                    hasChar.clear();
                                    hasChar.append(ch);
                                    ctype = CHAR_LOGICAL;
                                }
                                else if(ch == '(')
                                {
                                    ctype = CHAR_ILLEGAL;
                                }
                                else if(ch == ')')
                                {
                                    result.append(hasChar);
                                    hasChar.clear();
                                    hasChar.append(ch);
                                    ctype = CHAR_RIGHT_AGGRE;
                                }
                                else
                                {
                                    hasChar.append(ch);
                                    ctype = CHAR_NORMAL;
                                }
                            }
                            break;
            case CHAR_RELATION:
                            {
                                if(ch == '>' || ch == '=' || ch == '<' || ch == '!' )
                                {
                                    hasChar.append(ch);
                                    ctype = CHAR_RELATION;
                                }
                                else if(ch == '(')
                                {
                                    if(isRelationOperation(hasChar))
                                    {
                                        result.append(hasChar);
                                        hasChar.clear();
                                        hasChar.append(ch);
                                        ctype = CHAR_LEFT_AGGRE;
                                    }
                                    else
                                    {
                                        ctype = CHAR_ILLEGAL;
                                    }
                                }
                                else if(ch == ')' || ch == '&' || ch == '|' || ch =='!')
                                {
                                    ctype = CHAR_ILLEGAL;
                                }
                                else
                                {
                                    if(isRelationOperation(hasChar))
                                    {
                                        result.append(hasChar);
                                        hasChar.clear();
                                        hasChar.append(ch);
                                        ctype = CHAR_NORMAL;
                                    }
                                    else
                                    {
                                        ctype = CHAR_ILLEGAL;
                                    }
                                }
                            }
                            break;
            case CHAR_LOGICAL:
                            {
                                if(ch == '>' || ch == '=' || ch == '<' )
                                {
                                    if(hasChar == "!")
                                    {
                                        hasChar.append(ch);
                                        ctype = CHAR_LOGICAL;
                                    }
                                    else
                                    {
                                        ctype = CHAR_ILLEGAL;
                                    }
                                }
                                else if(ch == '(')
                                {
                                    if(isLogicalOperation(hasChar)||isRelationOperation(hasChar))
                                    {
                                        result.append(hasChar);
                                        hasChar.clear();
                                        hasChar.append(ch);
                                        ctype = CHAR_LEFT_AGGRE;
                                    }
                                    else
                                    {
                                        ctype = CHAR_ILLEGAL;
                                    }
                                }
                                else if(ch == '&' || ch == '|' || ch =='!')
                                {
                                    hasChar.append(ch);
                                    ctype = CHAR_LOGICAL;
                                }
                                else if(ch == ')' )
                                {
                                    ctype = CHAR_ILLEGAL;
                                }
                                else
                                {
                                    if(isLogicalOperation(hasChar)||isRelationOperation(hasChar))
                                    {
                                        result.append(hasChar);
                                        hasChar.clear();
                                        hasChar.append(ch);
                                        ctype = CHAR_NORMAL;
                                    }
                                    else
                                    {
                                        ctype = CHAR_ILLEGAL;
                                    }
                                }
                            }
                            break;
            case CHAR_LEFT_AGGRE:
                            {
                                if(ch == '>' || ch == '=' || ch == '<' || ch == '!' ||ch == ')' || ch == '&' || ch == '|')
                                {
                                    ctype = CHAR_ILLEGAL;
                                }
                                else if(ch == '(')
                                {
                                    result.append(hasChar);
                                    hasChar.clear();
                                    hasChar.append(ch);
                                    ctype = CHAR_LEFT_AGGRE;
                                }
                                else
                                {
                                    result.append(hasChar);
                                    hasChar.clear();
                                    hasChar.append(ch);
                                    ctype = CHAR_NORMAL;
                                }
                            }
                            break;
            case CHAR_RIGHT_AGGRE:
                            {
                                if(ch == '>' || ch == '=' || ch == '<' )
                                {
                                    result.append(hasChar);
                                    hasChar.clear();
                                    hasChar.append(ch);
                                    ctype = CHAR_RELATION;
                                }
                                else if(ch == '&' || ch == '|')
                                {
                                    result.append(hasChar);
                                    hasChar.clear();
                                    hasChar.append(ch);
                                    ctype = CHAR_LOGICAL;
                                }
                                else
                                {
                                    ctype = CHAR_ILLEGAL;
                                }
                            }
                            break;
            case CHAR_ILLEGAL:
                            {
                                isRight = false;
                            }
                            break;
        }
    }

    if(ctype == CHAR_RIGHT_AGGRE)
    {
        result.append(")");
    }
    else if(ctype == CHAR_NORMAL)
    {
        hasChar.append(ch);
        result.append(hasChar);
    }
    else
    {
        return false;
    }
    return true;
}
