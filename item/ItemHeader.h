/*************************************************
**版  权：RenGu Company
**文件名: ItemHeader.h
**作  者: wey       Version: 1.0       Date: 2016.09.01
**描  述: 绘图模块头文件
**Others:
**
**修改历史:
**20160918:wey:添加DragDirect枚举类型
*************************************************/
#ifndef ITEMHEADER_H
#define ITEMHEADER_H

enum PointType
{
    TOP_LEFT,
    TOP_MIDDLE,
    TOP_RIGHT,
    MIDDLE_LEFT,
    MIDDLE_RIGHT,
    BOTTOM_LEFT,
    BOTTOM_MIDDLE,
    BOTTOM_RIGHT
};

enum MouseType
{
    MOUSE_NONE,
    MOUSE_PRESS,
    MOUSE_MOVE,
    MOUSE_RELEASE
};

//线段两端所连接的类型
enum LineType
{
    LINE_NONE,
    LINE_MYITEM,
    LINE_NODEPORT
};

#define PI 3.141592653
#define ROTATE_WIDTH   5
#define ROTATE_SQUARE_WIDTH 5
#define ALLOW_DROP_RANGE 10             //控件拖入后允许放下的范围


#endif // HEADER_H
