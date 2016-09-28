#include "rotateline.h"

#include <QPainter>
#include <QCursor>
#include <QGraphicsSceneMouseEvent>

#include <QDebug>

#include "math.h"

#define ATAN(degree)  atan(degree)*180/PI

RotateLine::RotateLine(QGraphicsItem *parent):
    parentItem(parent),
    QGraphicsObject(parent)
{
    boundRect = QRectF(-ROTATE_WIDTH/2,-ROTATE_WIDTH/2,ROTATE_WIDTH,ROTATE_WIDTH);

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

//���ÿؼ��Ƿ�����
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
        painter->drawRect(boundRect);
    }

    painter->restore();
}

void RotateLine::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
//    this->setCursor(QCursor(pixmap));
    QGraphicsObject::hoverEnterEvent(event);
}

void RotateLine::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    this->setCursor(Qt::ArrowCursor);
    QGraphicsObject::hoverLeaveEvent(event);
}

void RotateLine::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    startPoint = event->pos();

    qDebug()<<pos();

    emit rotateItem(MOUSE_PRESS,0);
    QGraphicsObject::mousePressEvent(event);
}

void RotateLine::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
//    qreal posY = mapToParent(event->pos()).y();
//    qreal posX = mapToParent(event->pos()).x();

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

    QGraphicsObject::mouseMoveEvent(event);
}

void RotateLine::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    emit rotateItem(MOUSE_RELEASE,0);
    QGraphicsObject::mouseReleaseEvent(event);
}


RotateLine::~RotateLine()
{

}
