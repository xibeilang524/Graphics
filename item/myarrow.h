#ifndef MYARROW_H
#define MYARROW_H

#include <QGraphicsLineItem>
#include <QPolygonF>

#include "../Header.h"

class MyItem;

class MyArrow : public QGraphicsLineItem
{
public:
    MyArrow(MyItem  * startItem,MyItem  * endItem,QGraphicsItem * parent = 0);
    ~MyArrow();

    QRectF boundingRect()const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QPainterPath shape()const;
    void updatePosition();

    MyItem * getStartItem() const{ return startItem; }
    MyItem * getEndItem() const{ return endItem; }

private:
    MyItem  * startItem;
    MyItem  * endItem;
    QPolygonF arrowHead;

    QRectF boundRect;
};

#endif // MYARROW_H
