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
*************************************************/
#ifndef MYNODEPORT_H
#define MYNODEPORT_H

#include <QGraphicsObject>
#include <QBrush>

#include "ItemHeader.h"

class QMenu;
class MyItem;

class MyNodePort : public QGraphicsObject
{
    Q_OBJECT
public:
    MyNodePort(MyItem * parentItem);
    ~MyNodePort();

    QRectF boundingRect() const;

    void setPos(const QPointF &pos);

    void setBrush(const QBrush & brush);

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    void setDragDirect(DragDirect direct);
    DragDirect getDragDirect(){return this->dragDirect;}

    void setScaleFactor(qreal scaleFactor);
    qreal getScaleFactor(){return this->scaleFactor;}

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

    QPolygonF itemPolygon;             //当前图形的各个顶点坐标集合

    DragDirect dragDirect;

    qreal scaleFactor;                 //拖入的位置相当于当前一边所在的比例

    QBrush brush;

    QMenu * nodePortRightMenu;         //端口右键菜单，支持删除
};

#endif // MYNODEPORT_H
