#include "mynodeport.h"

#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QMenu>
#include <QAction>
#include <QDebug>

#include "myitem.h"
#include "myarrow.h"
#include "../actionmanager.h"

using namespace Graphics;

QDataStream & operator <<(QDataStream & dataStream,NodePortProperty & prop)
{
    dataStream<<prop.itemBrush<<prop.direct
             <<prop.scaleFactor<<prop.startItemID<<prop.parentItemID;
    return dataStream;
}

QDataStream & operator >>(QDataStream & dataStream,NodePortProperty & prop)
{
    int direct;

    dataStream>>prop.itemBrush>>direct
             >>prop.scaleFactor>>prop.startItemID>>prop.parentItemID;

    prop.direct = (DragDirect)direct;

    return dataStream;
}

QDataStream & operator <<(QDataStream & dataStream,MyNodePort * nodePort)
{
    int type = nodePort->type;
    dataStream<<type<<nodePort->nodeProperty;

    return dataStream;
}

QDataStream & operator >>(QDataStream & dataStream,MyNodePort * nodePort)
{
    int type;
    NodePortProperty property;

    dataStream>>type>>property;

    nodePort->type = (GraphicsType)type;
    nodePort->nodeProperty = property;
    return dataStream;
}

MyNodePort::MyNodePort(MyItem *parentItem):
    parentMyItem(parentItem),
    QGraphicsObject(parentItem)
{
//    setFlag(QGraphicsItem::ItemIsMovable,true);

    setFlag(QGraphicsItem::ItemIsSelectable,parentItem->isMoveable());

    radius = 13;
    type = GRA_NODE_PORT;
    nextDirect = DRAG_NONE;
    arrivalLimitRang = false;

    nodeProperty.parentItemID = parentItem->getProperty().startItemID;     //保存父节点的ID号

    boundRect = QRectF(-radius,-radius,2*radius,2*radius);

    initNodePortRightMenu();
    connect(this,SIGNAL(deletePort(MyNodePort*)),parentItem,SLOT(procDeleteNodePort(MyNodePort*)));
    connect(this,SIGNAL(editPort(MyNodePort*)),parentItem,SLOT(procEditNodePort(MyNodePort*)));

    isMoveable = true;
}

//设置端口拖入的方向
void MyNodePort::setDragDirect(DragDirect direct)
{
    nodeProperty.direct = direct;
}

//到达拐点
void MyNodePort::setNextDirect(DragDirect direct)
{
    this->nextDirect = direct;
    arrivalLimitRang = true;
}

//设置是否已经到达边界点
void MyNodePort::setArrivalLimitRang(bool flag)
{
    arrivalLimitRang = flag;
}

//设置显示的中心点
void MyNodePort::setPos(const QPointF &pos)
{
    QGraphicsObject::setPos(pos);
}

//设置拖入位置与拖入边长度的比例
void MyNodePort::setScaleFactor(qreal scaleFactor)
{
    nodeProperty.scaleFactor = scaleFactor;
}

QRectF MyNodePort::boundingRect()const
{
    return boundRect;
}

void MyNodePort::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)

    painter->save();

    painter->setBrush(nodeProperty.itemBrush);

    painter->drawRect(boundRect);

    painter->restore();
}

void MyNodePort::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    emit portPosChanged(MOUSE_PRESS,event->pos());
    QGraphicsObject::mousePressEvent(event);
}

void MyNodePort::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    emit portPosChanged(MOUSE_MOVE,event->pos());
    QGraphicsObject::mouseMoveEvent(event);
}

void MyNodePort::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    emit portPosChanged(MOUSE_RELEASE,event->pos());
    QGraphicsObject::mouseReleaseEvent(event);
}

void MyNodePort::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    emit editPort(this);
    QGraphicsObject::mouseDoubleClickEvent(event);
}

void MyNodePort::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    nodePortRightMenu->exec(event->screenPos());
}

//初始化端口右键菜单
void MyNodePort::initNodePortRightMenu()
{
    nodePortRightMenu = new QMenu;

    QAction * editAction = new QAction(QIcon(":/images/edit.png"),"编辑节点",nodePortRightMenu);
    QAction * deleteAction = new QAction(QIcon(":/images/delete.png"),"删除",nodePortRightMenu);

    nodePortRightMenu->addAction(editAction);
    nodePortRightMenu->addSeparator();
    nodePortRightMenu->addAction(deleteAction);

    connect(editAction,SIGNAL(triggered()),this,SLOT(respEditAction()));
    connect(deleteAction,SIGNAL(triggered()),this,SLOT(respDeleteAction()));
}

//响应删除
void MyNodePort::respDeleteAction()
{
    emit deletePort(this);
}

//响应编辑
void MyNodePort::respEditAction()
{
    emit editPort(this);
}

//保存添加的箭头
void MyNodePort::addArrow(MyArrow *arrow)
{
    arrows.push_back(arrow);
}

//删除此控件时，移除所有的箭头
void MyNodePort::removeArrows()
{
    foreach (MyArrow *arrow, arrows)
    {
        arrow->getStartNodePort()->removeArrow(arrow);
        arrow->getEndNodePort()->removeArrow(arrow);
//        scene()->removeItem(arrow);
        delete arrow;
    }
}

void MyNodePort::removeArrow(MyArrow *arrow)
{
    int index = arrows.indexOf(arrow);

    if (index != -1)
        arrows.removeAt(index);
}

//获取端口顶点在当前坐标系中的值(要顺时针保存)
QPolygonF MyNodePort::getScenePolygon()
{
    QPointF topLeftPoint = getParentItem()->mapToScene(boundingRect().topLeft() + pos());
    QPointF topRightPoint = getParentItem()->mapToScene(boundingRect().topRight() + pos());
    QPointF bottomRightPoint = getParentItem()->mapToScene(boundingRect().bottomRight() + pos());
    QPointF bottomLeftPoint = getParentItem()->mapToScene(boundingRect().bottomLeft() + pos());

    QPolygonF polygonF;
    polygonF<<topLeftPoint<<topRightPoint<<bottomRightPoint<<bottomLeftPoint;

    return polygonF;
}

//设置控件的样式属性
void MyNodePort::setProperty(ItemProperty property)
{
    nodeProperty.itemBrush = property.itemBrush;

    update();
}

//更新端口的ID号，主要用于本地打开时保存原始的ID号
void MyNodePort::updatePortID(QString portId)
{
    nodeProperty.startItemID = portId;
}

//更新节点属性
void MyNodePort::setNodeProperty(NodePortProperty prop)
{
    this->nodeProperty = prop;
}

//设置是否可以移动
void MyNodePort::setMoveable(bool isMoveable)
{
    isMoveable = isMoveable;
    setFlag(QGraphicsItem::ItemIsSelectable,isMoveable);
}

MyNodePort::~MyNodePort()
{
}
