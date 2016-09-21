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

    if(pixmap.load(":/img/rotate32.png"))
    {
        int normalSize = ROTATE_WIDTH;
        pixmap = pixmap.scaled(normalSize,normalSize);
    }

    setFlags(ItemIsSelectable /*| ItemIsMovable*/);

    setAcceptHoverEvents(true);

    rotateDegree = 0;
}

QRectF RotateLine::boundingRect()const
{
    return boundRect;
}

void RotateLine::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)

    painter->save();

    painter->setPen(Qt::black);

    int posX = boundRect.center().x();
    int posY = boundingRect().bottomRight().y();

    painter->setBrush(Qt::blue);
    painter->drawRect(boundRect);

//    painter->drawRect(QRectF(posX-ROTATE_SQUARE_WIDTH/2,posY-ROTATE_SQUARE_WIDTH,ROTATE_SQUARE_WIDTH,ROTATE_SQUARE_WIDTH));

    int pixW = ROTATE_WIDTH - ROTATE_SQUARE_WIDTH;
    posX = posX - pixW/2;
    posY = boundingRect().topLeft().y();

//    painter->drawPixmap(QRectF(posX,posY,pixW,pixW),pixmap,pixmap.rect());

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

//    qDebug()<<startPoint.x()<<"============"<<startPoint.y();

    emit rotateItem(MOUSE_PRESS,0);
    QGraphicsObject::mousePressEvent(event);
}

void RotateLine::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    qreal posY = event->pos().y();
    qreal posX = event->pos().x();

    if(posX == 0)
    {
        return;
    }
    qreal tmpValue = posY / posX;

    rotateDegree = ATAN(tmpValue) ;

    if(posX >=0 && posY>0)
    {
//        rotateDegree += 90;
    }
    else if(posX >=0 && posY<0)
    {
        rotateDegree = 360 + rotateDegree;
    }
    else if(posX < 0 && posY<0)
    {
        rotateDegree = 180 + rotateDegree;
    }
    else if(posX < 0 && posY>0)
    {
        rotateDegree = 180 + rotateDegree;
    }

    qDebug()<<rotateDegree;

    QPointF tmpPoint = mapFromScene(event->pos());

    qDebug()<< rotateDegree<<"==="<<posX<<"=="<<posY<<"__"<<tmpPoint.x()<<"--"<<tmpPoint.y();

    emit rotateItem(MOUSE_MOVE,rotateDegree);

    startPoint = event->pos();

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
