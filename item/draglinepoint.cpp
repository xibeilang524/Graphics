#include "draglinepoint.h"

#include <QPainter>
#include <QCursor>
#include <QDebug>

#include "myitem.h"

DragLinePoint::DragLinePoint(const PointType pointType, MyItem *parent):
    pointType(pointType),
    QGraphicsObject(parent)
{
    radius = 2;
    boundRect = QRectF(-radius,-radius,radius *2,radius *2);

    setFlags(QGraphicsItem::ItemIsSelectable |
             QGraphicsItem::ItemSendsGeometryChanges |
             QGraphicsItem::ItemIsFocusable);

    setAcceptHoverEvents(true);

}
QRectF DragLinePoint::boundingRect()const
{
    return boundRect;
}

void DragLinePoint::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget)
    Q_UNUSED(option)
    painter->save();

    painter->setBrush(GLOBAL_ITEM_BRUSH);

    painter->drawEllipse(boundRect);

    painter->restore();
}

//改变鼠标的样式
void DragLinePoint::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    Q_UNUSED(event)
    setCursor(Qt::CrossCursor);
}

void DragLinePoint::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    Q_UNUSED(event)
    this->setCursor(Qt::ArrowCursor);
}

