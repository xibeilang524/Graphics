#include "myarrow.h"

#include <QPainter>
#include <QRectF>
#include <QLineF>
#include <QDebug>

#include "myitem.h"
#include "mynodeport.h"

#include "math.h"

const qreal Pi = 3.14;

QDataStream & operator <<(QDataStream &stream,MyArrow * item)
{
    int type = item->type;

    stream<<type<<item->property;

    return stream;
}

QDataStream & operator >>(QDataStream &stream,MyArrow * item)
{
    int type;
    ItemProperty prop;

    stream>>type>>prop;
    item->type = (GraphicsType)type;

    item->setProperty(prop);

    return stream;
}

MyArrow::MyArrow(MyItem  * startItem,MyItem  * endItem,QGraphicsItem *parent):
    startItem(startItem),
    endItem(endItem),
    QGraphicsLineItem(parent)
{
    setFlag(QGraphicsItem::ItemIsSelectable, true);

    type = GRA_LINE;
    property.itemBrush = QBrush(Qt::black);
    //直线保存两端控件的引用
    property.startItemID = startItem->getProperty().startItemID;
    property.endItemID = endItem->getProperty().startItemID;

    lineType = LINE_MYITEM;
}

MyArrow::MyArrow(MyNodePort  * startItem,MyNodePort  * endItem,QGraphicsItem *parent):
    startNodePort(startItem),
    endNodePort(endItem),
    QGraphicsLineItem(parent)
{
    setFlag(QGraphicsItem::ItemIsSelectable, true);

    type = GRA_LINE;
    property.itemBrush = QBrush(Qt::black);
    //直线保存两端控件的引用
    property.startItemID = startItem->getProperty().startItemID;
    property.endItemID = endItem->getProperty().startItemID;

    lineType = LINE_NODEPORT;
}

QRectF MyArrow::boundingRect()const
{
    return QRectF(line().p1(), QSizeF(line().p2().x() - line().p1().x(),
                                           line().p2().y() - line().p1().y()));
}

QPainterPath MyArrow::shape()const
{
    QPainterPath path = QGraphicsLineItem::shape();
    if(lineType == LINE_MYITEM)
    {
        path.addPolygon(arrowHead);
    }
    return path;
}

void MyArrow::setProperty(ItemProperty property)
{
    this->property = property;

    update();
}

void MyArrow::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->save();

    if(lineType == LINE_MYITEM && startItem && endItem)
    {
        QLineF centerLine(startItem->pos(), endItem->pos());
        QPolygonF endPolygon = endItem->polygon();
        //将polygons中item的坐标系转换成scene的坐标系，支持即使控件旋转后依然可以保持箭头指向某一边
        QPointF p1 = endItem->mapToScene(endPolygon.first());
        QPointF p2;
        QPointF intersectPoint;
        QLineF polyLine;
        //计算起点多边形的中点和终点多边形的交点
        for (int i = 1; i <= endPolygon.count(); ++i)
        {
            //最后一个需要和第一个进行连线
            if(i == endPolygon.count())
            {
                p1 = endItem->mapToScene(endPolygon.first());
            }
            else
            {
                p2 = endItem->mapToScene(endPolygon.at(i));
            }

            polyLine = QLineF(p1, p2);
            QLineF::IntersectType intersectType = polyLine.intersect(centerLine, &intersectPoint);
            if (intersectType == QLineF::BoundedIntersection)
            {
                break;
            }

            p1 = p2;
        }
        //将交点和起点作为线段的端点
        setLine(QLineF(intersectPoint, startItem->pos()));
        if (isSelected())
        {
            painter->setPen(QPen(Qt::blue, 1, Qt::DashLine));
            QLineF myLine = line();
            myLine.translate(0, 4.0);
            painter->drawLine(myLine);
            myLine.translate(0,-8.0);
            painter->drawLine(myLine);
        }

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

            painter->setPen(property.itemPen);
            painter->drawLine(line());

            painter->setBrush(property.itemBrush);
            painter->drawPolygon(arrowHead);

    }
    else if(lineType == LINE_NODEPORT && startNodePort && endNodePort)
    {
        QPointF startPoint = startNodePort->getParentItem()->mapToScene(startNodePort->pos());
        QPointF endPoint = endNodePort->getParentItem()->mapToScene(endNodePort->pos());

        painter->setPen(Qt::black);
        QLineF centerLine(startPoint, endPoint);
        painter->drawLine(centerLine);
    }
    painter->restore();
}

void MyArrow::updatePosition()
{
    if(lineType == LINE_NODEPORT)
    {
        QLineF line(mapFromItem(startNodePort, 0, 0), mapFromItem(endNodePort, 0, 0));
        setLine(line);
    }
    else if(lineType == LINE_MYITEM)
    {
        QLineF line(mapFromItem(startItem, 0, 0), mapFromItem(endItem, 0, 0));
        setLine(line);
    }
}

MyArrow::~MyArrow()
{
    startItem = NULL;
    endItem = NULL;

}
