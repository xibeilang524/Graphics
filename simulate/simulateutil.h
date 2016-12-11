/*************************************************
**版  权：RenGu Company
**文件名: simulateutil.h
**作  者: wey       Version: 1.0       Date: 2016.10.31
**描  述:推演的工具类
**Others:1.根据某个控件，获取其所有的父控件
**
**修改历史:
**20161101:wey:增加GRA_RECT引单功能[!!!]
**20161107:wey:修复因dowhile产生的rect无限循环问题(尚未对判断框功能的区分(判断/循环))
**20161117:wey:增加对循环或判断控件的引单功能
**20161122:wey:添加状态机词法解析判断框语句
**             添加对!的支持
**20161123:wey:添加对判断条件执行，实现判断框[!!!]
**20161211:wey:在进行模拟时，在原来直线基础上添加折线
*************************************************/
#ifndef SIMULATEUTIL_H
#define SIMULATEUTIL_H

#include "../Header.h"

class MyItem;
class MyArrow;
class MyPathItem;

class SimulateUtil
{
public:
    static SimulateUtil * instance();

    QList<MyItem *> getCurrParentItem(MyItem *);

    MyItem * getMyItem(MyArrow * arrow,bool isEnd = true);
    MyItem * getMyItem(MyPathItem * pathItem,bool isEnd = true);
    QList<MyItem *> getInOutItems(MyItem *currItem, QList<MyArrow *>& arrows, QList<MyPathItem *> &pathItems, bool isEnd = true);
    void getItemInOutNum(MyItem * currItem, QList<MyArrow *>& arrows, QList<MyPathItem *> &pathItems, int &inputNum, int &outputNum);
    MyItem * getConditionItem(MyItem *currItem, QList<MyArrow *>& arrows, QList<MyPathItem *> &pathItems, bool isYes = true);

    bool getIsItemStart(MyItem * item);
    bool getIsItemEnd(MyItem * item);

    bool parseText(QString text,QStringList & result);
    bool getResult(QStringList & wordList);

private:
    SimulateUtil();
    static SimulateUtil * util;

    bool isRelationOperation(QString text);
    bool isLogicalOperation(QString text);

    //运算符计算
    QVariant calcRelation(QStringList & result,int &index,bool & correct);
    QVariant calcLogical(QStringList & result,int &index,bool & correct);
    QVariant getNext(QStringList & result,int &index,bool & correct);

    //用于词法分析判断框并提取字符
    enum CharacterType
    {
        CHAR_START,         //未开始解析
        CHAR_NORMAL,        //普通字符(变量)
        CHAR_RELATION,      //关系运算符>、>=、==、!=、<、<=
        CHAR_LOGICAL,       //逻辑运算符&&、||、！
        CHAR_LEFT_AGGRE,    //左括号'('
        CHAR_RIGHT_AGGRE,   //右括号')'
        CHAR_OPPOSE,        //逻辑非'!'
        CHAR_NOT_EXIST,     //组合的字符未存在
        CHAR_ILLEGAL        //非法字符
    };

    //单条路径的状态
    struct SignalPathDesc
    {
        SignalPathDesc()
        {
            hasSearched = false;
            isSearching = false;
            hasAddItem = 0;
            startItem = NULL;
        }
        bool isSearching;           //是否正在搜寻此节点
        bool hasSearched;           //是否已经搜索结束
        int hasAddItem;             //在当前路径下，已经添加的至保存队列的item数量，用于在循环状态下反向弹出
        MyItem * startItem;         //开始搜索item
    };

    //用于记录一个GRA_RECT存在多个输入箭头时，针对每个箭头来路径搜索，记录每个路径的状态
    struct MultiPathDesc
    {
        MultiPathDesc()
        {
            totalPathNum = 0;
            currPathNum = 0;
            currItem = NULL;
        }
        int totalPathNum;       //总路径数量
        int currPathNum;        //当前搜寻的数量,currPathNum = totalPathNum -1;
        MyItem * currItem;      //产生分支的GRA_RECT

        QList<SignalPathDesc*> pathes;      //总共存在的路径
    };

    QStringList relationList;       //关系运算符表
    QStringList logicalList;        //逻辑运算符表
};

#endif // SIMULATEUTIL_H
