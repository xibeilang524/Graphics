#include "rotateline.h"

#include <QPainter>
#include <QCursor>
#include <QGraphicsSceneMouseEvent>

#include <QDebug>

#include "math.h"

#define ATAN(degree)  atan(degree)*180/PI

RotateLine::RotateLine(GraphicsType type, QGraphicsItem *parent, QObject *parent1):
    MySuperItem(type,parent,parent1)
{
    radius = ROTATE_WIDTH/2;
    prepareGeometryChange();
    boundRect = QRectF(-radius,-radius,2*radius,2*radius);

    currItemType = GRA_ROTATE_POINT;

    itemPolygon<<QPointF(-radius,-radius)<<QPointF(radius,-radius)
            <<QPointF(radius,radius)<<QPointF(-radius,radius);
    setPolygon(itemPolygon);

    pixmap.load(":/images/itemLock.png");

    isMoveable = true;

    setFlags(ItemIsSelectable);
    setAcceptHoverEvents(true);

    rotateDegree = 0;
}

QRectF RotateLine::boundingRect()const
{
    return boundRect;
}

//设置控件是否锁定
void RotateLine::setMoveable(bool moveable)
{
    isMoveable = moveable;
}

void RotateLine::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)

    painter->save();

    painter->setPen(Qt::black);

    if(!isMoveable)
    {
        painter->drawPixmap(QRectF(-pixmap.width()/2,-pixmap.height()/2,pixmap.width(),pixmap.height()),pixmap,pixmap.rect());
    }
    else
    {
        painter->setBrush(Qt::blue);
        painter->drawPolygon(itemPolygon);
    }

    painter->restore();
}

void RotateLine::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    this->setCursor(Qt::PointingHandCursor);
    QGraphicsPolygonItem::hoverEnterEvent(event);
}

void RotateLine::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    this->setCursor(Qt::ArrowCursor);
    QGraphicsPolygonItem::hoverLeaveEvent(event);
}

void RotateLine::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    startPoint = event->pos();

    emit rotateItem(MOUSE_PRESS,0);
    QGraphicsPolygonItem::mousePressEvent(event);
}

void RotateLine::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    qreal posY = mapToParent(event->pos()).y();
    qreal posX = mapToParent(event->pos()).x();

    qDebug()<<mapToParent(event->pos())<<"__"<<event->pos();

    if(posX == 0)
    {
        return;
    }

    qreal tmpValue = posY / posX;

    rotateDegree = ATAN(tmpValue) ;

    if(posX >=0 && posY>0)
    {
        rotateDegree += 90;
    }
    else if(posX >=0 && posY<0)
    {
        rotateDegree = 90 + rotateDegree;
    }
    else if(posX < 0 && posY<0)
    {
        rotateDegree = 270 + rotateDegree;
    }
    else if(posX < 0 && posY>0)
    {
        rotateDegree = 270 + rotateDegree;
    }

    qDebug()<<rotateDegree;

    emit rotateItem(MOUSE_MOVE,rotateDegree);

//    startPoint = event->pos();

    QGraphicsPolygonItem::mouseMoveEvent(event);
}

void RotateLine::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    emit rotateItem(MOUSE_RELEASE,0);
    QGraphicsPolygonItem::mouseReleaseEvent(event);
}

RotateLine::~RotateLine()
{

}
