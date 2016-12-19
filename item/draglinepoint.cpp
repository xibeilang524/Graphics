#include "draglinepoint.h"

#include <QPainter>
#include <QCursor>
#include <QGraphicsSceneMouseEvent>
#include <QDebug>

#include "myitem.h"
#include "mypathitem.h"

DragLinePoint::DragLinePoint(const PointType pointType, GraphicsType type, MyItem *parent1, QObject *parent):
    pointType(pointType),
    MyNodeLine(type,parent1,parent)
{
    radius = 4.5;

    prepareGeometryChange();
    boundRect = QRectF(-radius,-radius,radius *2,radius *2);

    QPainterPath path;
    path.addEllipse(boundRect);
    itemPolygon = path.toFillPolygon();
    setPolygon(itemPolygon);

    setFlags(QGraphicsItem::ItemIsSelectable |
             QGraphicsItem::ItemSendsGeometryChanges |
             QGraphicsItem::ItemIsFocusable);

    setAcceptHoverEvents(true);

    connect(this,SIGNAL(currMouseState(MouseType)),parent1,SLOT(procDragLineMouseState(MouseType)));
}

QRectF DragLinePoint::boundingRect()const
{
    return boundRect;
}

void DragLinePoint::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget)
    Q_UNUSED(option)
    painter->save();

    painter->setBrush(GLOBAL_ITEM_BRUSH);

    painter->drawPolygon(itemPolygon);

    painter->restore();
}

//改变鼠标的样式
void DragLinePoint::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    Q_UNUSED(event)
    setCursor(Qt::CrossCursor);
}

void DragLinePoint::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    Q_UNUSED(event)
    this->setCursor(Qt::ArrowCursor);
}

void DragLinePoint::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED(event)

    emit currMouseState(MOUSE_PRESS);

    MySuperItem::mousePressEvent(event);
}

void DragLinePoint::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED(event)

    emit currMouseState(MOUSE_MOVE);

    MySuperItem::mouseMoveEvent(event);
}

void DragLinePoint::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED(event)

    emit currMouseState(MOUSE_RELEASE);

    MySuperItem::mouseReleaseEvent(event);
}

//更新线条的位置
void DragLinePoint::updateLinePos()
{
    foreach(MyPathItem * pathItem,pathLines)
    {
        pathItem->updateCurrItemPos();
    }
}

DragLinePoint::~DragLinePoint()
{

}
