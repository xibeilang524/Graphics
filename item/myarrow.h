/*************************************************
**版  权：RenGu Company
**文件名: myarrow.h
**作  者: wey       Version: 1.0       Date: 2016.09.01
**描  述: 带箭头的线
**Others: 1：支持样式的设置
**        2：支持与其它控件的连接
**
**修改历史:
**20160907:wey:添加箭头的绘制
**20160908:wey:支持控件旋转后，箭头依然与控件边相连
**20160920:wey:添加端口箭头绘制
**20160927:wey:添加文字信息
**20160928:wey:添加对箭头类型的切换
**20150929:wey:修复缩放控件时导致箭头长度不变问题
**             修复箭头方向不正确问题
**             添加鼠标进入和离开改变样式，方便状态显示
**20161008:wey:调整连接线绘制起始点至控件一遍的中点
**20161017:gx:修复连接线选择区域小的问题(目前在垂直或水平状态下选中效果依然不好)
**20161110:wey:添加右键菜单功能
**20161209:wey:修复在paint中实时计算当前位置信息，以提高效率
*************************************************/
#ifndef MYARROW_H
#define MYARROW_H

#include <QGraphicsLineItem>
#include <QPolygonF>
#include <QDataStream>

#include "../Header.h"

#include <QObject>

class MyNodePort;
class MyTextItem;
class MyNodeLine;

class MyArrow :public QObject, public QGraphicsLineItem
{
    Q_OBJECT
public:
    MyArrow(QGraphicsItem * parent = 0);

    ~MyArrow();

    QRectF boundingRect()const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QPainterPath shape()const;
    void updatePosition();

    void setStartItem(MyNodeLine * startItem);
    MyNodeLine * getStartItem() const{ return startItem; }

    void setEndItem(MyNodeLine * endItem);
    MyNodeLine * getEndItem() const{ return endItem; }

    void setStartPointType(PointType type);
    PointType getStartPointType(){return this->property.startPointType;}

    void setEndPointType(PointType type);
    PointType getEndPointType(){return this->property.endPointType;}

    void setStartItemID(const QString id);
    void setEndItemID(const QString id);

    //获取起始和结束端线条箭头的类型(直线、箭头、实心箭头)
    AddLineType getStartLineType(){return this->property.startLineType;}
    void setStartLineType(int type);

    AddLineType getEndLineType(){return this->property.endLineType;}
    void setEndLineType(int type);

    void setLineType(LineType lineType);
    LineType getLineType()const {return this->property.lineType;}

    void setProperty(ItemProperty property);
    ItemProperty getProperty(){return this->property;}

    QString getText();
    void setText(QString text);

    friend QDataStream & operator <<(QDataStream &,MyArrow * item);
    friend QDataStream & operator >>(QDataStream &,MyArrow * item);

signals:
    void editMe();
    void updateSceneDraw();

protected:
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);

private:
    void countNodeCrossPoint(QPolygonF polygon,QLineF centerLine,QPointF &endIntersectPoint);
    void countItemCrossPoint(bool isStart, QPolygonF polygon, QLineF centerLine, QPointF &endIntersectPoint);
    void countArrowPoint(bool isStart, QPointF startPoint, QPointF &point1, QPointF &point2);
    QPolygonF countArrowPolygon(QPointF startPoint, bool isStart);

    void createTextItem();

    MyNodeLine * startItem;
    MyNodeLine * endItem;

    QPolygonF arrowHead;

    GraphicsType type;
    ItemProperty property;

    QRectF boundRect;

    MyTextItem * myTextItem;           //文字信息
};

#endif // MYARROW_H
