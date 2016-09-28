#include "myitem.h"

#include <QGraphicsSceneMouseEvent>
#include <QGraphicsSceneHoverEvent>
#include <QGraphicsSceneContextMenuEvent>
#include <QPainter>
#include <QMenu>
#include <QDebug>
#include <QGraphicsScene>
#include <QPointF>
#include <QGraphicsSceneDragDropEvent>
#include <QMimeData>
#include <QUrl>
#include <QKeyEvent>

#include "myarrow.h"
#include "mytextitem.h"
#include "mynodeport.h"
#include "mygraphicsview.h"

#include "qmath.h"

#define QCOS(a) qCos(a*PI/180)
#define QSIN(a) qSin(a*PI/180)

#define CROSS_RADIUS 8        //绘制拖入十字星半径
#define VER_HOR_SCALE 0.5     //控件竖直和水平的长度比例，用于绘制矩形等非等边图形

//对MyRect的输出进行重载
QDataStream & operator <<(QDataStream &stream,MyRect & rect)
{
    stream<<rect.x<<rect.y<<rect.width<<rect.height;
    return stream;
}

QDataStream & operator >>(QDataStream &stream,MyRect & rect)
{
    stream>>rect.x>>rect.y>>rect.width>>rect.height;
    return stream;
}

QDataStream & operator <<(QDataStream &stream,LineType & type)
{
    int tmp = (int)type;
    stream<<tmp;
    return stream;
}

QDataStream & operator >>(QDataStream &stream,LineType & type)
{
    int tmp;
    stream>>tmp;
    type = (LineType)tmp;
    return stream;
}

QDataStream & operator <<(QDataStream &stream,AddLineType & type)
{
    int tmp = (int)type;
    stream<<tmp;
    return stream;
}

QDataStream & operator >>(QDataStream &stream,AddLineType & type)
{
    int tmp;
    stream>>tmp;
    type = (AddLineType)tmp;
    return stream;
}

////写入属性
QDataStream & operator <<(QDataStream & stream,ItemProperty & prop)
{
    stream<<prop.startItemID<<prop.endItemID
           <<prop.isNeedBrush<<prop.itemBrush
            <<prop.isNeedBorder<<prop.itemPen<<prop.itemRect
             <<prop.alphaValue<<prop.rotateDegree
              <<prop.isFont<<prop.content<<prop.itemFont<<prop.fontColor
               <<prop.zValue<<prop.lineType<<prop.startLineType<<prop.endLineType
                <<prop.isMoveable;

    return stream;
}

////读属性
QDataStream & operator >>(QDataStream & stream,ItemProperty & prop)
{
    stream>>prop.startItemID>>prop.endItemID
           >>prop.isNeedBrush>>prop.itemBrush
            >>prop.isNeedBorder>>prop.itemPen>>prop.itemRect
             >>prop.alphaValue>>prop.rotateDegree
              >>prop.isFont>>prop.content>>prop.itemFont>>prop.fontColor
               >>prop.zValue>>prop.lineType>>prop.startLineType>>prop.endLineType
                >>prop.isMoveable;

    return stream;
}

QDataStream & operator <<(QDataStream &stream,MyItem * item)
{
    int type = item->currItemType;

    stream<<type<<item->property;

    return stream;
}

QDataStream & operator >>(QDataStream &stream,MyItem * item)
{
    int type;
    ItemProperty prop;

    stream>>type>>prop;
    item->currItemType = (GraphicsType)type;

    item->setProperty(prop);

    return stream;
}

MyItem::MyItem(GraphicsType itemType, QMenu *menu, QGraphicsScene *parentScene, QObject *parent1, QGraphicsItem *parent2):
    currItemType(itemType),
    rightMenu(menu),
    parentScene(parentScene),
    QObject(parent1),
    QGraphicsPolygonItem(parent2)
{
    radius = 60;
    isDragging = false;            //默认无拖入

    setFlag(QGraphicsItem::ItemIsMovable,true);
    setFlag(QGraphicsItem::ItemIsSelectable,true);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges,true);
    setAcceptDrops(true);

    prepareGeometryChange();

    switch(currItemType)
    {
            //正方形
            case GRA_SQUARE:
                               boundRect = QRectF(-radius,-radius,2*radius,2*radius);   //设置范围时同时也默认指定了其中心点坐标(0,0)
                               itemPolygon<<QPointF(-radius,-radius)<<QPointF(radius,-radius)<<
                                      QPointF(radius,radius)<<QPointF(-radius,radius);
                               break;
            //矩形
            case GRA_RECT:
                               boundRect = QRectF(-radius,-VER_HOR_SCALE*radius,2*radius,radius);
                               itemPolygon<<QPointF(-radius,-VER_HOR_SCALE*radius)<<QPointF(radius,-VER_HOR_SCALE*radius)<<
                                       QPointF(radius,VER_HOR_SCALE*radius)<<QPointF(-radius,VER_HOR_SCALE*radius);
                               break;
           //圆角矩形
           case GRA_ROUND_RECT:
                              {
                                  boundRect = QRectF(-radius,-VER_HOR_SCALE*radius,2*radius,radius);
                                  QPainterPath path;
                                  path.addRoundedRect(boundRect,10,10);
                                  itemPolygon = path.toFillPolygon();
                              }
                              break;
            //圆形
            case GRA_CIRCLE:
                               {
                                   boundRect = QRectF(-VER_HOR_SCALE*radius,-VER_HOR_SCALE*radius,radius,radius);
                                   QPainterPath path;
                                   path.addEllipse(boundRect);
                                   itemPolygon = path.toFillPolygon();
                               }
                               break;
            //椭圆
            case GRA_ELLIPSE:
                               {
                                   boundRect = QRectF(-radius,-VER_HOR_SCALE*radius,2*radius,radius);
                                   QPainterPath path;
                                   path.addEllipse(boundRect);
                                   itemPolygon = path.toFillPolygon();
                               }
                               break;
            //菱形
            case GRA_POLYGON:
                               boundRect = QRectF(-radius,-VER_HOR_SCALE*radius,2*radius,radius);

                               itemPolygon<<QPointF(-radius,0)<<QPointF(0,-VER_HOR_SCALE*radius)<<
                                         QPointF(radius,0)<<QPointF(0,VER_HOR_SCALE*radius);
                               break;
            //平行四边形
            case GRA_PARALLELOGRAM:
                              boundRect = QRectF(-radius,-VER_HOR_SCALE*radius,2*radius,radius);

                              itemPolygon<<QPointF(-VER_HOR_SCALE*radius,-VER_HOR_SCALE*radius)<<QPointF(radius,-VER_HOR_SCALE*radius)<<
                                        QPointF(VER_HOR_SCALE*radius,VER_HOR_SCALE*radius)<<QPointF(-radius,VER_HOR_SCALE*radius);
                              break;
    }

    setPolygon(itemPolygon);
    setBrush(property.itemBrush);

    property.isMoveable = true;         //默认可以移动
    property.itemBrush = QBrush(Qt::white);
    property.itemPen = QPen(Qt::black,1,Qt::SolidLine);

    property.itemRect.width = boundRect.width();
    property.itemRect.height = boundRect.height();

    property.itemFont = QFont("黑体",FONT_DEAFULT_PIX);

    currMouseType = MOUSE_NONE;
    isNeedBorder = false;

    selectedPenWidth = 1;
    selectedPen.setColor(Qt::black);
    selectedPen.setStyle(Qt::DashLine);
    selectedPen.setWidth(selectedPenWidth);

    //旋转线条
    rotateLine = new RotateLine(this);
    connect(rotateLine,SIGNAL(rotateItem(MouseType,qreal)),this,SLOT(procRotate(MouseType,qreal)));
    updateRotateLinePos();

    //文字信息
    myTextItem = new MyTextItem(GRA_TEXT,menu,this);
    connect(myTextItem,SIGNAL(updateTextGeometry()),this,SLOT(procUpdateTextGeometry()));

    myTextItem->setTextInteractionFlags(Qt::NoTextInteraction);
    myTextItem->setFlag(QGraphicsItem::ItemIsMovable,false);
    myTextItem->setFlag(QGraphicsItem::ItemIsSelectable,false);
    myTextItem->cleartText();

    //八个角度的缩放点
    leftTopPoint = new DragPoint(TOP_LEFT,this);
    rightTopPoint = new DragPoint(TOP_RIGHT,this);
    leftBottomPoint = new DragPoint(BOTTOM_LEFT,this);
    rightBottomPoint = new DragPoint(BOTTOM_RIGHT,this);

    topPoint = new DragPoint(TOP_MIDDLE,this);
    leftPoint = new DragPoint(MIDDLE_LEFT,this);
    rightPoint = new DragPoint(MIDDLE_RIGHT,this);
    bottomPoint = new DragPoint(BOTTOM_MIDDLE,this);

    setAcceptHoverEvents(true);
    setDragPointVisible(false);

    procResizeItem();
}

void MyItem::setPos(const QPointF &pos)
{
    property.itemRect.x = pos.x();
    property.itemRect.y = pos.y();

    QGraphicsPolygonItem::setPos(pos);
}

void MyItem::setZValue(qreal z)
{
    property.zValue = z;
    QGraphicsPolygonItem::setZValue(z);
}

QString MyItem::getText()
{
    return myTextItem->toPlainText();
}

//更新文字信息，同时更新textItem在item中的位置
void MyItem::setText(QString text)
{
    property.content = text;
    myTextItem->setPlainText(text);

    QRectF rectF = myTextItem->getBoundRect();

    myTextItem->setPos(-rectF.width()/2,-rectF.height()/2);
}

//动态调整输入字符框的位置尺寸
void MyItem::procUpdateTextGeometry()
{
    QRectF rectF = myTextItem->getBoundRect();
    myTextItem->setPos(-rectF.width()/2,-rectF.height()/2);
}

void MyItem::updateRotateLinePos()
{
    int posX = boundingRect().center().x();
    int posY = boundingRect().topLeft().y() - 5*rotateLine->boundingRect().height();

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

void MyItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)

    painter->save();

    if(property.isNeedBorder)
    {
        painter->setPen(property.itemPen);
    }
    else
    {
        painter->setPen(Qt::NoPen);
    }

    if(property.isNeedBrush)
    {
        painter->setBrush(property.itemBrush);
    }

    painter->drawPolygon(itemPolygon);

    //绘制外部接口拖入至本控件时的位置状态
    if(isDragging)
    {
        painter->save();
        painter->setPen(QPen(Qt::red,2,Qt::SolidLine));

        switch(dragMoveDirect)
        {
            case DRAG_LEFT:
                             painter->drawLine(dragMovePoint.x(),dragMovePoint.y(),dragMovePoint.x()+CROSS_RADIUS,dragMovePoint.y());
                             painter->drawLine(dragMovePoint.x(),dragMovePoint.y()-CROSS_RADIUS,dragMovePoint.x(),dragMovePoint.y()+CROSS_RADIUS);
                             break;
            case DRAG_TOP:
                             painter->drawLine(dragMovePoint.x()-CROSS_RADIUS,dragMovePoint.y(),dragMovePoint.x()+CROSS_RADIUS,dragMovePoint.y());
                             painter->drawLine(dragMovePoint.x(),dragMovePoint.y(),dragMovePoint.x(),dragMovePoint.y()+CROSS_RADIUS);
                             break;
            case DRAG_RIGHT:
                             painter->drawLine(dragMovePoint.x()-CROSS_RADIUS,dragMovePoint.y(),dragMovePoint.x(),dragMovePoint.y());
                             painter->drawLine(dragMovePoint.x(),dragMovePoint.y()-CROSS_RADIUS,dragMovePoint.x(),dragMovePoint.y()+CROSS_RADIUS);
                             break;
            case DRAG_BOTTOM:
                             painter->drawLine(dragMovePoint.x()-CROSS_RADIUS,dragMovePoint.y(),dragMovePoint.x()+CROSS_RADIUS,dragMovePoint.y());
                             painter->drawLine(dragMovePoint.x(),dragMovePoint.y()-CROSS_RADIUS,dragMovePoint.x(),dragMovePoint.y());
                             break;
        }
        painter->restore();
    }

    painter->restore();
}

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
    //【修复移动控件后，修改样式位置发生微小偏移】
    getRealTimePos();
    emit updateSceneDraw();

    QGraphicsPolygonItem::mouseMoveEvent(event);
}

void MyItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    getRealTimePos();
    emit updateSceneDraw();

    QGraphicsPolygonItem::mouseReleaseEvent(event);
}

//双击编辑文字信息
void MyItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    emit editMe();
    QGraphicsPolygonItem::mouseDoubleClickEvent(event);

//    myTextItem->setTextInteractionFlags(Qt::TextEditorInteraction);
}

//在鼠标事件中获取实时的位置并更新至右侧面板
void MyItem::getRealTimePos()
{
    property.itemRect.x = this->x();
    property.itemRect.y = this->y();
    property.itemRect.width = this->boundingRect().width();
    property.itemRect.height = this->boundingRect().height();

    emit propHasChanged(property);
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

//拖入事件
void MyItem::dragEnterEvent(QGraphicsSceneDragDropEvent *event)
{
    if(event->mimeData()->hasFormat("MyItem"))
    {
        QByteArray array = event->mimeData()->data("MyItem");
        QDataStream stream(&array, QIODevice::ReadOnly);//流，读数据

        int graphicsType;
        stream>>graphicsType;
        if(graphicsType == GRA_NODE_PORT)
        {
            setSelected(true);
            event->acceptProposedAction();
            isDragging = false;
        }
        else
        {
            isDragging = false;
            event->ignore();
        }
    }
    else
    {
        setSelected(false);
        isDragging = false;
        event->ignore();
    }
    update();
}

//拖入移动
void MyItem::dragMoveEvent(QGraphicsSceneDragDropEvent *event)
{
    if(event->mimeData()->hasFormat("MyItem"))
    {
        if(getPointToRectMinDistance(boundRect,event->pos()) <= ALLOW_DROP_RANGE)
        {
            isDragging = true;

            dragMoveDirect = getDropDirect(event->pos());

            switch (dragMoveDirect)
            {
                case DRAG_NONE:
                                  return;
                              break;
                case DRAG_LEFT:
                                  dragMovePoint.setX(boundRect.topLeft().x());
                                  dragMovePoint.setY(event->pos().y());
                              break;
                case DRAG_TOP:
                                  dragMovePoint.setX(event->pos().x());
                                  dragMovePoint.setY(boundRect.topLeft().y());
                              break;
                case DRAG_RIGHT:
                                  dragMovePoint.setX(boundRect.topRight().x());
                                  dragMovePoint.setY(event->pos().y());
                              break;
                case DRAG_BOTTOM:
                                  dragMovePoint.setX(event->pos().x());
                                  dragMovePoint.setY(boundRect.bottomRight().y());
                              break;
            }
            setSelected(true);
            event->acceptProposedAction();
        }
        else
        {
            isDragging = false;
            setSelected(false);
            event->ignore();
        }
    }
    else
    {
        isDragging = false;
        setSelected(false);
        event->ignore();
    }
    update();
}

//计算点到矩形四边最小距离
qreal MyItem::getPointToRectMinDistance(QRectF rect,QPointF point)
{
    //左侧
    qreal minDis = 100000;

    qreal leftDis = qAbs(rect.topLeft().x() - point.x());
    minDis = minDis <= leftDis ? minDis:leftDis;

    qreal topDis = qAbs(rect.topLeft().y() - point.y());
    minDis = minDis <= topDis ? minDis:topDis;

    qreal rightDis = qAbs(rect.bottomRight().x() - point.x());
    minDis = minDis <= rightDis ? minDis:rightDis;

    qreal bottomDis = qAbs(rect.bottomRight().y() - point.y());
    minDis = minDis <= bottomDis ? minDis:bottomDis;

    return minDis;
}

//拖入离开
void MyItem::dragLeaveEvent(QGraphicsSceneDragDropEvent *event)
{
    isDragging = false;
    setSelected(false);
    event->ignore();
    update();
}

/*!
*拖拽放下
*在放下时候需要考虑放在哪一侧,
***/
void MyItem::dropEvent(QGraphicsSceneDragDropEvent *event)
{
    if(event->mimeData()->hasFormat("MyItem"))
    {
        QPointF dropPoint = event->pos();
        QPointF itemPos;
        qreal scaleFactor;

        DragDirect direct = getDropDirect(dropPoint);

        switch (direct)
        {
            case DRAG_NONE:
                              return;
                          break;
            case DRAG_LEFT:
                              itemPos.setX(boundRect.topLeft().x());
                              itemPos.setY(dropPoint.y());
                              scaleFactor = (dropPoint.y() - boundRect.topLeft().y()) / boundRect.height();
                          break;
            case DRAG_TOP:
                              itemPos.setX(dropPoint.x());
                              itemPos.setY(boundRect.topLeft().y());
                              scaleFactor = (dropPoint.x() - boundRect.topLeft().x()) / boundRect.width();
                          break;
            case DRAG_RIGHT:
                              itemPos.setX(boundRect.bottomRight().x());
                              itemPos.setY(dropPoint.y());
                              scaleFactor = (dropPoint.y() - boundRect.topRight().y()) / boundRect.height();
                          break;
            case DRAG_BOTTOM:
                              itemPos.setX(dropPoint.x());
                              itemPos.setY(boundRect.bottomRight().y());
                              scaleFactor = (dropPoint.x() - boundRect.bottomLeft().x()) / boundRect.width();
                          break;
        }

        createProp(itemPos,direct,scaleFactor);

        setSelected(false);

        isDragging = false;
    }
}

//获取拖放在四边的位置
DragDirect MyItem::getDropDirect(const QPointF &currPoint)
{
    DragDirect direct;
    //放在左侧
    if(qAbs(currPoint.x() - boundRect.topLeft().x()) <=  ALLOW_DROP_RANGE)
    {
        direct = DRAG_LEFT;
    }
    //放在上侧
    else if(qAbs(currPoint.y() - boundRect.topLeft().y()) <=  ALLOW_DROP_RANGE)
    {
        direct = DRAG_TOP;
    }
    //放在右侧
    else if(qAbs(currPoint.x() - boundRect.bottomRight().x()) <=  ALLOW_DROP_RANGE)
    {
        direct = DRAG_RIGHT;
    }
    //放在下侧
    else if(qAbs(currPoint.y() - boundRect.bottomRight().y()) <=  ALLOW_DROP_RANGE)
    {
        direct = DRAG_BOTTOM;
    }
    else
    {
        direct = DRAG_NONE;
    }
    return direct;
}

//拷贝或者拖入一个端口
MyNodePort * MyItem::addNodePort(const NodePortProperty &prop)
{
    QPointF itemPos;
    switch(prop.direct)
    {
        case DRAG_LEFT:
                    {
                        itemPos.setX(boundRect.topLeft().x());
                        itemPos.setY(prop.scaleFactor*boundRect.height()+boundRect.topLeft().y());
                    }
                    break;
        case DRAG_TOP:
                    {
                        itemPos.setX(prop.scaleFactor*boundRect.width()+boundRect.topLeft().x());
                        itemPos.setY(boundRect.topLeft().y());
                    }
                    break;
        case DRAG_RIGHT:
                    {
                        itemPos.setX(boundRect.topRight().x());
                        itemPos.setY(prop.scaleFactor*boundRect.height()+boundRect.topRight().y());
                    }
                    break;
        case DRAG_BOTTOM:
                    {
                        itemPos.setX(prop.scaleFactor*boundRect.width()+boundRect.bottomLeft().x());
                        itemPos.setY(boundRect.bottomLeft().y());
                    }
                    break;
    }
    return createProp(itemPos,prop.direct,prop.scaleFactor);
}

MyNodePort * MyItem::createProp(const QPointF pos,const DragDirect direct,const qreal scaleFactor)
{
    MyNodePort * port = new MyNodePort(this);
    port->setPos(pos);
    port->setDragDirect(direct);
    port->setScaleFactor(scaleFactor);
    port->setProperty(property);

    connect(port,SIGNAL(portPosChanged(MouseType,QPointF)),this,SLOT(procPortChanged(MouseType,QPointF)));

    ports.push_back(port);

    return port;
}

//移除端口
void MyItem::removeNodePort(MyNodePort *port)
{
    if(ports.size() > 0)
    {
        ports.removeOne(port);
    }
}

//处理端口位置改变,确保端口只可以在边上移动
void MyItem::procPortChanged(MouseType type, QPointF currPoint)
{
    MyNodePort * tmpPort = qobject_cast<MyNodePort *>(QObject::sender());

    if(tmpPort)
    {
        if(type == MOUSE_MOVE)
        {
            QPointF newPoint;
            qreal tmpX,tmpY;
            DragRange  range = DRAG_RANG_NONE;

            switch(tmpPort->getDragDirect())
            {
                case DRAG_LEFT:
                              {
                                   tmpX = boundRect.topLeft().x();
                                   tmpY = tmpPort->pos().y() + currPoint.y();
                                   //到达线段的边界
                                   bool flag = tmpPort->isArrivalLimitRang();
                                   //到达边界后，
                                   if(flag)
                                   {
                                       if(currPoint.x()>=MOVE_LIMIT_RANG)
                                       {
                                           if(tmpPort->getNextDirect() == DRAG_TOP)
                                           {
                                               tmpPort->setDragDirect(DRAG_TOP);
                                               tmpPort->setPos(boundRect.topLeft());
                                           }
                                           else if(tmpPort->getNextDirect() == DRAG_BOTTOM)
                                           {
                                               tmpPort->setDragDirect(DRAG_BOTTOM);
                                               tmpPort->setPos(boundRect.bottomLeft());
                                           }
                                       }
                                       else
                                       {
                                           tmpPort->setArrivalLimitRang(false);
                                       }
                                       return;
                                   }
                                   else
                                   {
                                       if(getRangeValue(boundRect.bottomLeft().y(),boundRect.topLeft().y(),tmpY,range))
                                       {
                                           if(range == DRAG_RANG_MAX)
                                           {
                                               tmpPort->setNextDirect(DRAG_BOTTOM);
                                           }
                                           else if(range == DRAG_RANG_MIN)
                                           {
                                               tmpPort->setNextDirect(DRAG_TOP);
                                           }
                                       }
                                   }
                              }
                               break;
                case DRAG_TOP:
                              {
                                   tmpX = tmpPort->pos().x() + currPoint.x();
                                   tmpY = boundRect.topLeft().y();

                                   //到达线段的边界
                                   bool flag = tmpPort->isArrivalLimitRang();
                                   //到达边界后，
                                   if(flag)
                                   {
                                       if(currPoint.y()>=MOVE_LIMIT_RANG)
                                       {
                                           if(tmpPort->getNextDirect() == DRAG_RIGHT)
                                           {
                                               tmpPort->setDragDirect(DRAG_RIGHT);
                                               tmpPort->setPos(boundRect.topRight());
                                           }
                                           else if(tmpPort->getNextDirect() == DRAG_LEFT)
                                           {
                                               tmpPort->setDragDirect(DRAG_LEFT);
                                               tmpPort->setPos(boundRect.topLeft());
                                           }
                                       }
                                       else
                                       {
                                           tmpPort->setArrivalLimitRang(false);
                                       }
                                       return;
                                   }
                                   else
                                   {
                                       if(getRangeValue(boundRect.topRight().x(),boundRect.topLeft().x(),tmpX,range))
                                       {
                                           if(range == DRAG_RANG_MAX)
                                           {
                                               tmpPort->setNextDirect(DRAG_RIGHT);
                                           }
                                           else if(range == DRAG_RANG_MIN)
                                           {
                                               tmpPort->setNextDirect(DRAG_LEFT);
                                           }
                                       }
                                   }
                              }
                               break;
                case DRAG_RIGHT:
                              {
                                   tmpX = boundRect.topRight().x();
                                   tmpY = tmpPort->pos().y() + currPoint.y();
                                   //到达线段的边界
                                   bool flag = tmpPort->isArrivalLimitRang();
                                   //到达边界后，
                                   if(flag)
                                   {
                                       if(-currPoint.x()>=MOVE_LIMIT_RANG)
                                       {
                                           if(tmpPort->getNextDirect() == DRAG_BOTTOM)
                                           {
                                               tmpPort->setDragDirect(DRAG_BOTTOM);
                                               tmpPort->setPos(boundRect.bottomRight());
                                           }
                                           else if(tmpPort->getNextDirect() == DRAG_TOP)
                                           {
                                               tmpPort->setDragDirect(DRAG_TOP);
                                               tmpPort->setPos(boundRect.topRight());
                                           }
                                       }
                                       else
                                       {
                                           tmpPort->setArrivalLimitRang(false);
                                       }
                                       return;
                                   }
                                   else
                                   {
                                       if(getRangeValue(boundRect.bottomRight().y(),boundRect.topRight().y(),tmpY,range))
                                       {
                                           if(range == DRAG_RANG_MAX)
                                           {
                                               tmpPort->setNextDirect(DRAG_BOTTOM);
                                           }
                                           else if(range == DRAG_RANG_MIN)
                                           {
                                               tmpPort->setNextDirect(DRAG_TOP);
                                           }
                                       }
                                   }
                              }
                               break;
                case DRAG_BOTTOM:
                              {
                                   tmpX = tmpPort->pos().x() + currPoint.x();
                                   tmpY = boundRect.bottomLeft().y();

                                   //到达线段的边界
                                   bool flag = tmpPort->isArrivalLimitRang();
                                   //到达边界后，
                                   if(flag)
                                   {
                                       if(-currPoint.y()>=MOVE_LIMIT_RANG)
                                       {
                                           if(tmpPort->getNextDirect() == DRAG_RIGHT)
                                           {
                                               tmpPort->setDragDirect(DRAG_RIGHT);
                                               tmpPort->setPos(boundRect.bottomRight());
                                           }
                                           else if(tmpPort->getNextDirect() == DRAG_LEFT)
                                           {
                                               tmpPort->setDragDirect(DRAG_LEFT);
                                               tmpPort->setPos(boundRect.bottomLeft());
                                           }
                                       }
                                       else
                                       {
                                           tmpPort->setArrivalLimitRang(false);
                                       }
                                       return;
                                   }
                                   else
                                   {
                                       if(getRangeValue(boundRect.bottomRight().x(),boundRect.bottomLeft().x(),tmpX,range))
                                       {
                                           if(range == DRAG_RANG_MAX)
                                           {
                                               tmpPort->setNextDirect(DRAG_RIGHT);
                                           }
                                           else if(range == DRAG_RANG_MIN)
                                           {
                                               tmpPort->setNextDirect(DRAG_LEFT);
                                           }
                                       }
                                   }
                              }
                               break;
            }

            newPoint.setX(tmpX);
            newPoint.setY(tmpY);

            tmpPort->setPos(newPoint);
        }
        else if(type == MOUSE_RELEASE)
        {
            qreal scaleFactor;
            switch(tmpPort->getDragDirect())
            {
                case DRAG_LEFT:
                              {
                                   scaleFactor = (tmpPort->pos().y() - boundRect.topLeft().y()) / boundRect.height();
                              }
                               break;
                case DRAG_TOP:
                              {
                                   scaleFactor = (tmpPort->pos().x() - boundRect.topLeft().x()) / boundRect.width();
                              }
                               break;
                case DRAG_RIGHT:
                              {
                                   scaleFactor = (tmpPort->pos().y() - boundRect.topRight().y()) / boundRect.height();
                              }
                               break;
                case DRAG_BOTTOM:
                              {
                                   scaleFactor = (tmpPort->pos().x() - boundRect.topLeft().x()) / boundRect.width();
                              }
                               break;
            }

            tmpPort->setScaleFactor(scaleFactor);
        }

        emit updateSceneDraw();
    }
}

//删除某个端口
void MyItem::procDeleteNodePort(MyNodePort *nodePort)
{
    int index = -1;
    for(int i = 0;i<ports.size();i++)
    {
        if(ports.at(i) ==  nodePort)
        {
            index = i;
            break;
        }
    }

    if(index >= 0)
    {
        ports.at(index)->removeArrows();

        delete ports.at(index);
        ports.removeAt(index);
    }
}

//编辑某个端口
void MyItem::procEditNodePort(MyNodePort *nodePort)
{
    MyGraphicsView::instance()->showNodePortEdit(nodePort);
}

//将当前值和最大及最小值相比较，只能在此范围内,到达最大或最小值的边界时，返回用于设置改变方向
bool MyItem::getRangeValue(qreal maxValue, qreal minValue, qreal &currValue, DragRange& range)
{
    if(currValue >= maxValue)
    {
        currValue = maxValue;
        range = DRAG_RANG_MAX;
        return true;
    }
    else if(currValue <= minValue)
    {
        currValue = minValue;
        range = DRAG_RANG_MIN;
        return true;
    }
    return false;
}

//设置是否拖拽点和旋转点是否可见
void MyItem::setDragPointVisible(bool flag)
{
    leftTopPoint->setVisible(flag);
    rightTopPoint->setVisible(flag);
    leftBottomPoint->setVisible(flag);
    rightBottomPoint->setVisible(flag);

    topPoint->setVisible(flag);
    leftPoint->setVisible(flag);
    rightPoint->setVisible(flag);
    bottomPoint->setVisible(flag);

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
        if(!isSelected()&&(currMouseType == MOUSE_NONE||currMouseType == MOUSE_RELEASE))
        {
            setDragPointVisible(false);
            isNeedBorder = false;
        }
    }
    else if(change == QGraphicsItem::ItemRotationHasChanged && scene())
    {
        leftTopPoint->updateDragPointHoverCursor(rotation());
        rightTopPoint->updateDragPointHoverCursor(rotation());
        leftBottomPoint->updateDragPointHoverCursor(rotation());
        rightBottomPoint->updateDragPointHoverCursor(rotation());

        topPoint->updateDragPointHoverCursor(rotation());
        leftPoint->updateDragPointHoverCursor(rotation());
        rightPoint->updateDragPointHoverCursor(rotation());
        bottomPoint->updateDragPointHoverCursor(rotation());
    }

    return QGraphicsPolygonItem::itemChange(change,value);
}

//根据旋转的角度动态改变拖拽点鼠标悬停时的样式

//左上、右上、左下、右下在缩放时要按照比例进行
//上、左、右、下在缩放时只按改变一边长度
void MyItem::procMouseState(MouseType type,PointType pointType,QPointF currPos)
{

    if(!property.isMoveable)
    {
        return;
    }

    currMouseType = type;

    if(currMouseType == MOUSE_PRESS)
    {

    }
    else if(currMouseType == MOUSE_MOVE)
    {
        qreal leftTopX = mapToScene(boundRect.topLeft()).x();
        qreal leftTopY = mapToScene(boundRect.topLeft()).y();

        qreal rightTopX = mapToScene(boundRect.topRight()).x();
        qreal rightTopY = mapToScene(boundRect.topRight()).y();

        qreal rightBottomX = mapToScene(boundRect.bottomRight()).x();
        qreal rightBottomY = mapToScene(boundRect.bottomRight()).y();

        qreal leftBottomX = mapToScene(boundRect.bottomLeft()).x();
        qreal leftBottomY = mapToScene(boundRect.bottomLeft()).y();

        qreal w = boundRect.width();
        qreal h = boundRect.height();

        //鼠标移动当前坐标值
        qreal px = currPos.x();
        qreal py = currPos.y();
        //移动后x、y、w、h
        qreal tmpX,tmpY,tmpW,tmpH;
        //移动后中心点的x、y
        qreal centerX,centerY;
        //当前旋转的角度
        qreal rotateDegree = rotation();

        qreal  factor = w / h;

        QPainterPath path;

        bool hasProcessed = false;                 //选中的控件在拖拽时是否可用

        switch(pointType)
        {
            case TOP_LEFT:
            case TOP_RIGHT:
            case BOTTOM_LEFT:
            case BOTTOM_RIGHT:
                {
                    if(pointType ==  TOP_LEFT)
                    {
                        tmpW = w - px;
                        tmpH = tmpW /factor;
                        tmpX = tmpW/2;
                        tmpY = tmpH/2;
                        if(rotateDegree == 0)
                        {
                            centerX = rightBottomX - tmpW/2;
                            centerY = rightBottomY - tmpH/2;
                        }
                        else
                        {
                            centerX = rightBottomX - (tmpW/2)*QCOS(rotateDegree);
                            centerY = rightBottomY - (tmpH/2)*QSIN(rotateDegree);
                        }
//                        qDebug()<<px<<"_"<<py<<"_"<<tmpW<<"_"<<tmpH<<"__"<<centerX<<"__"<<centerY<<"__"<<rightBottomX<<"__"<<rightBottomY;
                        prepareGeometryChange();
                        boundRect = QRectF(-tmpX,-tmpY,tmpW,tmpH);
                    }
                    else if(pointType ==  TOP_RIGHT)
                    {
                        tmpW = w + px;
                        tmpH = tmpW /factor;
                        tmpX = tmpW/2;
                        tmpY = tmpH/2;
                        centerX = leftBottomX + tmpW/2;
                        centerY = leftBottomY - tmpH/2;
                        prepareGeometryChange();
                        boundRect = QRectF(-tmpX,-tmpY,tmpW,tmpH);
                    }
                    else if(pointType ==  BOTTOM_LEFT)
                    {
                        tmpW = w - px;
                        tmpH = tmpW /factor;
                        tmpX = tmpW/2;
                        tmpY = tmpH/2;
                        centerX = rightTopX - tmpW/2;
                        centerY = rightTopY + tmpH/2;
                        prepareGeometryChange();
                        boundRect = QRectF(-tmpX,-tmpY,tmpW,tmpH);
                    }
                    else if(pointType ==  BOTTOM_RIGHT)
                    {
                        tmpW = w + px;
                        tmpH = tmpW /factor;
                        tmpX = tmpW/2;
                        tmpY = tmpH/2;
                        centerX = leftTopX + tmpW/2;
                        centerY = leftTopY + tmpH/2;
                        prepareGeometryChange();
                        boundRect = QRectF(-tmpX,-tmpY,tmpW,tmpH);
                    }

                    switch(currItemType)
                    {
                        case GRA_SQUARE:
                        case GRA_RECT:
                            itemPolygon.clear();
                            itemPolygon<<QPointF(-tmpX,-tmpY)<<QPointF(tmpX,-tmpY)<<
                                    QPointF(tmpX,tmpY)<<QPointF(-tmpX,tmpY);
                            hasProcessed = true;
                            break;

                        case GRA_ROUND_RECT:
                            itemPolygon.clear();

                            path.addRoundedRect(boundRect,10,10);
                            itemPolygon = path.toFillPolygon();
                            hasProcessed = true;
                            break;

                        case GRA_CIRCLE:
                            itemPolygon.clear();
                            path.addEllipse(boundRect);
                            itemPolygon = path.toFillPolygon();
                            hasProcessed = true;
                            break;
                        case GRA_ELLIPSE:
                            itemPolygon.clear();
                            path.addEllipse(boundRect);
                            itemPolygon = path.toFillPolygon();
                            hasProcessed = true;
                            break;
                        case GRA_POLYGON:
                            itemPolygon.clear();
                            itemPolygon<<QPointF(-tmpX,0)<<QPointF(0,-tmpY)<<
                                    QPointF(tmpX,0)<<QPointF(0,tmpY);
                            hasProcessed = true;
                            break;
                        case GRA_PARALLELOGRAM:
                            itemPolygon.clear();
                            itemPolygon<<QPointF(-VER_HOR_SCALE*tmpX,-tmpY)<<QPointF(tmpX,-tmpY)<<
                                    QPointF(VER_HOR_SCALE*tmpX,tmpY)<<QPointF(-tmpX,tmpY);
                            hasProcessed = true;
                            break;
                    }
                    break;
                }
            case MIDDLE_RIGHT:
            case MIDDLE_LEFT:
            case TOP_MIDDLE:
            case BOTTOM_MIDDLE:
                {
                    if(currItemType == GRA_SQUARE ||currItemType == GRA_CIRCLE)
                    {
                        break;
                    }
                    if(pointType ==  MIDDLE_RIGHT)
                    {
                        tmpW = w + px /** QCOS(property.rotateDegree)*/;
                        tmpX = tmpW/2;
                        tmpY = h/2;
                        centerX = leftTopX + tmpX;
                        centerY = leftTopY + tmpY;
                        prepareGeometryChange();
                        boundRect = QRectF(-tmpX,-tmpY,tmpW,h);
                    }
                    else if(pointType ==  MIDDLE_LEFT)
                    {
                        tmpW = w - px;
                        tmpX = tmpW/2;
                        tmpY = h/2;
                        centerX = rightBottomX - tmpW/2;
                        centerY = rightBottomY - tmpY;
                        prepareGeometryChange();
                        boundRect = QRectF(-tmpX,-tmpY,tmpW,h);
                    }
                    else if(pointType ==  TOP_MIDDLE)
                    {
                        tmpH = h - py;
                        tmpX = w/2;
                        tmpY = tmpH/2;
                        centerX = rightBottomX - w/2;
                        centerY = rightBottomY - tmpH/2;
                        prepareGeometryChange();
                        boundRect = QRectF(-tmpX,-tmpY,w,tmpH);
                    }
                    else if(pointType ==  BOTTOM_MIDDLE)
                    {
                        tmpH = h + py;
                        tmpX = w/2;
                        tmpY = tmpH/2;
                        centerX = leftTopX + tmpX;
                        centerY = leftTopY + tmpY;
                        prepareGeometryChange();
                        boundRect = QRectF(-tmpX,-tmpY,w,tmpH);
                    }

                    switch(currItemType)
                    {
                        case GRA_RECT:
                            itemPolygon.clear();
                            itemPolygon<<QPointF(-tmpX,-tmpY)<<QPointF(tmpX,-tmpY)<<
                                    QPointF(tmpX,tmpY)<<QPointF(-tmpX,tmpY);
                            hasProcessed = true;
                            break;

                        case GRA_ROUND_RECT:
                            itemPolygon.clear();

                            path.addRoundedRect(boundRect,10,10);
                            itemPolygon = path.toFillPolygon();
                            hasProcessed = true;
                            break;

                        case GRA_ELLIPSE:
                            itemPolygon.clear();
                            path.addEllipse(boundRect);
                            itemPolygon = path.toFillPolygon();
                            hasProcessed = true;
                            break;
                        case GRA_POLYGON:
                            itemPolygon.clear();
                            itemPolygon<<QPointF(-tmpX,0)<<QPointF(0,-tmpY)<<
                                    QPointF(tmpX,0)<<QPointF(0,tmpY);
                            hasProcessed = true;
                            break;
                        case GRA_PARALLELOGRAM:
                            itemPolygon.clear();
                            itemPolygon<<QPointF(-VER_HOR_SCALE*tmpX,-tmpY)<<QPointF(tmpX,-tmpY)<<
                                    QPointF(VER_HOR_SCALE*tmpX,tmpY)<<QPointF(-tmpX,tmpY);
                            hasProcessed = true;
                            break;
                    }
                    break;
                }
        }

        if(hasProcessed)
        {
            setPos(QPointF(centerX,centerY));
            setPolygon(itemPolygon);
            procResizeItem();
            updateRotateLinePos();
            procResizeNodePort();

            property.itemRect.x = mapToScene(boundRect.topLeft()).x()+boundRect.width()/2;
            property.itemRect.y = mapToScene(boundRect.topLeft()).y()+boundRect.height()/2;
            property.itemRect.width = boundRect.width();
            property.itemRect.height = boundRect.height();

            emit propHasChanged(property);
        }
    }
    else if(currMouseType == MOUSE_RELEASE)
    {
        setSelected(true);
    }
    emit updateSceneDraw();
}

//调整8个拖拽矩形的位置
void MyItem::procResizeItem()
{
    leftTopPoint->setPos(boundRect.topLeft());
    rightTopPoint->setPos(boundRect.topRight());
    leftBottomPoint->setPos(boundRect.bottomLeft());
    rightBottomPoint->setPos(boundRect.bottomRight());

    topPoint->setPos(QPointF(0,boundRect.topLeft().y()));
    leftPoint->setPos(QPointF(boundRect.topLeft().x(),0));
    rightPoint->setPos(QPointF(boundRect.bottomRight().x(),0));
    bottomPoint->setPos(QPointF(0,boundRect.bottomLeft().y()));
}

//调整拖入端口的位置
void MyItem::procResizeNodePort()
{
    QPointF newPoint;
    foreach (MyNodePort * nodePort, ports)
    {
        switch(nodePort->getDragDirect())
        {
            case DRAG_LEFT:
                          {
                               newPoint.setX(boundRect.topLeft().x());
                               newPoint.setY(nodePort->getScaleFactor()*boundRect.height() + boundRect.topLeft().y());
                          }
                           break;
            case DRAG_TOP:
                          {
                               newPoint.setX(nodePort->getScaleFactor()*boundRect.width() + boundRect.topLeft().x());
                               newPoint.setY(boundRect.topLeft().y());
                          }
                           break;
            case DRAG_RIGHT:
                          {
                               newPoint.setX(boundRect.topRight().x());
                               newPoint.setY(nodePort->getScaleFactor()*boundRect.height() + boundRect.topRight().y());
                          }
                           break;
            case DRAG_BOTTOM:
                          {
                               newPoint.setX(nodePort->getScaleFactor()*boundRect.width() + boundRect.bottomLeft().x());
                               newPoint.setY(boundRect.bottomLeft().y());
                          }
                           break;
        }

        nodePort->setPos(newPoint);
    }
}

//处理旋转控件旋转后角度的设置
void MyItem::procRotate(MouseType mouseType,qreal degree)
{
    currMouseType = mouseType;

    if(currMouseType == MOUSE_MOVE)
    {
        setRotation(degree);

        property.rotateDegree = degree;
    }
    else if(currMouseType == MOUSE_RELEASE)
    {
        setSelected(true);
    }

//    emit propHasChanged(property);
}

//设置控件的样式属性
void MyItem::setProperty(ItemProperty property)
{
    this->property = property;
    setBrush(property.itemBrush);

    setRotation(property.rotateDegree);
    setZValue(property.zValue);
    setPos(QPointF (property.itemRect.x,property.itemRect.y));

    resetPolygon();

    myTextItem->setPlainText(property.content);
    myTextItem->updateFont(property.itemFont);
    myTextItem->setDefaultTextColor(property.fontColor);
    setText(myTextItem->toPlainText());

    setFlag(QGraphicsItem::ItemIsMovable,property.isMoveable);

    foreach (MyNodePort * node, ports)
    {
        node->setProperty(property);
    }

    rotateLine->setMoveable(property.isMoveable);

    parentScene->update();
}

//本地打开文件后，对控件进行重新设定
void MyItem::resetPolygon()
{
    qreal tw = this->property.itemRect.width;
    qreal th = this->property.itemRect.height;
    qreal tx = tw/2;
    qreal ty = th/2;

    prepareGeometryChange();
    boundRect = QRectF(-tx,-ty,tw,th);

    itemPolygon.clear();

    switch(currItemType)
    {
            //正方形
            case GRA_SQUARE:
                               itemPolygon<<QPointF(-tx,-ty)<<QPointF(tx,-ty)<<
                                      QPointF(tx,ty)<<QPointF(-tx,ty);
                               break;
            //矩形
            case GRA_RECT:
                               itemPolygon<<QPointF(-tx,-ty)<<QPointF(tx,-ty)<<
                                       QPointF(tx,ty)<<QPointF(-tx,ty);
                               break;
           //圆角矩形
           case GRA_ROUND_RECT:
                            {
                                QPainterPath path;
                                path.addRoundedRect(boundRect,10,10);
                                itemPolygon = path.toFillPolygon();
                            }
                              break;
            //圆形
            case GRA_CIRCLE:
                            {
                                QPainterPath path;
                                path.addEllipse(boundRect);
                                itemPolygon = path.toFillPolygon();
                            }
                               break;
            //椭圆
            case GRA_ELLIPSE:
                            {
                                QPainterPath path;
                                path.addEllipse(boundRect);
                                itemPolygon = path.toFillPolygon();
                            }
                               break;
            //菱形
            case GRA_POLYGON:
                            {
                               itemPolygon<<QPointF(-tx,0)<<QPointF(0,-ty)<<
                                      QPointF(tx,0)<<QPointF(0,ty);
                            }
                               break;
            //菱形
            case GRA_PARALLELOGRAM:
                            {
                               itemPolygon<<QPointF(-VER_HOR_SCALE*tx,-ty)<<QPointF(tx,-ty)<<
                                      QPointF(VER_HOR_SCALE*tx,ty)<<QPointF(-tx,ty);
                            }
                               break;
    }
    setPolygon(itemPolygon);

    procResizeItem();
    updateRotateLinePos();
    procResizeNodePort();
}

//左旋转或者右旋转后更新当前属性的旋转角度值
void MyItem::updateRotation(int rotateValue)
{
    property.rotateDegree += rotateValue;
    property.rotateDegree = property.rotateDegree%360;
    emit propHasChanged(property);
}

//重新设定控件的UUID编号，同时更新子节点的父索引值【谨慎使用】
void MyItem::resetItemUUID()
{
    property.createUUID();
}

//设置控件状态【锁定状态:不能改变位置、大小、角度、Z值、样式等特征】
void MyItem::setMoveable(bool lockState)
{
    this->property.isMoveable = lockState;

    setFlag(QGraphicsItem::ItemIsMovable,lockState);

    rotateLine->setMoveable(lockState);

    foreach (MyNodePort* node, ports)
    {
        node->setMoveable(lockState);
    }
}

//键盘事件
void MyItem::keyPressEvent(QKeyEvent *event)
{
    qDebug()<<event->key();
    QGraphicsPolygonItem::keyPressEvent(event);
}

MyItem::~MyItem()
{
    foreach (MyNodePort* node, ports)
    {
        node->removeArrows();
        delete node;
    }
    ports.clear();

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

    if(topPoint)
    {
        delete topPoint;
        topPoint = NULL;
    }

    if(leftPoint)
    {
        delete leftPoint;
        leftPoint = NULL;
    }

    if(rightPoint)
    {
        delete rightPoint;
        rightPoint = NULL;
    }

    if(bottomPoint)
    {
        delete bottomPoint;
        bottomPoint = NULL;
    }

    if(rotateLine)
    {
        delete rotateLine;
        rotateLine = NULL;
    }

    if(myTextItem)
    {
        delete myTextItem;
        myTextItem = NULL;
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
