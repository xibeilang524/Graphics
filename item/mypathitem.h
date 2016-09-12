/*************************************************
**版  权：RenGu Company
**文件名: mypathitem.h
**作  者: wey       Version: 1.0       Date: 2016.09.09
**描  述: 用于绘制折线
**Others:
**
**修改历史:
**
*************************************************/
#ifndef MYPATHITEM_H
#define MYPATHITEM_H

#include <QGraphicsPathItem>
#include <QPainterPath>
#include <QObject>

class MyItem;

#include "../Header.h"

class MyPathItem : public QObject, public QGraphicsPathItem
{
public:
    MyPathItem(MyItem  * startItem,MyItem  * endItem,QObject * parent = 0,QGraphicsItem * parent1 = 0);
    ~MyPathItem();

    MyItem * getStartItem() const{ return startItem; }
    MyItem * getEndItem() const{ return endItem; }

    void setProperty(ItemProperty property);
    ItemProperty getProperty(){return this->property;}
    GraphicsType getType(){return this->currItemType;}

    void setPath(const QPainterPath &path);
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QRectF boundingRect()const;

    QPainterPath  shape() const;

private:
    MyItem  * startItem;
    MyItem  * endItem;

    GraphicsType currItemType;
    ItemProperty property;             //保存当前属性

    QRectF boundRect;                  //包裹的矩形

    QPainterPath painterPath;          //当前路径

    QList<QPointF> points;

};

#endif // MYPATHITEM_H
