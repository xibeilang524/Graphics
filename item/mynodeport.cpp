#include "mynodeport.h"

#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QMenu>
#include <QAction>
#include <QDebug>

#include "myitem.h"
#include "myarrow.h"
#include "mytextitem.h"
#include "mypathitem.h"
#include "../manager/actionmanager.h"

using namespace Graphics;

QDataStream & operator <<(QDataStream & dataStream,NodePortProperty & prop)
{
    dataStream<<prop.portType<<prop.content<<prop.itemBrush<<prop.direct
             <<prop.scaleFactor<<prop.startItemID<<prop.parentItemID;

    return dataStream;
}

QDataStream & operator >>(QDataStream & dataStream,NodePortProperty & prop)
{
    int direct;
    int type;

    dataStream>>type>>prop.content>>prop.itemBrush>>direct
             >>prop.scaleFactor>>prop.startItemID>>prop.parentItemID;

    prop.portType = (GraphicsType)type;
    prop.direct = (DragDirect)direct;

    return dataStream;
}

QDataStream & operator <<(QDataStream & dataStream,MyNodePort * nodePort)
{
    int type = nodePort->currItemType;
    dataStream<<type<<nodePort->nodeProperty;

#ifdef ADD_STATE_MODEL
    switch(nodePort->nodeProperty.portType)
    {
        case GRA_NODE_CIRCLE:
                            dataStream<<nodePort->statePortProp;
                            break;

        case GRA_NODE_TRIANGLE_OUT:
        case GRA_NODE_HALF_CIRCLE_OUT:
                            dataStream<<nodePort->stateInOutProp;
                            break;
    }
#endif

    return dataStream;
}

QDataStream & operator >>(QDataStream & dataStream,MyNodePort * nodePort)
{
    int type;
    NodePortProperty property;
    dataStream>>type>>property;

#ifdef ADD_STATE_MODEL
    switch(property.portType)
    {
        case GRA_NODE_CIRCLE:
                            dataStream>>nodePort->statePortProp;
                            break;

        case GRA_NODE_TRIANGLE_OUT:
        case GRA_NODE_HALF_CIRCLE_OUT:
                            dataStream>>nodePort->stateInOutProp;
                            break;
    }
#endif

    nodePort->currItemType = (GraphicsType)type;
    nodePort->nodeProperty = property;
    return dataStream;
}

#ifdef ADD_STATE_MODEL
//对初始化端口的保存
QDataStream & operator<<(QDataStream & stream,StatePortProperty &prop)
{
    stream<<prop.portName<<prop.portType;
    return stream;
}

QDataStream & operator>> (QDataStream & stream,StatePortProperty &prop)
{
    stream>>prop.portName>>prop.portType;
    return stream;
}

//输入/出端口设置
QDataStream & operator<< (QDataStream & stream,StateInOutProperty & prop)
{
    stream<<prop.portName<<prop.portType;
    stream<<prop.props.size();

    for(int i=0;i<prop.props.size();i++)
    {
        StatePortProperty sprop = prop.props.at(i);
        stream<<sprop;
    }
    return stream;
}

QDataStream & operator>> (QDataStream & stream,StateInOutProperty & prop)
{
    stream>>prop.portName>>prop.portType;

    int size = 0;
    stream>>size;

    for(int i=0;i<size;i++)
    {
        StatePortProperty sprop;
        stream>>sprop;
        prop.props.append(sprop);
    }

    return stream;
}
#endif

MyNodePort::MyNodePort(GraphicsType type, MyItem *parentItem, QObject *parent1):
    MyNodeLine(type,parentItem,parent1)
{
    setFlag(QGraphicsItem::ItemIsSelectable,parentItem->isMoveable());

    radius = 8;
    currItemType = type;
    nodeProperty.portType = type;
    nextDirect = DRAG_NONE;
    arrivalLimitRang = false;
    isMoveable = true;

    nodeProperty.parentItemID = parentItem->getProperty().startItemID;     //保存父节点的ID号

    prepareGeometryChange();
    boundRect = QRectF(-radius,-radius,2*radius,2*radius);

    initWidget(parentItem);
}

//初始化控件
void MyNodePort::initWidget(MyItem *parentItem)
{
    //文字信息
    myTextItem = new MyTextItem(GRA_TEXT,this);
    myTextItem->setTextExistType(TEXT_CHILD);
    connect(myTextItem,SIGNAL(updateTextGeometry()),this,SLOT(updateTextPosByDirect()));

    myTextItem->setTextInteractionFlags(Qt::NoTextInteraction);
    myTextItem->setFlag(QGraphicsItem::ItemIsMovable,false);
    myTextItem->setFlag(QGraphicsItem::ItemIsSelectable,false);
    myTextItem->setFlag(ItemIsFocusable,false);
    myTextItem->cleartText();

    initNodePortRightMenu();
    connect(this,SIGNAL(deletePort(MyNodePort*)),parentItem,SLOT(procDeleteNodePort(MyNodePort*)));
    connect(this,SIGNAL(editPort(MyNodePort*)),parentItem,SLOT(procEditNodePort(MyNodePort*)));
    connect(this,SIGNAL(dClickEditPort(MyNodePort*)),parentItem,SLOT(procDoubleClickEditNodePort(MyNodePort*)));
    connect(this,SIGNAL(portPosChanged(MouseType,QPointF)),parentItem,SLOT(procPortChanged(MouseType,QPointF)));
}

//设置端口拖入的方向
void MyNodePort::setDragDirect(DragDirect direct)
{
    nodeProperty.direct = direct;
    prepareGeometryChange();

    itemPolygon.clear();
    if(currItemType == GRA_NODE_PORT)
    {
        itemPolygon<<QPointF(-radius,-radius)<<QPointF(radius,-radius)
                 <<QPointF(radius,radius)<<QPointF(-radius,radius);
    }
    else if(currItemType == GRA_NODE_CIRCLE)
    {
        QPainterPath path;
        path.addEllipse(QRectF(-radius,-radius,2*radius,2*radius));
        itemPolygon = path.toFillPolygon();
    }
    else
    {
        switch(direct)
        {
            case DRAG_LEFT:
                            {
                                if(currItemType == GRA_NODE_HALF_CIRCLE_IN)
                                {
                                    QPainterPath path;
                                    path.moveTo(-radius,-radius);
                                    path.lineTo(-radius,radius);
                                    path.arcTo(QRectF(-radius,-radius,2*radius,2*radius),-90,180);
                                    itemPolygon = path.toFillPolygon();
                                }
                                else if(currItemType == GRA_NODE_HALF_CIRCLE_OUT)
                                {
                                    QPainterPath path;
                                    path.moveTo(radius,-radius);
                                    path.lineTo(radius,radius);
                                    path.arcTo(QRectF(-radius,-radius,2*radius,2*radius),-90,-180);
                                    itemPolygon = path.toFillPolygon();
                                }
                                else if(currItemType == GRA_NODE_TRIANGLE_OUT)
                                {
                                    itemPolygon<<QPointF(radius,-radius)<<QPointF(-radius,0)
                                                <<QPointF(radius,radius);
                                }
                                else if(currItemType == GRA_NODE_TRIANGLE_IN)
                                {
                                    itemPolygon<<QPointF(-radius,-radius)<<QPointF(radius,0)
                                                <<QPointF(-radius,radius);
                                }
                                break;
                            }
            case DRAG_TOP:
                            {
                                if(currItemType == GRA_NODE_HALF_CIRCLE_IN)
                                {
                                    QPainterPath path;
                                    path.moveTo(radius,-radius);
                                    path.lineTo(-radius,-radius);
                                    path.arcTo(QRectF(-radius,-radius,2*radius,2*radius),180,180);
                                    itemPolygon = path.toFillPolygon();
                                }
                                else if(currItemType == GRA_NODE_HALF_CIRCLE_OUT)
                                {
                                    QPainterPath path;
                                    path.moveTo(radius,radius);
                                    path.lineTo(-radius,radius);
                                    path.arcTo(QRectF(-radius,-radius,2*radius,2*radius),180,-180);
                                    itemPolygon = path.toFillPolygon();
                                }
                                else if(currItemType == GRA_NODE_TRIANGLE_OUT)
                                {
                                    itemPolygon<<QPointF(radius,radius)<<QPointF(0,-radius)
                                                <<QPointF(-radius,radius);
                                }
                                else if(currItemType == GRA_NODE_TRIANGLE_IN)
                                {
                                    itemPolygon<<QPointF(-radius,-radius)<<QPointF(0,radius)
                                                <<QPointF(radius,-radius);
                                }
                                break;
                            }
            case DRAG_RIGHT:
                            {
                                if(currItemType == GRA_NODE_HALF_CIRCLE_IN)
                                {
                                    QPainterPath path;
                                    path.moveTo(radius,radius);
                                    path.lineTo(radius,-radius);
                                    path.arcTo(QRectF(-radius,-radius,2*radius,2*radius),90,180);
                                    itemPolygon = path.toFillPolygon();
                                }
                                else if(currItemType == GRA_NODE_HALF_CIRCLE_OUT)
                                {
                                    QPainterPath path;
                                    path.moveTo(-radius,-radius);
                                    path.lineTo(-radius,radius);
                                    path.arcTo(QRectF(-radius,-radius,2*radius,2*radius),-90,180);
                                    itemPolygon = path.toFillPolygon();
                                }
                                else if(currItemType == GRA_NODE_TRIANGLE_OUT)
                                {
                                    itemPolygon<<QPointF(-radius,-radius)<<QPointF(radius,0)
                                                <<QPointF(-radius,radius);
                                }
                                else if(currItemType == GRA_NODE_TRIANGLE_IN)
                                {
                                    itemPolygon<<QPointF(radius,-radius)<<QPointF(-radius,0)
                                                <<QPointF(radius,radius);
                                }
                                break;
                            }
            case DRAG_BOTTOM:
                            {
                                if(currItemType == GRA_NODE_HALF_CIRCLE_IN)
                                {
                                    QPainterPath path;
                                    path.moveTo(radius,radius);
                                    path.lineTo(-radius,radius);
                                    path.arcTo(QRectF(-radius,-radius,2*radius,2*radius),180,-180);
                                    itemPolygon = path.toFillPolygon();
                                }
                                else if(currItemType == GRA_NODE_HALF_CIRCLE_OUT)
                                {
                                    QPainterPath path;
                                    path.moveTo(-radius,-radius);
                                    path.lineTo(radius,-radius);
                                    path.arcTo(QRectF(-radius,-radius,2*radius,2*radius),0,-180);
                                    itemPolygon = path.toFillPolygon();
                                }
                                else if(currItemType == GRA_NODE_TRIANGLE_OUT)
                                {
                                    itemPolygon<<QPointF(-radius,-radius)<<QPointF(0,radius)
                                                <<QPointF(radius,-radius);
                                }
                                else if(currItemType == GRA_NODE_TRIANGLE_IN)
                                {
                                    itemPolygon<<QPointF(-radius,radius)<<QPointF(0,-radius)
                                                <<QPointF(radius,radius);
                                }
                                break;
                            }
        }
    }
    setPolygon(itemPolygon);

    updateTextPosByDirect();
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
    QGraphicsPolygonItem::setPos(pos);
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

    painter->drawPolygon(itemPolygon);

    painter->restore();
}

void MyNodePort::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    emit portPosChanged(MOUSE_PRESS,event->pos());
    QGraphicsPolygonItem::mousePressEvent(event);
}

void MyNodePort::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    emit portPosChanged(MOUSE_MOVE,event->pos());
    QGraphicsPolygonItem::mouseMoveEvent(event);
}

void MyNodePort::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    emit portPosChanged(MOUSE_RELEASE,event->pos());
    QGraphicsPolygonItem::mouseReleaseEvent(event);
}

void MyNodePort::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    emit dClickEditPort(this);
    QGraphicsPolygonItem::mouseDoubleClickEvent(event);
}

void MyNodePort::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    setSelected(true);
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

QString MyNodePort::getText()
{
    return myTextItem->toPlainText();
}

//更新文字信息，同时更新textItem在Nodeport中的位置
void MyNodePort::setText(QString text)
{
    nodeProperty.content = text;
    myTextItem->setPlainText(text);
    updateTextPosByDirect();

#ifdef ADD_STATE_MODEL
    statePortProp.portName = text;
    stateInOutProp.portName = text;

#endif
}

//根据当前控件所在的Direct来动态的改变文字的位置
void MyNodePort::updateTextPosByDirect()
{
    QRectF rectF = myTextItem->getBoundRect();

    switch(nodeProperty.direct)
    {
          case DRAG_LEFT:
                            myTextItem->setPos(-(radius*2 + rectF.width()),-rectF.height());
                          break;
          case DRAG_TOP:
                            myTextItem->setPos(-rectF.width()/2,-(radius*2+rectF.height()));
                          break;
          case DRAG_RIGHT:
                            myTextItem->setPos(radius,-rectF.height());
                          break;
          case DRAG_BOTTOM:
                            myTextItem->setPos(-rectF.width()/2,radius);
                          break;
          default:
                    break;
    }
}

#ifdef ADD_STATE_MODEL
void MyNodePort::setPortProp(StatePortProperty &prop)
{
    statePortProp.portName = prop.portName;
    statePortProp.portType = prop.portType;
    setText(statePortProp.portName);
}

void MyNodePort::setPortInoutProp(StateInOutProperty &prop)
{
    stateInOutProp.portName = prop.portName;
    stateInOutProp.portType = prop.portType;
    stateInOutProp.props = prop.props;
    setText(stateInOutProp.portName);
}
#endif

//更新折线线条的位置
void MyNodePort::updateLinePos()
{
    foreach(MyPathItem * pathItem,pathLines)
    {
        pathItem->updateCurrItemPos();
    }
}

//当端口的方向改变后，需要更改折线的起点/终点的ItemType
void MyNodePort::updatePathQuoteType()
{
    foreach(MyPathItem * pathItem,pathLines)
    {
        if(pathItem->getStartItem()->getProperty().startItemID == property.startItemID)
        {
            pathItem->setStartPointType(nodeProperty.direct);
        }
        else if(pathItem->getEndItem()->getProperty().startItemID == property.startItemID)
        {
            pathItem->setEndPointType(nodeProperty.direct);
        }
        pathItem->updateCurrItemPos();
    }
}

MyNodePort::~MyNodePort()
{
}
