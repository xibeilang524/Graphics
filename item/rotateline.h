/*************************************************
**版  权：RenGu Company
**文件名: rotateline.h
**作  者: wey       Version: 1.0       Date: 2016.09.01
**描  述: 控件旋转控制点
**Others: 1：用于控制控件的旋转
**
**修改历史:
**20160922:wey:添加状态锁定，锁定后图标改变
*************************************************/
#ifndef ROTATELINE_H
#define ROTATELINE_H

#include <QGraphicsObject>
#include <QPixmap>

#include "ItemHeader.h"

class RotateLine : public QGraphicsObject
{
    Q_OBJECT
public:
    RotateLine(QGraphicsItem * parent = 0);
    ~RotateLine();

    void setMoveable(bool moveable);

    QRectF boundingRect()const;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

protected:
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

signals:
    void rotateItem(MouseType,int);

private:
    QGraphicsItem * parentItem;
    QRectF boundRect;

    QPointF startPoint;

    int rotateDegree;
    bool hasSelected;
    bool isMoveable;

    QPixmap pixmap;
};

#endif // ROTATELINE_H
