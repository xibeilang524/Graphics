#include "rotateline.h"

#include <QPainter>
#include <QCursor>
#include <QGraphicsSceneMouseEvent>

#include <QDebug>

#include "math.h"

RotateLine::RotateLine(QGraphicsItem *parent):
    parentItem(parent),
    QGraphicsObject(parent)
{
    boundRect = QRectF(0,0,ROTATE_WIDTH,ROTATE_WIDTH);

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
    painter->save();

    painter->setPen(Qt::black);

    int posX = boundRect.center().x();
    int posY = boundingRect().bottomRight().y();

    painter->drawRect(QRectF(posX-ROTATE_SQUARE_WIDTH/2,posY-ROTATE_SQUARE_WIDTH,ROTATE_SQUARE_WIDTH,ROTATE_SQUARE_WIDTH));

    int pixW = ROTATE_WIDTH - ROTATE_SQUARE_WIDTH;
    posX = posX - pixW/2;
    posY = boundingRect().topLeft().y();

    painter->drawPixmap(QRectF(posX,posY,pixW,pixW),pixmap,pixmap.rect());

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

    QGraphicsObject::mousePressEvent(event);
}

void RotateLine::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    int posY = event->pos().y();
    int posX = event->pos().x();

    if(posX == 0)
    {
        return;
    }
    double tmpValue = (double)(posY-ROTATE_WIDTH) / (posX-ROTATE_WIDTH);

    rotateDegree = atan(tmpValue)*180/PI;

    //4
//    if(posX >=0 && posY>0)
//    {
//        rotateDegree += 90;
//    }
//    else if(posX >=0 && posY<0)
//    {

//    }
//    else if(posX < 0 && posY<0)
//    {
//        rotateDegree += 270;
//    }
//    else if(posX < 0 && posY>0)
//    {
//        rotateDegree += 180;
//    }

    QPointF tmpPoint = mapFromScene(event->pos());

//    qDebug()<< rotateDegree<<"==="<<posX<<"=="<<posY<<"__"<<tmpPoint.x()<<"--"<<tmpPoint.y();

//    emit rotateItem(rotateDegree);

    startPoint = event->pos();

    QGraphicsObject::mouseMoveEvent(event);
}

void RotateLine::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsObject::mouseReleaseEvent(event);
}


RotateLine::~RotateLine()
{

}
