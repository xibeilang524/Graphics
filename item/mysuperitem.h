/*************************************************
**版  权：RenGu Company
**文件名: mysuperitem.h
**作  者: wey       Version: 1.0       Date: 2016.09.30
**描  述:
**Others: 除线条外所有控件的父类
**
**修改历史:
**
*************************************************/
#ifndef MYSUPERITEM_H
#define MYSUPERITEM_H

#include <QObject>
#include <QGraphicsPolygonItem>

#include "../Header.h"

class MySuperItem : public QObject,public QGraphicsPolygonItem
{
    Q_OBJECT
public:
    MySuperItem(GraphicsType currItemType,QGraphicsItem * parent1 = 0,QObject * parent2 = 0);
    ~MySuperItem();

    ItemProperty getProperty(){return this->property;}
    GraphicsType getType(){return this->currItemType;}
    QGraphicsItem * getParentItem(){return this->parentItem;}

protected:
    QRectF boundRect;                    //控件外围边框
    ItemProperty property;               //保存当前属性
    QPolygonF itemPolygon;               //当前图形的各个顶点坐标集合
    qreal radius;                        //控件的半径
    GraphicsType currItemType;           //控件的类型

    QGraphicsItem * parentItem;          //父控件
};

#endif // MYSUPERITEM_H
