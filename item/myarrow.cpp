#include "myarrow.h"

#include <QPainter>
#include <QRectF>
#include <QLineF>
#include <QDebug>

#include "myitem.h"
#include "mynodeport.h"
#include "mytextitem.h"

#include "math.h"

const qreal Pi = 3.14;
#define ARROW_SIZE 15

QDataStream & operator <<(QDataStream &stream,MyArrow * item)
{
    int type = item->type;

    stream<<type<<item->property;

    return stream;
}

QDataStream & operator >>(QDataStream &stream,MyArrow * item)
{
    int type;
    ItemProperty prop;

    stream>>type>>prop;
    item->type = (GraphicsType)type;

    item->setProperty(prop);

    return stream;
}

MyArrow::MyArrow(MyItem  * startItem,MyItem  * endItem,QGraphicsItem *parent):
    startItem(startItem),
    endItem(endItem),
    QGraphicsLineItem(parent)
{
    setFlag(QGraphicsItem::ItemIsSelectable, true);

    type = GRA_LINE;
    property.itemBrush = QBrush(Qt::black);
    property.itemPen = QPen(Qt::black,2);
    //直线保存两端控件的引用
    property.startItemID = startItem->getProperty().startItemID;
    property.endItemID = endItem->getProperty().startItemID;

    property.lineType = LINE_MYITEM;

    createTextItem();
}

MyArrow::MyArrow(MyNodePort  * startItem,MyNodePort  * endItem,QGraphicsItem *parent):
    startNodePort(startItem),
    endNodePort(endItem),
    QGraphicsLineItem(parent)
{
    setFlag(QGraphicsItem::ItemIsSelectable, true);

    type = GRA_LINE;
    property.itemBrush = QBrush(Qt::black);
    property.itemPen = QPen(Qt::black,2);
    //直线保存两端控件的引用
    property.startItemID = startItem->getNodeProperty().startItemID;
    property.endItemID = endItem->getNodeProperty().startItemID;

    property.lineType = LINE_NODEPORT;

    createTextItem();
}

//创建文字
void MyArrow::createTextItem()
{
    myTextItem = new MyTextItem(GRA_TEXT,NULL,this);
    myTextItem->setTextInteractionFlags(Qt::NoTextInteraction);
    myTextItem->setFlag(QGraphicsItem::ItemIsMovable,false);
    myTextItem->setFlag(QGraphicsItem::ItemIsSelectable,false);
    myTextItem->cleartText();
}

QRectF MyArrow::boundingRect()const
{
    return QRectF(line().p1(), QSizeF(line().p2().x() - line().p1().x(),
                                           line().p2().y() - line().p1().y()));
}

QPainterPath MyArrow::shape()const
{
    QPainterPath path = QGraphicsLineItem::shape();
    if(property.lineType == LINE_MYITEM)
    {
        path.addPolygon(arrowHead);
    }
    return path;
}

//更新样式属性
void MyArrow::setProperty(ItemProperty property)
{
    this->property = property;
    myTextItem->setProperty(property);
    if(property.content.size() > 0)
    {
        setText(property.content);
    }
    update();
}

//设置起点的条类型
void MyArrow::setStartLineType(int type)
{
    this->property.startLineType = (AddLineType)type;
    emit updateSceneDraw();
}

//设置终点的条类型
void MyArrow::setEndLineType(int type)
{
    this->property.endLineType = (AddLineType)type;
    emit updateSceneDraw();
}

void MyArrow::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)

    painter->save();

    if(property.lineType == LINE_MYITEM && startItem && endItem)
    {
        QLineF centerLine(startItem->pos(), endItem->pos());
//        qDebug()<<centerLine;
        QPointF endCrossPoint;
        countItemCrossPoint(true,endItem->polygon(),centerLine,endCrossPoint);

        QPointF startCrossPoint;
        countItemCrossPoint(false,startItem->polygon(),centerLine,startCrossPoint);

        setLine(QLineF(endCrossPoint,startCrossPoint));
    }
    else if(property.lineType == LINE_NODEPORT && startNodePort && endNodePort)
    {
        //获取端口相对整个scene的坐标值
        QPointF startPoint = startNodePort->getParentItem()->mapToScene(startNodePort->pos());
        QPointF endPoint = endNodePort->getParentItem()->mapToScene(endNodePort->pos());

        QLineF centerLine(startPoint, endPoint);

        QPointF endCrossPoint;
        countNodeCrossPoint(endNodePort->getScenePolygon(),centerLine,endCrossPoint);

        QPointF startCrossPoint;
        countNodeCrossPoint(startNodePort->getScenePolygon(),centerLine,startCrossPoint);

        setLine(QLineF(endCrossPoint,startCrossPoint));
    }

    painter->setPen(property.itemPen);
    painter->drawLine(line());

    //起点
    if(property.startLineType == LINE_HORIZONTAL)
    {

    }
    else if(property.startLineType == LINE_ARROW)
    {
        QPointF arrowP1,arrowP2;
        countArrowPoint(true,line().p2(),arrowP1,arrowP2);
        painter->drawLine(line().p2(),arrowP1);
        painter->drawLine(line().p2(),arrowP2);
    }
    else if(property.startLineType == LINE_SOLID_TRIANGLE)
    {
        QPolygonF polygon = countArrowPolygon(line().p2(),true);
        painter->setBrush(property.itemBrush);
        painter->drawPolygon(polygon);
    }

    //终点
    if(property.endLineType == LINE_HORIZONTAL)
    {

    }
    else if(property.endLineType == LINE_ARROW)
    {
        QPointF arrowP1,arrowP2;
        countArrowPoint(false,line().p1(),arrowP1,arrowP2);
        painter->drawLine(line().p1(),arrowP1);
        painter->drawLine(line().p1(),arrowP2);
    }
    else if(property.endLineType == LINE_SOLID_TRIANGLE)
    {
        QPolygonF polygon = countArrowPolygon(line().p1(),false);

        painter->setBrush(property.itemBrush);
        painter->drawPolygon(polygon);
    }

    if (isSelected())
    {
        painter->setPen(QPen(Qt::blue, 1, Qt::DashLine));
        QLineF myLine = line();
        myLine.translate(0, 4.0);
        painter->drawLine(myLine);
        myLine.translate(0,-8.0);
        painter->drawLine(myLine);
    }

    myTextItem->setCentralPos((line().p1()+line().p2())/2);

    painter->restore();
}

//计算箭头
QPolygonF MyArrow::countArrowPolygon(QPointF startPoint,bool isStart)
{
    QPointF arrowP1,arrowP2;
    countArrowPoint(isStart,startPoint,arrowP1,arrowP2);

    QPolygonF  polygon;
    polygon << startPoint << arrowP1 << arrowP2;

    return polygon;
}

void MyArrow::countArrowPoint(bool isStart,QPointF & startPoint,QPointF & point1, QPointF & point2)
{
    double angle = ::acos(line().dx() / line().length());

    if (line().dy() >= 0)
    {
        angle = (Pi * 2) - angle;
    }

    if(isStart)
    {
        point1 = startPoint - QPointF(sin(angle + Pi / 3) * ARROW_SIZE,cos(angle + Pi / 3) * ARROW_SIZE);
        point2 = startPoint - QPointF(sin(angle + Pi - Pi / 3) * ARROW_SIZE,cos(angle + Pi - Pi / 3) * ARROW_SIZE);
    }
    else
    {
        point1 = startPoint + QPointF(sin(angle + Pi / 3) * ARROW_SIZE,cos(angle + Pi / 3) * ARROW_SIZE);
        point2 = startPoint + QPointF(sin(angle + Pi - Pi / 3) * ARROW_SIZE,cos(angle + Pi - Pi / 3) * ARROW_SIZE);
    }
}

//计算端口交叉点的坐标
void MyArrow::countNodeCrossPoint(QPolygonF polygon,QLineF centerLine,QPointF &endIntersectPoint)
{
    QPointF p1 = polygon.first();
    QPointF p2;
    QLineF polyLine;
    //计算起点多边形的中点和终点多边形的交点
    for (int i = 1; i <= polygon.count(); ++i)
    {
        //最后一个需要和第一个进行连线
        if(i == polygon.count())
        {
            p1 = polygon.first();
        }
        else
        {
            p2 = polygon.at(i);
        }

        polyLine = QLineF(p1, p2);
        QLineF::IntersectType intersectType = polyLine.intersect(centerLine, &endIntersectPoint);
        if (intersectType == QLineF::BoundedIntersection)
        {
            break;
        }
        p1 = p2;
    }
}

//计算交叉点的坐标
void MyArrow::countItemCrossPoint(bool isStart , QPolygonF polygon,QLineF centerLine,QPointF &intersectPoint)
{
    //将polygons中item的坐标系转换成scene的坐标系，支持即使控件旋转后依然可以保持箭头指向某一边
    MyItem * tmpItem;
    if(isStart)
    {
        tmpItem = startItem;
    }
    else
    {
        tmpItem = endItem;
    }

    QPointF p1 = tmpItem->mapToScene(polygon.first());
    QPointF p2;
    QLineF polyLine;
    //计算起点多边形的中点和终点多边形的交点
    for (int i = 1; i <= polygon.count(); ++i)
    {
        //最后一个需要和第一个进行连线
        if(i == polygon.count())
        {
            p1 = tmpItem->mapToScene(polygon.first());
        }
        else
        {
            p2 = tmpItem->mapToScene(polygon.at(i));
        }

        polyLine = QLineF(p1, p2);
        QLineF::IntersectType intersectType = polyLine.intersect(centerLine, &intersectPoint);
        if (intersectType == QLineF::BoundedIntersection)
        {
            break;
        }
        p1 = p2;
    }
}

//更新线段的位置
void MyArrow::updatePosition()
{
    if(property.lineType == LINE_NODEPORT)
    {
        QLineF line(mapFromItem(startNodePort, 0, 0), mapFromItem(endNodePort, 0, 0));
        setLine(line);
    }
    else if(property.lineType == LINE_MYITEM)
    {
        QLineF line(mapFromItem(startItem, 0, 0), mapFromItem(endItem, 0, 0));
        setLine(line);
    }
}

//双击编辑文字信息
void MyArrow::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    emit editMe();
    QGraphicsLineItem::mouseDoubleClickEvent(event);
}

QString MyArrow::getText()
{
    return myTextItem->toPlainText();
}

//更新文字信息，同时更新textItem在item中的位置
void MyArrow::setText(QString text)
{
    property.content = text;
    myTextItem->setPlainText(text);

    QRectF rectF = myTextItem->getBoundRect();

    myTextItem->setPos(-rectF.width()/2,-rectF.height()/2);
}

MyArrow::~MyArrow()
{
    startItem = NULL;
    endItem = NULL;

}
