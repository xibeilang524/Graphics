#ifndef MYITEM_H
#define MYITEM_H

#include <QGraphicsObject>
#include <QPen>

#include "ItemHeader.h"

class QGraphicsItem;
class DragPoint;
class RotateLine;
class QMenu;
class MyArrow;

#include "dragpoint.h"
#include "rotateline.h"

class MyItem : public QGraphicsObject
{
    Q_OBJECT
public:
    MyItem(QMenu * menu,QGraphicsItem * parent = 0);
    ~MyItem();

    QRectF boundingRect() const;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    void addArrow(MyArrow * arrow);
    void removeArrows();
    void removeArrow(MyArrow * arrow);

signals:
    void updateSceneDraw();

private slots:
    void procDragSize(PointType type);
    void procResizeItem();
    void procRotate(int degree);
    void procMouseState(MouseType);

protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);

private:
    void setDragPointVisible(bool flag);
    void updateRotateLinePos();

    int radius;

    QRectF boundRect;

    QPen selectedPen;
    int selectedPenWidth;

    DragPoint * leftTopPoint;
    DragPoint * rightTopPoint;
    DragPoint * leftBottomPoint;
    DragPoint * rightBottomPoint;

    RotateLine * rotateLine;

    QMenu * rightMenu;

    MouseType currMouseType;

    QList<MyArrow *> arrows;           //保存添加的箭头

    bool isNeedBorder;
};

#endif // MYITEM_H
