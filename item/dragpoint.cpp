#include "dragpoint.h"

#include <QPainter>
#include <QCursor>
#include <QDebug>

#include "myitem.h"

DragPoint::DragPoint(const PointType pointType, MyItem *parent):
    pointType(pointType),
    QGraphicsObject(parent)
{

    radius = 3;
    isPressed= false;

    boundRect = QRectF(-radius,-radius,radius *2,radius *2);

    setFlags(QGraphicsItem::ItemIsSelectable |
             QGraphicsItem::ItemSendsGeometryChanges |
             QGraphicsItem::ItemIsFocusable |
             QGraphicsItem::ItemIsMovable);

    setAcceptHoverEvents(true);

    connect(this,SIGNAL(pointPosChanged(PointType)),parent,SLOT(procDragSize(PointType)));
    connect(this,SIGNAL(resizeItemSize()),parent,SLOT(procResizeItem()));
    connect(this,SIGNAL(currMouseState(MouseType)),parent,SLOT(procMouseState(MouseType)));
}

void DragPoint::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    switch(pointType)
    {
        case TOP_LEFT:
        case BOTTOM_RIGHT:setCursor(Qt::SizeFDiagCursor);
             break;
        case TOP_MIDDLE:
        case BOTTOM_MIDDLE:setCursor(Qt::SizeVerCursor);
            break;
        case TOP_RIGHT:
        case BOTTOM_LEFT:setCursor(Qt::SizeBDiagCursor);
            break;
        case MIDDLE_LEFT:
        case MIDDLE_RIGHT: setCursor(Qt::SizeHorCursor);
            break;
    }
}

void DragPoint::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    this->setCursor(Qt::ArrowCursor);
}

QRectF DragPoint::boundingRect()const
{
    return boundRect;
}

void DragPoint::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->save();

    painter->setBrush(Qt::black);

    painter->drawEllipse(boundRect);

    painter->restore();
}

//在移动时，如果拖拽本身就不再设置更新位置。
void DragPoint::updatePos(PointType type, QPointF point)
{
    if( type != pointType)
    {
        setPos(point);
    }
}

QVariant DragPoint::itemChange(GraphicsItemChange change, const QVariant &value)
{
    if(isPressed && change == ItemPositionHasChanged && scene())
    {
        emit pointPosChanged(pointType);
    }

    return QGraphicsObject::itemChange(change,value);
}

void DragPoint::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    isPressed = true;
    emit currMouseState(MOUSE_PRESS);
    QGraphicsItem::mousePressEvent(event);
}

void DragPoint::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    emit currMouseState(MOUSE_MOVE);
    QGraphicsItem::mouseMoveEvent(event);
}

void DragPoint::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    isPressed = false;
    emit resizeItemSize();
    emit currMouseState(MOUSE_RELEASE);
    QGraphicsItem::mouseReleaseEvent(event);
}
