/*************************************************
**版  权：RenGu Company
**文件名: myiteminfo.h
**作  者: wey       Version: 1.0       Date: 2016.10.02
**描  述:
**Others: 用于显示MyItem的位置和旋转角度信息
**
**修改历史:
**
*************************************************/
#ifndef MYITEMINFO_H
#define MYITEMINFO_H

#include "mysuperitem.h"

class MyItemInfo : public MySuperItem
{
public:
    MyItemInfo(GraphicsType type, QGraphicsItem * parent1 = 0 ,QObject* parent2 = 0);
    ~MyItemInfo();

    QRectF boundingRect()const;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    void setRotationInfo(int degree);
    void setPosInfo(int x,int y);

private:
    QString showText;
};

#endif // MYITEMINFO_H
