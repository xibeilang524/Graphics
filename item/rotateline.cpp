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
    rotatePixmap.load(":/images/rotate32.png");
    rotatePixmap = rotatePixmap.scaled(rotatePixmap.width()/3*2,rotatePixmap.height()/3*2);

    isMoveable = true;
    isEnter = false;

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
//        painter->drawPixmap(QRectF(-rotatePixmap.width()/2,-rotatePixmap.height()/2,rotatePixmap.width(),rotatePixmap.height()),rotatePixmap,rotatePixmap.rect());
    }

    painter->restore();
}

void RotateLine::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    isEnter = true;
    update();
    this->setCursor(QCursor(rotatePixmap));
//    this->setCursor(Qt::PointingHandCursor);
    QGraphicsPolygonItem::hoverEnterEvent(event);
}

void RotateLine::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    isEnter = false;
    update();
    this->setCursor(Qt::ArrowCursor);
    QGraphicsPolygonItem::hoverLeaveEvent(event);
}

void RotateLine::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    startPoint = event->pos();

    emit rotateItem(MOUSE_PRESS,0);
    QGraphicsPolygonItem::mousePressEvent(event);
}

//【利用鼠标的scene坐标和item的夹角关系，可以实现旋转】
void RotateLine::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    QPointF tmpPoint = event->scenePos() - parentItem->pos();

    qreal posY = tmpPoint.y();
    qreal posX = tmpPoint.x();

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

    if(isMoveable)
    {
        emit rotateItem(MOUSE_MOVE,rotateDegree);
    }

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
