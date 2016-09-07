#include "myarrow.h"

#include <QPainter>
#include <QRectF>
#include <QLineF>
#include <QDebug>

#include "myitem.h"

const qreal Pi = 3.14;

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
        QLineF centerLine(startItem->pos(), endItem->pos());
        QPolygonF endPolygon = endItem->polygon();
        QPointF p1 = endPolygon.first() + endItem->pos();   //将polygons中item的坐标系转换成scene的坐标系
        QPointF p2;
        QPointF intersectPoint;
        QLineF polyLine;
        //计算起点多边形的中点和终点多边形的交点
        for (int i = 1; i <= endPolygon.count(); ++i)
        {
            //最后一个需要和第一个进行连线
            if(i == endPolygon.count())
            {
                p1 = endPolygon.first() + endItem->pos();
            }
            else
            {
                p2 = endPolygon.at(i) + endItem->pos();
            }

            polyLine = QLineF(p1, p2);
            QLineF::IntersectType intersectType = polyLine.intersect(centerLine, &intersectPoint);
            if (intersectType == QLineF::BoundedIntersection)
            {
                break;
            }

            p1 = p2;
        }

        setLine(QLineF(intersectPoint, startItem->pos()));

        qreal arrowSize = 20;

        double angle = ::acos(line().dx() / line().length());
        if (line().dy() >= 0)
        {
            angle = (Pi * 2) - angle;
        }

            QPointF arrowP1 = line().p1() + QPointF(sin(angle + Pi / 3) * arrowSize,
                                            cos(angle + Pi / 3) * arrowSize);
            QPointF arrowP2 = line().p1() + QPointF(sin(angle + Pi - Pi / 3) * arrowSize,
                                            cos(angle + Pi - Pi / 3) * arrowSize);

            arrowHead.clear();
            arrowHead << line().p1() << arrowP1 << arrowP2;
            painter->setBrush(Qt::black);
            painter->drawLine(line());
            painter->drawPolygon(arrowHead);
            if (isSelected())
            {
                painter->setPen(QPen(Qt::blue, 1, Qt::DashLine));
                QLineF myLine = line();
                myLine.translate(0, 4.0);
                painter->drawLine(myLine);
                myLine.translate(0,-8.0);
                painter->drawLine(myLine);
            }
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
