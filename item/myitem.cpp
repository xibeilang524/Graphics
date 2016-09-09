#include "myitem.h"

#include <QGraphicsSceneMouseEvent>
#include <QGraphicsSceneHoverEvent>
#include <QGraphicsSceneContextMenuEvent>
#include <QPainter>
#include <QMenu>
#include <QDebug>
#include <QGraphicsScene>

#include "myarrow.h"
#include "mytextitem.h"

//对MyRect的输出进行重构
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

////写入属性
QDataStream & operator <<(QDataStream & stream,ItemProperty & prop)
{
    stream<<prop.isNeedBrush<<prop.itemBrush
            <<prop.isNeedBorder<<prop.itemPen<<prop.itemRect
               <<prop.alphaValue<<prop.rotateDegree
                 <<prop.isFont<<prop.content<<prop.itemFont<<prop.fontColor;

    return stream;
}

////读属性
QDataStream & operator >>(QDataStream & stream,ItemProperty & prop)
{
    stream>>prop.isNeedBrush>>prop.itemBrush
            >>prop.isNeedBorder>>prop.itemPen>>prop.itemRect
               >>prop.alphaValue>>prop.rotateDegree
                 >>prop.isFont>>prop.content>>prop.itemFont>>prop.fontColor;
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
    radius = 100;

    setFlag(QGraphicsItem::ItemIsMovable,true);
    setFlag(QGraphicsItem::ItemIsSelectable,true);

    prepareGeometryChange();

    float factor;

    switch(currItemType)
    {
            //正方形
            case GRA_SQUARE:
                               boundRect = QRectF(-radius,-radius,2*radius,2*radius);   //设置范围时同时也默认指定了其中心点坐标(0,0)
                               itemPolygon<<QPointF(-radius,-radius)<<QPointF(radius,-radius)<<
                                      QPointF(radius,radius)<<QPointF(-radius,radius);

                               property.itemBrush = QBrush(Qt::red);
                               break;
            //矩形
            case GRA_RECT:
                               factor = 0.5;
                               boundRect = QRectF(-radius,-factor*radius,2*radius,radius);
                               itemPolygon<<QPointF(-radius,-factor*radius)<<QPointF(radius,-factor*radius)<<
                                       QPointF(radius,factor*radius)<<QPointF(-radius,factor*radius);

                               property.itemBrush = QBrush(Qt::blue);
                               break;
           //圆角矩形
           case GRA_ROUND_RECT:
                              {
                                  boundRect = QRectF(-radius,-0.5*radius,2*radius,radius);
                                  QPainterPath path;
                                  path.addRoundedRect(boundRect,10,10);
                                  itemPolygon = path.toFillPolygon();

                                  property.itemBrush = QBrush(Qt::yellow);
                              }
                              break;
            //圆形
            case GRA_CIRCLE:
                               {
                                   boundRect = QRectF(-radius,-radius,2*radius,2*radius);
                                   QPainterPath path;
                                   path.addEllipse(boundRect);
                                   itemPolygon = path.toFillPolygon();

                                   property.itemBrush = QBrush(Qt::darkCyan);
                               }
                               break;
            //椭圆
            case GRA_ELLIPSE:
                               {
                                   boundRect = QRectF(-radius,-0.5*radius,2*radius,radius);
                                   QPainterPath path;
                                   path.addEllipse(boundRect);
                                   itemPolygon = path.toFillPolygon();

                                   property.itemBrush = QBrush(Qt::darkMagenta);
                               }
                               break;
            //菱形
            case GRA_POLYGON:
                               factor = 0.5;
                               boundRect = QRectF(-radius,-factor*radius,2*radius,radius);
                               itemPolygon<<QPointF(-radius,-factor*radius)<<QPointF(0.5*radius,-factor*radius)<<
                                         QPointF(radius,factor*radius)<<QPointF(-0.5*radius,factor*radius);

                               property.itemBrush = QBrush(Qt::gray);
                               break;
    }
    setPolygon(itemPolygon);
    setBrush(property.itemBrush);

    property.itemRect.width = boundRect.width();
    property.itemRect.height = boundRect.height();


    property.itemFont = QFont("黑体",15);

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

    myTextItem = new MyTextItem(GRA_TEXT,menu,this);

    myTextItem->setTextInteractionFlags(Qt::NoTextInteraction);
    myTextItem->setFlag(QGraphicsItem::ItemIsMovable,false);
    myTextItem->setFlag(QGraphicsItem::ItemIsSelectable,false);

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

QString MyItem::getText()
{
    return myTextItem->toPlainText();
}

//更新文字信息，同时更新textItem在item中的位置
void MyItem::setText(QString text)
{
    property.content = text;
    myTextItem->setPlainText(text);
    myTextItem->setPos(-myTextItem->getBoundRect().width()/2,-myTextItem->getBoundRect().height()/2);
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

void MyItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
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
    emit updateSceneDraw();

    property.itemRect.x = this->x();
    property.itemRect.y = this->y();
    property.itemRect.width = this->boundingRect().width();
    property.itemRect.height = this->boundingRect().height();
    emit posHasChanged(property.itemRect);

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

//    qDebug()<<boundRect.width()<<"=="<<boundRect.height();

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

//设置控件的样式属性
void MyItem::setProperty(ItemProperty property)
{
    this->property = property;
    setBrush(property.itemBrush);

    setRotation(property.rotateDegree);

    setPos(QPointF (property.itemRect.x,property.itemRect.y));

    //对于正方形和圆要保持宽高一致
//    itemPolygon.clear();
    prepareGeometryChange();
    int x,y;
//    if(currItemType == GRA_SQUARE || currItemType == GRA_CIRCLE)
//    {
//        qDebug()<<"==============="<<property.itemRect.width;
//        radius = property.itemRect.width /2 ;

//        boundRect = QRectF(-radius,-radius,2*radius,2*radius);

//        itemPolygon<<QPointF(-radius,-radius)<<QPointF(radius,-radius)<<
//              QPointF(radius,radius)<<QPointF(-radius,radius);

//        x = pos().x() + radius;
//        y = pos().y() + radius;

//        setPos(property.itemRect.x,property.itemRect.y);

//        qDebug()<<sceneBoundingRect().x()<<"=="<<sceneBoundingRect().y()<<"++"<<x<<"=="<<y<<"__"<<radius;

//    }
//    else
//    {

//    }

//    setPolygon(itemPolygon);

//    setPos(x,y);

    procResizeItem();
    updateRotateLinePos();

    myTextItem->setPlainText(property.content);
    myTextItem->updateFont(property.itemFont);
    myTextItem->setDefaultTextColor(property.fontColor);
    setText(myTextItem->toPlainText());

    parentScene->update();

//    qDebug()<<sceneBoundingRect().x()<<"=after="<<sceneBoundingRect().y()<<"++"<<x<<"=="<<y<<"__"<<radius;
}

//左旋转或者右旋转后更新当前属性的旋转角度值
void MyItem::updateRotation(int rotateValue)
{
    property.rotateDegree += rotateValue;
    property.rotateDegree = property.rotateDegree%360;

}

MyItem::~MyItem()
{
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
