/*************************************************
**版  权：RenGu Company
**文件名: mynodeport.h
**作  者: wey       Version: 1.0       Date: 2016.09.18
**描  述: 控件上的端口
**Others:1.需要支持拖入
**       2.支持移动、动态添加
**       3.支持连线
**
**修改历史:
**20160919:wey:添加右键删除事件
**             添加右键编辑功能
**             增加连线功能
**20160920:wey:增加本地保存
**20160930:wey:调整父类QGraphicsPolygonItem
**20161027:wey:增加三角和D端口
**20161028:wey:增加文字信息显示
**             调整右键菜单由全局同一创建
*************************************************/
#ifndef MYNODEPORT_H
#define MYNODEPORT_H

#include <QGraphicsPolygonItem>
#include <QObject>
#include <QBrush>

#include "../Header.h"
#include "mynodeline.h"

#include <QDataStream>

class QMenu;
class MyItem;
class MyArrow;
class MyTextItem;

class MyNodePort : public MyNodeLine
{
    Q_OBJECT
public:
    MyNodePort(GraphicsType type, MyItem * parentItem, QObject * parent1 = 0);
    ~MyNodePort();

    QRectF boundingRect() const;

    void setPos(const QPointF &pos);

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    void setDragDirect(DragDirect direct);
    DragDirect getDragDirect(){return this->nodeProperty.direct;}

    void setNextDirect(DragDirect direct);
    DragDirect getNextDirect(){return this->nextDirect;}

    bool isArrivalLimitRang(){return this->arrivalLimitRang;}
    void setArrivalLimitRang(bool flag);

    void setScaleFactor(qreal scaleFactor);
    qreal getScaleFactor(){return this->nodeProperty.scaleFactor;}

    void setProperty(ItemProperty property);

    void setNodeProperty(NodePortProperty prop);
    NodePortProperty  getNodeProperty(){return this->nodeProperty;}

    QPolygonF getScenePolygon();

    void updatePortID(QString portId);

    void setMoveable(bool isMoveable);

    friend QDataStream & operator >>(QDataStream & ,MyNodePort *);
    friend QDataStream & operator <<(QDataStream & ,MyNodePort *);

    QString getText();
    void setText(QString text);

signals:
    void deletePort(MyNodePort *);
    void editPort(MyNodePort *);
    void portPosChanged(MouseType,QPointF);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);

private slots:
    void respDeleteAction();
    void respEditAction();
    void updateTextPosByDirect();

private:
    void initWidget(MyItem *parentItem);
    void initNodePortRightMenu();
    bool isMoveable;                   //是否支持移动，用于父窗口锁定

    NodePortProperty nodeProperty;     //端口属性
    DragDirect nextDirect;             //端口到达拐点时，此变量会被赋值，鼠标在下一时刻移动时会参考当前方向以及下个方向
    bool arrivalLimitRang;             //端口是否到达线段的端点

    QMenu * nodePortRightMenu;         //端口右键菜单，支持删除
    MyTextItem * myTextItem;           //文字信息

    QList<MyArrow *> arrows;           //保存添加的箭头
};

#endif // MYNODEPORT_H
