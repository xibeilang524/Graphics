#include "myarrow.h"

#include <QPainter>
#include <QRectF>
#include <QLineF>

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
    path.addPolygon(arrowHead);
    return path;
}

void MyArrow::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->save();

    //»æÖÆÖ±Ïß
    QLineF centerLine(startItem->pos(), endItem->pos());
//    QPolygonF endPolygon = endItem->polygon();
//    QPointF p1 = endPolygon.first() + endItem->pos();
//    QPointF p2;
//    QPointF intersectPoint;
//    QLineF polyLine;
//    for (int i = 1; i < endPolygon.count(); ++i)
//    {
//        p2 = endPolygon.at(i) + endItem->pos();
//        polyLine = QLineF(p1, p2);
//        QLineF::IntersectType intersectType =
//            polyLine.intersect(centerLine, &intersectPoint);
//        if (intersectType == QLineF::BoundedIntersection)
//        {
//            break;
//        }
//            p1 = p2;
//    }

//    setLine(QLineF(intersectPoint, myStartItem->pos()));

    painter->restore();
}

MyArrow::~MyArrow()
{

}
