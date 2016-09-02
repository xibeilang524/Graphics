#include "myitem.h"

#include <QGraphicsSceneMouseEvent>
#include <QGraphicsSceneHoverEvent>
#include <QGraphicsSceneContextMenuEvent>
#include <QPainter>
#include <QMenu>
#include <QDebug>
#include <QGraphicsScene>

#include "myarrow.h"

MyItem::MyItem(GraphicsType itemType, QMenu *menu, QObject *parent1, QGraphicsItem *parent2):
    currItemType(itemType),
    rightMenu(menu),
    QObject(parent1),
    QGraphicsPolygonItem(parent2)
{
    radius = 100;

    setFlag(QGraphicsItem::ItemIsMovable,true);
    setFlag(QGraphicsItem::ItemIsSelectable,true);

    prepareGeometryChange();
    boundRect = QRectF(-radius,-radius,2*radius,2*radius);   //设置范围时同时也默认指定了其中心点坐标(0,0)

    QPolygonF tmpPoly;

    switch(currItemType)
    {
          case GRA_SQUARE:
                             tmpPoly<<QPointF(-radius,-radius)<<QPointF(radius,-radius)<<
                                      QPointF(radius,radius)<<QPointF(-radius,radius);
                             break;
    }


    setBrush(Qt::red);
    setPolygon(tmpPoly);


    currMouseType = MOUSE_NONE;
    isNeedBorder = false;

    selectedPenWidth = 1;
    selectedPen.setColor(Qt::black);
    selectedPen.setStyle(Qt::DashLine);
    selectedPen.setWidth(selectedPenWidth);

    leftTopPoint = new DragPoint(TOP_LEFT,this);
    rightTopPoint = new DragPoint(TOP_RIGHT,this);
    leftBottomPoint = new DragPoint(BOTTOM_LEFT,this);
    rightBottomPoint = new DragPoint(BOTTOM_RIGHT,this);

    rotateLine = new RotateLine(this);
    connect(rotateLine,SIGNAL(rotateItem(int)),this,SLOT(procRotate(int)));
    updateRotateLinePos();

    setAcceptHoverEvents(true);
    setDragPointVisible(false);

    procResizeItem();
}

void MyItem::updateRotateLinePos()
{
    int posX = boundingRect().center().x() - rotateLine->boundingRect().width()/2;
    int posY = boundingRect().topLeft().y() - 1.5*rotateLine->boundingRect().height();

    rotateLine->setPos(posX,posY);
}

QRectF MyItem::boundingRect()const
{
    return boundRect;
}

//保存添加的箭头
void MyItem::addArrow(MyArrow *arrow)
{
    arrows.push_back(arrow);
}

//删除此控件时，移除所有的箭头
void MyItem::removeArrows()
{
    foreach (MyArrow *arrow, arrows)
    {
        arrow->getStartItem()->removeArrow(arrow);
        arrow->getEndItem()->removeArrow(arrow);
        scene()->removeItem(arrow);
        delete arrow;
    }
}

void MyItem::removeArrow(MyArrow *arrow)
{
    int index = arrows.indexOf(arrow);

    if (index != -1)
        arrows.removeAt(index);
}

//void MyItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
//{
//    painter->save();

//    if(isNeedBorder)
//    {
//       painter->save();
//       painter->setPen(selectedPen);
//       painter->drawRect(boundRect.adjusted(-1,-1,1,1));
//       painter->restore();
//    }

//    painter->setPen(Qt::black);
//    painter->setBrush(Qt::blue);

//    painter->drawRect(boundRect);

//    painter->restore();
//}

void MyItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->buttons() == Qt::LeftButton)
    {
        setDragPointVisible(true);
    }

    QGraphicsPolygonItem::mousePressEvent(event);
}

void MyItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    emit updateSceneDraw();
    QGraphicsPolygonItem::mouseMoveEvent(event);
}

void MyItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    emit updateSceneDraw();
    QGraphicsPolygonItem::mouseReleaseEvent(event);
}

void MyItem::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    QGraphicsPolygonItem::hoverEnterEvent(event);
}

void MyItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    QGraphicsPolygonItem::hoverLeaveEvent(event);
}

//右键菜单事件
void MyItem::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    setSelected(true);
    rightMenu->exec(event->screenPos());
}

void MyItem::setDragPointVisible(bool flag)
{
    leftTopPoint->setVisible(flag);
    rightTopPoint->setVisible(flag);
    leftBottomPoint->setVisible(flag);
    rightBottomPoint->setVisible(flag);

    rotateLine->setVisible(flag);
}

QVariant MyItem::itemChange(GraphicsItemChange change, const QVariant &value)
{
    if(change == ItemSelectedChange && scene())
    {
        setDragPointVisible(true);
        isNeedBorder = true;
    }
    else if(change == ItemSelectedHasChanged && scene())
    {
//        qDebug()<<"====="<<isSelected();
        if(!isSelected()&&(currMouseType == MOUSE_NONE||currMouseType == MOUSE_RELEASE))
        {
            setDragPointVisible(false);
            isNeedBorder = false;
        }
    }
    else if(change  == QGraphicsItem::ItemPositionChange && scene())
    {
        qDebug()<<"====QGraphicsItem::ItemPositionChange==";
    }
    else if(change == QGraphicsItem::ItemPositionHasChanged && scene())
    {
qDebug()<<"=%%%QGraphicsItem::ItemPositionChange==";
    }


    return QGraphicsPolygonItem::itemChange(change,value);
}

void MyItem::procDragSize(PointType type)
{
    QRectF rect(0,0,0,0);

//    QList<QGraphicsItem*> allItems = childItems();

//    foreach (QGraphicsItem * item, allItems)
//    {
//        DragPoint* drag = dynamic_cast<DragPoint*>(item);
//        if(drag)
//        {
//            //计算可以包裹两个矩形的外切矩形
//            QRectF r = drag->mapRectToScene(drag->boundingRect());
//            rect = rect.united(r);
//        }
//    }

    //左上和右下拉动时，矩形依照此二点来构成。
    if(type == TOP_LEFT || type == BOTTOM_RIGHT)
    {
        QRectF r = leftTopPoint->mapRectToScene(leftTopPoint->boundingRect());
        rect = rect.united(r);

        rect = rect.united(rightBottomPoint->mapRectToScene(rightBottomPoint->boundingRect()));

        rightTopPoint->updatePos(type,boundRect.topRight());
        leftBottomPoint->updatePos(type,boundRect.bottomLeft());
    }
    //右上和左下拉动时，矩形按照此二点构成。
    else if(type == TOP_RIGHT || type == BOTTOM_LEFT)
    {
        QRectF r = rightTopPoint->mapRectToScene(rightTopPoint->boundingRect());
        rect = rect.united(r);

        rect = rect.united(leftBottomPoint->mapRectToScene(leftBottomPoint->boundingRect()));

        leftTopPoint->updatePos(type,boundRect.topLeft());
        rightBottomPoint->updatePos(type,boundRect.bottomRight());
//        leftBottomPoint->updatePos(type,(leftBottomPoint->boundingRect().adjusted(-2,-1,0,0)).bottomLeft());
    }
    prepareGeometryChange();

    QRectF tmpRect = mapRectFromScene(rect);

    if(tmpRect.width()<10 || tmpRect.height()<10)
    {
        return;
    }

    tmpRect.setWidth(tmpRect.height());

    boundRect = tmpRect;

    qDebug()<<boundRect.width()<<"=="<<boundRect.height();

    updateRotateLinePos();
}

void MyItem::procMouseState(MouseType type)
{
    currMouseType = type;
    if(currMouseType == MOUSE_RELEASE)
    {
        setSelected(true);
//        isNeedBorder = false;
//        update();
    }
}

void MyItem::procResizeItem()
{
    leftTopPoint->setPos(boundRect.topLeft());
    rightTopPoint->setPos(boundRect.topRight());
    leftBottomPoint->setPos(boundRect.bottomLeft());
    rightBottomPoint->setPos(boundRect.bottomRight());
}

void MyItem::procRotate(int degree)
{
    setRotation(degree);
}

MyItem::~MyItem()
{
//    foreach(MyArrow * tmp,arrows)
//    {
//        delete tmp;
//    }

    if(leftTopPoint)
    {
        delete leftTopPoint;
        leftTopPoint = NULL;
    }

    if(rightTopPoint)
    {
        delete rightTopPoint;
        rightTopPoint = NULL;
    }

    if(leftBottomPoint)
    {
        delete leftBottomPoint;
        leftBottomPoint = NULL;
    }

    if(rightBottomPoint)
    {
        delete rightBottomPoint;
        rightBottomPoint = NULL;
    }

    if(rotateLine)
    {
        delete rotateLine;
        rotateLine = NULL;
    }
}

/**
void ShapeLine::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QStyleOptionGraphicsItem op;
    op.initFrom(widget);
    op.state = QStyle::State_None;

    return QGraphicsLineItem::paint(painter, &op, widget);
}
*/
