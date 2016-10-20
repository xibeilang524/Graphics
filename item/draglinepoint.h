/*************************************************
**版  权：RenGu Company
**文件名: draglinepoint.h
**作  者: wey       Version: 1.0       Date: 2016.09.30
**描  述:
**Others: 拖拽线条节点
**
**修改历史:
**
*************************************************/
#ifndef DRAGLINEPOINT_H
#define DRAGLINEPOINT_H

#include "mynodeline.h"
#include "ItemHeader.h"

class MyItem;

class DragLinePoint : public MyNodeLine
{
    Q_OBJECT
public:
    DragLinePoint(const PointType pointType,GraphicsType type, MyItem * parent1 = 0 ,QObject* parent = 0);
    ~DragLinePoint();

    QRectF boundingRect()const;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    PointType getPointType(){return this->pointType;}

protected:
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);

private:
    PointType pointType;
};

#endif // DRAGLINEPOINT_H
