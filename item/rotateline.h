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

    QRectF boundingRect()const;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

protected:
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

signals:
    void rotateItem(int);

private:
    QGraphicsItem * parentItem;
    QRectF boundRect;

    QPointF startPoint;

    int rotateDegree;
    bool hasSelected;

    QPixmap pixmap;
};

#endif // ROTATELINE_H
