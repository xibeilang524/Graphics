/*************************************************
**版  权：RenGu Company
**文件名: Header.h
**作  者: wey       Version: 1.0       Date: 2016.09.01
**描  述: 定义通用的结构体
**Others:
**
**修改历史:
**20160913:wey:调整类型格式定义为qreal
**20160925:wey:增加隐藏控件位置关系SpiltDirect
**20160927:wey:添加线条类型AddLineType
**             添加拖入类型为平行四边形
*************************************************/
#ifndef HEADER_H
#define HEADER_H

#define PI 3.141592653
#define ICON_WIDTH  60          //图标的宽高
#define ICON_HEIGHT 60
#define LAYOUT_SPACING  1       //布局管理器中两控件间的距离
#define FONT_DEAFULT_PIX  9     //字体初始化大小
#define ROTATE_MIN_DEGREE 0     //旋转的最小角度
#define ROTATE_MAX_DEGREE 360   //旋转的最大角度

#include <QString>

#if defined(Q_CC_MSVC)
#include "typeinfo.h"
#elif defined(Q_CC_GNU)
#include "typeinfo"
#endif

#define TYPE_ID(a) typeid(a).name()
#define MY_ASSERT(a) {if(!a) return;}

#include <QBrush>
#include <QPen>
#include <QFont>
#include <QDataStream>
#include <QUuid>

#include "./item/ItemHeader.h"

enum ReturnType
{
    RETURN_OK,
    FILE_NOT_EXIST,
    FILE_CANT_READ,
    FILE_CANT_WRITE,
    FILE_ILLEGAL             //文件格式不合法
};

//当前添加的类型
enum GraphicsType
{
    GRA_NONE,
    GRA_SQUARE,          //正方形
    GRA_RECT,            //矩形
    GRA_ROUND_RECT,      //圆角矩形
    GRA_CIRCLE,          //圆
    GRA_ELLIPSE,         //椭圆
    GRA_POLYGON,         //菱形
    GRA_PARALLELOGRAM,   //平行四边形
    GRA_LOOP_UP,         //循环上限
    GRA_LOOP_DOWN,       //循环下限
    GRA_ANNOTATION,      //注解
    GAR_PARALLE,         //并行
    GRA_LINE,            //直线
    GRA_VECTOR_LINE,     //折线
    GRA_TEXT,            //文本
    GRA_NODE_PORT,       //端口
    GRA_DRAG_POINT,      //控件拖拽点
    GRA_DRAG_LING,       //控件线段拖拽点
    GRA_ROTATE_POINT,    //控件旋转点
    GRA_ITEM_INFO        //控件显示信息
};

//拖入端口的方向
enum DragDirect
{
    DRAG_NONE,
    DRAG_LEFT,
    DRAG_TOP,
    DRAG_RIGHT,
    DRAG_BOTTOM
};

//端口到达两端后，用于区分到达哪一端
enum DragRange
{
    DRAG_RANG_NONE,
    DRAG_RANG_MAX,
    DRAG_RANG_MIN
};

//分割栏与控件的位置关系
enum SpiltDirect
{
    SPLIT_NONE,
    SPLIT_LEFT,
    SPLIT_TOP,
    SPLIT_RIGHT,
    SPLIT_BOTTOM
};

//锁定的状态
enum ItemLockState
{
    ITEM_ALL_LOCK,             //全部锁定
    ITEM_NOT_ALL_LOCK,         //部分锁定/部分未锁定
    ITEM_ALL_UNLOCK            //全部未锁定
};

//线条的类型
enum AddLineType
{
    LINE_HORIZONTAL,           //直线
    LINE_ARROW,                //箭头
    LINE_SOLID_TRIANGLE        //实心三角箭头
};

//记录当前item在scene中的x、y、w、h值
struct MyRect
{
    MyRect()
    {
        x = 0;
        y = 0;
        width = 0;
        height = 0;
    }

    friend QDataStream& operator <<(QDataStream &,MyRect & rect);
    friend QDataStream& operator >>(QDataStream &,MyRect & rect);

    qreal x;
    qreal y;
    qreal width;
    qreal height;
};

//每个item拥有的属性，笔刷、画笔、位置、大小、角度、字体
struct ItemProperty
{
    ItemProperty()
    {
        isNeedBrush = true;
        isNeedBorder = true;
        isFont = false;
        isMoveable = true;
        rotateDegree = 0;
        itemPen.setWidth(1);
        alphaValue = 100;
        zValue = 0;
        startLineType = LINE_HORIZONTAL;
        endLineType = LINE_HORIZONTAL;
        createUUID();
    }

    //产生一个新的身份唯一标识，在剪切、复制、本地打开时需要手动的更新此值
    void createUUID()
    {
        startItemID = QUuid::createUuid().toString();    //新建对象时创建唯一标识符
    }

    QString startItemID;                  //在用于非直线的控件时，只用startItemId标识当前控件
    QString endItemID;                    //在用于直线控件时，两个表示直线两端连接的控件的ID号

    friend QDataStream & operator <<(QDataStream &,ItemProperty & item);
    friend QDataStream & operator >>(QDataStream &,ItemProperty & item);

    bool isNeedBrush;               //为ture时表示需要笔刷填充
    QBrush itemBrush;
    bool isNeedBorder;              //为true时表示是否需要边框
    QPen   itemPen;

    MyRect itemRect;
    int    alphaValue;              //透明度值
    int    rotateDegree;

    bool isFont;                    //是否为字体
    QString content;                //文字内容
    QFont  itemFont;
    QColor fontColor;               //字体颜色

    qreal zValue;                   //深度值

    LineType lineType;              //线条的类型
    AddLineType startLineType;      //起始线条类型
    AddLineType endLineType;        //终止线条类型
    PointType startPointType;       //线条在【LINE_MYITEM】模式下，起点所在item边的位置
    PointType endPointType;         //线条在【LINE_MYITEM】模式下，终点点所在item边的位置

    bool isMoveable;                //是否被锁定
};

//节点属性
struct NodePortProperty
{
    NodePortProperty()
    {
        startItemID = QUuid::createUuid().toString();    //新建对象时创建唯一标识符
    }

    friend QDataStream & operator <<(QDataStream &,NodePortProperty & item);
    friend QDataStream & operator >>(QDataStream &,NodePortProperty & item);

    QBrush itemBrush;           //填充颜色
    DragDirect direct;          //保存的方向
    qreal scaleFactor;          //拖入的位置相当于当前一边所在的比例
    QString parentItemID;       //父控件的ID(解析后寻找停靠点)
    QString startItemID;        //在用于非直线的控件时，只用startItemId标识当前控件(用于箭头寻找父节点)
};

//暂存一个剪切时的控件信息
struct CutInfo
{
    GraphicsType graphicsType;
    ItemProperty itemProperty;
    QString content;
    QList<NodePortProperty> nodeProperties;      //控件包含端口的信息
};

#endif // HEADER_H
