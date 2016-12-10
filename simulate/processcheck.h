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
**20161024:wey:修复因流程不完整导致推演检查时程序奔溃问题
**20161027:wey:修复因多个判断未完全处理结束而导致流程自动退出
**20161104:wey:添加对for和while循环的支持【！！！】
**20161106:wey:添加对doWhile循环的支持【！！doWhile循环和是否判断会重合】
**20161130:wey:【！！未修复】发现dowhile循环时no分支作为循环体导致无限循环,PolygonDesc增加isLoopYes字段描述
**20161210:wey:【！！】调整若菱形的no分支指向前面的控件，则将菱形认定为判断框而非dowhile循环，只有菱形的yes分支指向以前控件，才认定为dowhile循环。
*************************************************/
#ifndef PROCESSCHECK_H
#define PROCESSCHECK_H

#include "SimulateHeader.h"

class MyItem;
class MyArrow;

class ProcessCheck
{
public:
    //用于记录推演过程中遇到的判断框(和循环框复用)
    struct PolygonDesc
    {
        PolygonDesc()
        {
            isProcLeft = false;
            isProcRight = false;
            isLeftOver = false;
            isRightOver = false;
            isLoopYes = true;
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
        bool isLoopYes;                      //在作为循环框时，表示yes分支作为循环条件，还是no分支作为循环。默认为yes
        PolygonDesc * leftChild;             //左子树直属判断框
        PolygonDesc * rightChild;            //右子树直属判断框
        PolygonDesc * parent;                //父框
        ProcessUnit * processUnit;           //处理单元
    };
    static ProcessCheck * instance();

    ReturnType checkProcess(QList<QGraphicsItem *> &existedItems, QList<MyItem *> &resortedItems, QList<ProcessUnit *> &procUnits);

private:
    ProcessCheck();
    static ProcessCheck * pcheck;

};

#endif // PROCESSCHECK_H
