#include "mypathitem.h"

#include <QPainter>
#include <QDebug>

#include "myitem.h"

MyPathItem::MyPathItem(MyItem *startItem, MyItem *endItem, QObject * parent, QGraphicsItem * parent1):
    startItem(startItem),
    endItem(endItem),
    QObject(parent),
    QGraphicsPathItem(parent1)
{
    boundRect =  QRectF(-200,-200,400,400);

    painterPath.addRect(-200,-200,20,2);
    painterPath.addRect(-180,-200,2,400);
    painterPath.addRect(-180,198,200,2);

//    setFlag(QGraphicsItem::ItemIsMovable,true);
    setFlag(QGraphicsItem::ItemIsSelectable,true);
}

void MyPathItem::setPath(const QPainterPath &path)
{
    painterPath = path;
    QGraphicsPathItem::setPath(path);
}

QRectF MyPathItem::boundingRect()const
{
    return this->boundRect;
}

QPainterPath MyPathItem::shape()const
{
    return painterPath;
}

void MyPathItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->save();

    if(property.isNeedBorder)
    {
        QPen itemPen;
        itemPen.setWidth(5);
        itemPen.setColor(Qt::red);
        painter->setPen(property.itemPen);
    }
    else
    {
        painter->setPen(Qt::NoPen);
    }

    if(property.isNeedBrush)
    {
        painter->setBrush(property.itemBrush);
    }

    QPen itemPen;
    itemPen.setWidth(3);
    itemPen.setColor(Qt::red);
    painter->setPen(itemPen);

    painter->drawPath(painterPath);

    painter->restore();
}

void MyPathItem::setProperty(ItemProperty property)
{
    this->property = property;

    update();
}

MyPathItem::~MyPathItem()
{

}
