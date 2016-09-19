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
*************************************************/
#ifndef MYARROW_H
#define MYARROW_H

#include <QGraphicsLineItem>
#include <QPolygonF>
#include <QDataStream>

#include "../Header.h"
#include "ItemHeader.h"

class MyItem;
class MyNodePort;

class MyArrow : public QGraphicsLineItem
{
public:
    MyArrow(MyItem  * startItem,MyItem  * endItem,QGraphicsItem * parent = 0);
    MyArrow(MyNodePort  * startItem,MyNodePort  * endItem,QGraphicsItem * parent = 0);

    ~MyArrow();

    QRectF boundingRect()const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QPainterPath shape()const;
    void updatePosition();

    MyItem * getStartItem() const{ return startItem; }
    MyItem * getEndItem() const{ return endItem; }

    MyNodePort * getStartNodePort() const{return this->startNodePort;}
    MyNodePort * getEndNodePort() const{return this->endNodePort;}

    void setProperty(ItemProperty property);
    ItemProperty getProperty(){return this->property;}

    friend QDataStream & operator <<(QDataStream &,MyArrow * item);
    friend QDataStream & operator >>(QDataStream &,MyArrow * item);

private:
    MyItem  * startItem;
    MyItem  * endItem;

    MyNodePort * startNodePort;
    MyNodePort * endNodePort;

    LineType lineType;

    QPolygonF arrowHead;

    GraphicsType type;
    ItemProperty property;

    QRectF boundRect;
};

#endif // MYARROW_H
