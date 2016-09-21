/*************************************************
**版  权：RenGu Company
**文件名: Header.h
**作  者: wey       Version: 1.0       Date: 2016.09.01
**描  述: 定义通用的结构体
**Others:
**
**修改历史:
**20160913:wey:调整类型格式定义为qreal
*************************************************/
#ifndef HEADER_H
#define HEADER_H

#define PI 3.141592653
#define ICON_WIDTH  60    //图标的宽高
#define ICON_HEIGHT 60

#include <QBrush>
#include <QPen>
#include <QFont>
#include <QDataStream>
#include <QUuid>

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
    GRA_SQUARE,
    GRA_RECT,
    GRA_ROUND_RECT,
    GRA_CIRCLE,
    GRA_ELLIPSE,
    GRA_POLYGON,
    GRA_LINE,
    GRA_VECTOR_LINE,
    GRA_TEXT,
    GRA_NODE_PORT
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
        rotateDegree = 0;
        itemPen.setWidth(1);
        alphaValue = 100;
        zValue = 0;

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

    bool isNeedBrush;        //为ture时表示需要笔刷填充
    QBrush itemBrush;

    bool isNeedBorder;       //为true时表示是否需要边框
    QPen   itemPen;

    MyRect itemRect;
    int    alphaValue;       //透明度值
    int    rotateDegree;

    bool isFont;             //是否为字体
    QString content;         //文字内容

    QFont  itemFont;
    QColor fontColor;        //字体颜色

    qreal zValue;            //深度值
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
