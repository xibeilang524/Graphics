#include "myarrow.h"

#include <QPainter>
#include <QRectF>
#include <QLineF>
#include <QDebug>

#include "myitem.h"

MyArrow::MyArrow(MyItem  * startItem,MyItem  * endItem,QGraphicsItem *parent):
    startItem(startItem),
    endItem(endItem),
    QGraphicsLineItem(parent)
{
    setFlag(QGraphicsItem::ItemIsSelectable, true);
}

QRectF MyArrow::boundingRect()const
{
    return QRectF(line().p1(), QSizeF(line().p2().x() - line().p1().x(),
                                           line().p2().y() - line().p1().y()));
}

QPainterPath MyArrow::shape()const
{
    QPainterPath path = QGraphicsLineItem::shape();
//    path.addPolygon(arrowHead);
    return path;
}

void MyArrow::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->save();

    if(startItem && endItem)
    {
        setLine(QLineF(startItem->pos(), endItem->pos()));

        painter->setPen(QPen(Qt::black,2));
    //    painter->drawRect(QRectF(startItem->pos(), endItem->pos()));
        painter->drawLine(line());
    }
    painter->restore();
}

void MyArrow::updatePosition()
{
    QLineF line(mapFromItem(startItem, 0, 0), mapFromItem(endItem, 0, 0));
    setLine(line);
}

MyArrow::~MyArrow()
{
    startItem = NULL;
    endItem = NULL;

}
