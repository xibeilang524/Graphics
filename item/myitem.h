/*************************************************
**版  权：RenGu Company
**文件名: myitem.h
**作  者: wey       Version: 1.0       Date: 2016.09.01
**描  述: 自定义添加控件类型
**Others: 1：支持多种类型控件的创建
**        2：支持右键菜单事件
**        3：支持对样式的修改
**        4：支持移动、拖拽、缩放、旋转等功能（尚有问题）
**        5：支持两控件间的箭头连线，支持删除
**
**修改历史:
**20160908:wey:添加文字描述
**             添加对控件拖拽实时显示位置，同时支持菜单栏设置控件位置
**
*************************************************/
#ifndef MYITEM_H
#define MYITEM_H

#include <QObject>
#include <QGraphicsPolygonItem>
#include <QPolygonF>
#include <QPen>

#include "ItemHeader.h"

class QGraphicsItem;
class DragPoint;
class RotateLine;
class QMenu;
class MyArrow;
class MyTextItem;

#include "dragpoint.h"
#include "rotateline.h"
#include "../Header.h"

class MyItem : public QObject,public QGraphicsPolygonItem
{
    Q_OBJECT
public:
    MyItem(GraphicsType itemType,QMenu * menu,QGraphicsScene * parentScene,QObject * parent1 = 0,QGraphicsItem * parent2 = 0);
    ~MyItem();

    QRectF boundingRect() const;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    void addArrow(MyArrow * arrow);
    void removeArrows();
    void removeArrow(MyArrow * arrow);
    void setProperty(ItemProperty property);
    ItemProperty getProperty(){return this->property;}
    GraphicsType getType(){return this->currItemType;}
    void updateRotation(int rotateValue);
    QString getText();
    void setText(QString text);

    void setPos(const QPointF &pos);

signals:
    void updateSceneDraw();
    void posHasChanged(MyRect);

private slots:
    void procDragSize(PointType type);
    void procResizeItem();
    void procRotate(int degree);
    void procMouseState(MouseType);

protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);

private:
    void setDragPointVisible(bool flag);
    void updateRotateLinePos();

    int radius;

    GraphicsType currItemType;

    QRectF boundRect;

    QPen selectedPen;
    int selectedPenWidth;

    DragPoint * leftTopPoint;
    DragPoint * rightTopPoint;
    DragPoint * leftBottomPoint;
    DragPoint * rightBottomPoint;

    MyTextItem * myTextItem;           //文字信息

    RotateLine * rotateLine;
    QGraphicsScene * parentScene;      //窗口的索引

    QMenu * rightMenu;

    MouseType currMouseType;

    QList<MyArrow *> arrows;           //保存添加的箭头
    QPolygonF itemPolygon;             //当前图形的各个顶点坐标集合

    ItemProperty property;             //保存当前属性

    bool isNeedBorder;                 //是否需要选中边框
};

#endif // MYITEM_H
