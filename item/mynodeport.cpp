#include "mynodeport.h"

#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QDebug>

MyNodePort::MyNodePort(QGraphicsItem *parentItem):
    QGraphicsObject(parentItem)
{

//    setFlag(QGraphicsItem::ItemIsMovable,true);
    setFlag(QGraphicsItem::ItemIsSelectable,true);

    radius = 15;

    boundRect = QRectF(-radius,-radius,2*radius,2*radius);
}

//设置端口拖入的方向
void MyNodePort::setDragDirect(DragDirect direct)
{
    this->dragDirect = direct;
}

//设置显示的中心点
void MyNodePort::setPos(const QPointF &pos)
{
    QGraphicsObject::setPos(pos);
}

//设置拖入位置与拖入边长度的比例
void MyNodePort::setScaleFactor(qreal scaleFactor)
{
    this->scaleFactor = scaleFactor;
}

//设置端口的画刷
void MyNodePort::setBrush(const QBrush &brush)
{
    this->brush = brush;
    update();
}

QRectF MyNodePort::boundingRect()const
{
    return boundRect;
}

void MyNodePort::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->save();

    painter->setBrush(brush);

    painter->drawRect(boundRect);

    painter->restore();
}

void MyNodePort::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    emit portPosChanged(MOUSE_PRESS,event->pos());
    QGraphicsObject::mousePressEvent(event);
}

void MyNodePort::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    emit portPosChanged(MOUSE_MOVE,event->pos());
    QGraphicsObject::mouseMoveEvent(event);
}

void MyNodePort::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    emit portPosChanged(MOUSE_RELEASE,event->pos());
    QGraphicsObject::mouseReleaseEvent(event);
}

void MyNodePort::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsObject::mouseDoubleClickEvent(event);
}

MyNodePort::~MyNodePort()
{

}
