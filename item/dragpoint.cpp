#include "dragpoint.h"

#include <QPainter>
#include <QCursor>
#include <QGraphicsSceneMouseEvent>
#include <QDebug>

#include "myitem.h"

DragPoint::DragPoint(const PointType pointType, GraphicsType type, MyItem *parent, QObject *parent1):
    pointType(pointType),
    MySuperItem(type,parent,parent1)
{
    radius = 3;

    prepareGeometryChange();
    boundRect = QRectF(-radius,-radius,radius *2,radius *2);

    itemPolygon<<QPointF(-radius,-radius)<<QPointF(radius,-radius)
            <<QPointF(radius,radius)<<QPointF(-radius,radius);
    setPolygon(itemPolygon);

    setFlags(QGraphicsItem::ItemIsSelectable |
             QGraphicsItem::ItemSendsGeometryChanges |
             QGraphicsItem::ItemIsFocusable
             /*QGraphicsItem::ItemIsMovable*/);

    setAcceptHoverEvents(true);

    switch(pointType)
    {
        case TOP_LEFT:
        case BOTTOM_RIGHT: cursorShape = Qt::SizeFDiagCursor;
             break;
        case TOP_MIDDLE:
        case BOTTOM_MIDDLE: cursorShape = Qt::SizeVerCursor;
            break;
        case TOP_RIGHT:
        case BOTTOM_LEFT: cursorShape = Qt::SizeBDiagCursor;
            break;
        case MIDDLE_LEFT:
        case MIDDLE_RIGHT: cursorShape = Qt::SizeHorCursor;
            break;
    }

    connect(this,SIGNAL(resizeItemSize()),parent,SLOT(procResizeItem()));
    connect(this,SIGNAL(currMouseState(MouseType,PointType,QPointF)),parent,SLOT(procMouseState(MouseType,PointType,QPointF)));
}

//动态根据角度更新鼠标的样式
void DragPoint::updateDragPointHoverCursor(qreal parentRotateDegree)
{
    parentRotateDegree = (int)parentRotateDegree % 180;

    if(parentRotateDegree >=0 && parentRotateDegree < 30 )
    {
        switch(pointType)
        {
            case TOP_LEFT:
            case BOTTOM_RIGHT: cursorShape = Qt::SizeFDiagCursor;
                 break;
            case TOP_MIDDLE:
            case BOTTOM_MIDDLE: cursorShape = Qt::SizeVerCursor;
                break;
            case TOP_RIGHT:
            case BOTTOM_LEFT: cursorShape = Qt::SizeBDiagCursor;
                break;
            case MIDDLE_LEFT:
            case MIDDLE_RIGHT: cursorShape = Qt::SizeHorCursor;
                break;
        }
    }
    else if(parentRotateDegree >=30 && parentRotateDegree < 60)
    {
        switch(pointType)
        {
            case TOP_LEFT:
            case BOTTOM_RIGHT: cursorShape = Qt::SizeVerCursor;
                 break;
            case TOP_MIDDLE:
            case BOTTOM_MIDDLE: cursorShape = Qt::SizeBDiagCursor;
                break;
            case TOP_RIGHT:
            case BOTTOM_LEFT: cursorShape = Qt::SizeHorCursor;
                break;
            case MIDDLE_LEFT:
            case MIDDLE_RIGHT: cursorShape = Qt::SizeFDiagCursor;
                break;
        }
    }
    else if(parentRotateDegree >=60 && parentRotateDegree < 120)
    {
        switch(pointType)
        {
            case TOP_LEFT:
            case BOTTOM_RIGHT: cursorShape = Qt::SizeBDiagCursor;
                 break;
            case TOP_MIDDLE:
            case BOTTOM_MIDDLE: cursorShape = Qt::SizeHorCursor;
                break;
            case TOP_RIGHT:
            case BOTTOM_LEFT: cursorShape = Qt::SizeFDiagCursor;
                break;
            case MIDDLE_LEFT:
            case MIDDLE_RIGHT: cursorShape = Qt::SizeVerCursor;
                break;
        }
    }
    else if(parentRotateDegree >=120 && parentRotateDegree < 150)
    {
        switch(pointType)
        {
            case TOP_LEFT:
            case BOTTOM_RIGHT: cursorShape = Qt::SizeHorCursor;
                 break;
            case TOP_MIDDLE:
            case BOTTOM_MIDDLE: cursorShape = Qt::SizeFDiagCursor;
                break;
            case TOP_RIGHT:
            case BOTTOM_LEFT: cursorShape = Qt::SizeVerCursor;
                break;
            case MIDDLE_LEFT:
            case MIDDLE_RIGHT: cursorShape = Qt::SizeBDiagCursor;
                break;
        }
    }
    else if(parentRotateDegree >=150 && parentRotateDegree <= 180)
    {
        switch(pointType)
        {
            case TOP_LEFT:
            case BOTTOM_RIGHT: cursorShape = Qt::SizeFDiagCursor;
                 break;
            case TOP_MIDDLE:
            case BOTTOM_MIDDLE: cursorShape = Qt::SizeVerCursor;
                break;
            case TOP_RIGHT:
            case BOTTOM_LEFT: cursorShape = Qt::SizeBDiagCursor;
                break;
            case MIDDLE_LEFT:
            case MIDDLE_RIGHT: cursorShape = Qt::SizeHorCursor;
                break;
        }
    }
}

//改变鼠标的样式
void DragPoint::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    Q_UNUSED(event)
    setCursor(cursorShape);
}

void DragPoint::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    Q_UNUSED(event)
    this->setCursor(Qt::ArrowCursor);
}

QRectF DragPoint::boundingRect()const
{
    return boundRect;
}

void DragPoint::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget)
    Q_UNUSED(option)
    painter->save();

    painter->setBrush(GLOBAL_ITEM_BRUSH);

    painter->drawPolygon(itemPolygon);

    painter->restore();
}

//在移动时，如果拖拽本身就不再设置更新位置。
void DragPoint::updatePos(PointType type, QPointF point)
{
    if( type != pointType)
    {
        setPos(point);
    }
}

QVariant DragPoint::itemChange(GraphicsItemChange change, const QVariant &value)
{
    if(change == ItemPositionHasChanged && scene())
    {

    }

    return QGraphicsPolygonItem::itemChange(change,value);
}

void DragPoint::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    emit currMouseState(MOUSE_PRESS,pointType,event->pos());
    QGraphicsPolygonItem::mousePressEvent(event);
}

void DragPoint::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    emit currMouseState(MOUSE_MOVE,pointType,event->pos());
    QGraphicsPolygonItem::mouseMoveEvent(event);
}

void DragPoint::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    emit resizeItemSize();
    emit currMouseState(MOUSE_RELEASE,pointType,event->pos());
    QGraphicsPolygonItem::mouseReleaseEvent(event);
}

DragPoint::~DragPoint()
{

}
