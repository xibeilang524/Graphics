/*************************************************
**版  权：RenGu Company
**文件名: dragpoint.h
**作  者: wey       Version: 1.0       Date: 2016.09.01
**描  述: 缩放点
**Others: 1：用于缩放控件
**
**修改历史:
**
*************************************************/
#ifndef DRAGPOINT_H
#define DRAGPOINT_H

#include <QGraphicsObject>

#include "ItemHeader.h"

class MyItem;

class DragPoint : public QGraphicsObject
{
    Q_OBJECT
public:
    DragPoint(const PointType pointType, MyItem * parent = 0 );
    void updatePos(PointType type,QPointF point);

    QRectF boundingRect()const;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

signals:
    void currMouseState(MouseType,PointType,QPointF);
    void resizeItemSize();

protected:
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);

    QVariant itemChange(GraphicsItemChange change, const QVariant &value);

    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

private:
    QRectF boundRect;

    int radius;

    PointType pointType;
};

#endif // DRAGPOINT_H
