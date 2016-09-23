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
**20160909:wey:增加对对当前类的输入输出重载，主要用于文件保存。
**20160912:wey:增加对控件中、左、右、下拉伸
**20160913:wey:增加对控件左斜上、右斜上、左斜下、右斜下拉伸
**             修复保存和解析时，未能按照缩放后的尺寸显示
**20160918:wey:增加端口拖入，支持移动和按边拖入
**20160919:wey:增加端口样式和本控件笔刷绑定
**             修复复制控件时，大小不会复制问题
**             增加删除时同步删除端口集合
**             增加尺寸或者移动端口更新端口连线
**             增加拖入状态切换，允许时选中控件否则取消选择。
**20160920:wey:增加对拖入端口标绘位置十字星
**20160921:wey:修复移动控件后，修改样式位置发生微小偏移
**20160922:wey:添加控件锁定、解锁功能
**20160923:wey:添加端口四个边任意移动功能【！！！】
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
class MyNodePort;

#include <QDataStream>

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

    const QList<MyNodePort *> & getNodePorts(){return this->ports;}

    MyNodePort *addNodePort(const NodePortProperty & prop);
    void removeNodePort(MyNodePort * port);

    void setPos(const QPointF &pos);
    void setZValue(qreal z);

    void resetPolygon();                   //重新设定多边形
    void setDragPointVisible(bool flag);

    void setMoveable(bool lockState);
    bool isMoveable(){return this->property.isMoveable;}

    void resetItemUUID();                  //重新设定控件的UUID编号，同时更新子节点的父索引值

    friend QDataStream & operator <<(QDataStream &,MyItem * item);
    friend QDataStream & operator >>(QDataStream &,MyItem * item);

signals:
    void updateSceneDraw();
    void propHasChanged(ItemProperty);

private slots:
    void procResizeItem();
    void procRotate(MouseType mouseType, int degree);
    void procMouseState(MouseType, PointType pointType, QPointF currPos);
    void procPortChanged(MouseType type, QPointF currPoint);

    void procDeleteNodePort(MyNodePort * nodePort);
    void procEditNodePort(MyNodePort * nodePort);

protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);
    void dragEnterEvent(QGraphicsSceneDragDropEvent *event);
    void dragMoveEvent(QGraphicsSceneDragDropEvent *event);
    void dragLeaveEvent(QGraphicsSceneDragDropEvent *event);
    void dropEvent(QGraphicsSceneDragDropEvent *event);

private:
    void updateRotateLinePos();
    void procResizeNodePort();
    void getRealTimePos();
    qreal getPointToRectMinDistance(QRectF rect,QPointF point);
    bool getRangeValue(qreal maxValue, qreal minValue, qreal & currValue, DragRange &range);
    MyNodePort * createProp(const QPointF pos,const DragDirect direct,const qreal scaleFactor);
    DragDirect getDropDirect(const QPointF &currPoint);

    int radius;

    GraphicsType currItemType;

    QRectF boundRect;

    QPen selectedPen;
    int selectedPenWidth;

    DragPoint * leftTopPoint;
    DragPoint * rightTopPoint;
    DragPoint * leftBottomPoint;
    DragPoint * rightBottomPoint;

    DragPoint * topPoint;
    DragPoint * leftPoint;
    DragPoint * rightPoint;
    DragPoint * bottomPoint;

    MyTextItem * myTextItem;           //文字信息

    RotateLine * rotateLine;
    QGraphicsScene * parentScene;      //窗口的索引

    QMenu * rightMenu;

    MouseType currMouseType;

    QList<MyArrow *> arrows;           //保存添加的箭头
    QPolygonF itemPolygon;             //当前图形的各个顶点坐标集合

    QList<MyNodePort*> ports;          //当前控件端口的集合

    ItemProperty property;             //保存当前属性

    bool isNeedBorder;                 //是否需要选中边框
    bool isDragging;                   //是否有拖入，有拖入后在边框显示拖入点
    DragDirect dragMoveDirect;         //拖动时拖入点在哪个方向上
    QPointF dragMovePoint;             //有拖入时，用于记录拖入移动的点

};

#endif // MYITEM_H
