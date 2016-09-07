#ifndef HEADER_H
#define HEADER_H

#include <QBrush>
#include <QPen>
#include <QFont>

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
    GRA_TEXT,
    GRA_ARROW
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
    int x;
    int y;
    int width;
    int height;
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
    }

    bool isNeedBrush;        //为ture时表示需要笔刷填充
    QBrush itemBrush;

    bool isNeedBorder;       //为true时表示是否需要边框
    QPen   itemPen;
    QFont  itemFont;
    MyRect itemRect;
    int    alphaValue;       //透明度值
    int    rotateDegree;

    bool isFont;             //是否为字体
};

#endif // HEADER_H
