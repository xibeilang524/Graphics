/*************************************************
**版  权：RenGu Company
**文件名: processcheck.h
**作  者: wey       Version: 1.0       Date: 2016.10.11
**描  述:
**Others: 流程验证
**       【3】:判断整个链路是否完整，同时判断逻辑是否正确；并且将控件按照逻辑顺序重新排列【重要】
**            目前只考虑顺序和判断结构，不考虑并行
**修改历史:
**20161012:wey:添加流程解析功能(暂不支持循环和并行)
**20161017:wey:添加对验证的队列控件的提取并转换成处理单元
*************************************************/
#ifndef PROCESSCHECK_H
#define PROCESSCHECK_H

#include "SimulateHeader.h"

class MyItem;
class MyArrow;

class ProcessCheck
{
public:
    //用于记录推演过程中遇到的判断框
    struct PolygonDesc
    {
        PolygonDesc()
        {
            isProcLeft = false;
            isProcRight = false;
            isLeftOver = false;
            isRightOver = false;
            currItem = NULL;
            leftItem = NULL;
            rightItem = NULL;
            leftChild = NULL;
            rightChild = NULL;
            parent = NULL;
            processUnit = NULL;
        }

        bool isProcLeft;                     //是否正在处理左子树
        bool isProcRight;                    //是否正在处理右子树
        bool isLeftOver;                     //左子树是否处理完成
        bool isRightOver;                    //右子树是否处理完成
        MyItem * currItem;                   //当前指向的控件
        MyItem * leftItem;                   //当前左控件
        MyItem * rightItem;                  //当前右控件
        PolygonDesc * leftChild;             //左子树直属判断框
        PolygonDesc * rightChild;            //右子树直属判断框
        PolygonDesc * parent;                //父框
        ProcessUnit * processUnit;           //处理单元
    };

    static ProcessCheck * instance();

    ReturnType checkProcess(QList<QGraphicsItem *> &existedItems, QList<MyItem *> &resortedItems, QList<ProcessUnit *> &procUnits);

private:
    MyItem * getMyItem(MyArrow * arrow,bool isEnd = true);
    QList<MyItem *> getInOutItems(MyItem *currItem, QList<MyArrow *>& arrows, bool isEnd = true);
    void getItemInOutNum(MyItem * currItem,QList<MyArrow *>& arrows,int &inputNum,int &outputNum);
    MyItem * getConditionItem(MyItem *currItem, QList<MyArrow *>& arrows, bool isYes = true);

    ProcessCheck();
    static ProcessCheck * pcheck;

};

#endif // PROCESSCHECK_H
