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
*************************************************/
#ifndef MYNODEPORT_H
#define MYNODEPORT_H

#include <QGraphicsObject>
#include <QBrush>

#include "ItemHeader.h"
#include "../Header.h"

#include <QDataStream>

class QMenu;
class MyItem;
class MyArrow;

class MyNodePort : public QGraphicsObject
{
    Q_OBJECT
public:
    MyNodePort(MyItem * parentItem);
    ~MyNodePort();

    QRectF boundingRect() const;

    void setPos(const QPointF &pos);

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    void setDragDirect(DragDirect direct);
    DragDirect getDragDirect(){return this->nodeProperty.direct;}

    void setScaleFactor(qreal scaleFactor);
    qreal getScaleFactor(){return this->nodeProperty.scaleFactor;}

    void setProperty(ItemProperty property);

    GraphicsType getType()const{return this->type;}

    void setNodeProperty(NodePortProperty prop);
    NodePortProperty  getNodeProperty(){return this->nodeProperty;}

    QPolygonF getScenePolygon();

    MyItem * getParentItem(){return this->parentMyItem;}

    friend QDataStream & operator >>(QDataStream & ,MyNodePort *);
    friend QDataStream & operator <<(QDataStream & ,MyNodePort *);

    void addArrow(MyArrow * arrow);
    void removeArrows();
    void removeArrow(MyArrow * arrow);

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

private:
    void initNodePortRightMenu();

    QRectF boundRect;
    int radius;

    GraphicsType  type;                //节点类型

    NodePortProperty nodeProperty;     //端口属性

    QMenu * nodePortRightMenu;         //端口右键菜单，支持删除

    QList<MyArrow *> arrows;           //保存添加的箭头

    MyItem * parentMyItem;             //父节点
};

#endif // MYNODEPORT_H
