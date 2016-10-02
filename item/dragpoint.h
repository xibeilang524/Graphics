/*************************************************
**版  权：RenGu Company
**文件名: dragpoint.h
**作  者: wey       Version: 1.0       Date: 2016.09.01
**描  述: 缩放点
**Others: 1：用于缩放控件
**
**修改历史:
**20160928:wey:增加随父控件旋转角度变化，动态切换悬停的鼠标样式
**20160930:wey:调整父类成QGraphicsPolygonItem
*************************************************/
#ifndef DRAGPOINT_H
#define DRAGPOINT_H

#include "mysuperitem.h"
#include "ItemHeader.h"

class MyItem;

class DragPoint : public MySuperItem
{
    Q_OBJECT
public:
    DragPoint(const PointType pointType,GraphicsType type, MyItem * parent = 0 ,QObject * parent1 = 0);
    ~DragPoint();
    void updatePos(PointType type,QPointF point);

    QRectF boundingRect()const;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    void updateDragPointHoverCursor(qreal parentRotateDegree);

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
    PointType pointType;
    Qt::CursorShape cursorShape;
};

#endif // DRAGPOINT_H
