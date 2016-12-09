/*************************************************
**版  权：RenGu Company
**文件名: myscene.h
**作  者: wey       Version: 1.0       Date: 2016.09.08
**描  述: 自定义场景
**Others: 1：用于创建、装载其它的子控件，对子控件的管理
**
**
**修改历史:
**20160907:wey:添加对delete键盘事件，支持选中删除
**20160908:wey:添加对item数量的监管
**20160913:wey:修复复制或剪切后按钮未及时更新
**20160921:wey:修复拷贝、剪切控件ID同步复制未更新问题
**             修复添加文字后无法换行、及输入卡顿问题【scene未处理的键盘事件要交由系统处理】
**             添加拖入文字
**20160925:wey:增加键盘对锁定(Ctrl+L)/解锁(Ctrl+U)的支持
**20160927:wey:增加自定义背景
**20160929:wey:调整解锁快捷键(Ctrl+Shift+L)
**             修复控件添加直线，由于鼠标点击在Item的Text上，导致无法添加新直线问题
**             修复控件打开的深度与保存深度不一致问题
**             修复文件打开时，存在线条连接不上问题
**20161002:wey:添加显示item位置和旋转信息MyItemInfo
**20161003:wey:修复添加MyItemInfo，在清空显示导致程序奔溃问题。
**20161008:wey:修复本地打开后，临时数据未清空问题
**20161025:wey:修复从同一控件多次删除连线后，导致奔溃问题
**20161026:wey:添加确保右键菜单只选中一个控件
**20161208:wey:添加折线
*************************************************/
#ifndef MYSCENE_H
#define MYSCENE_H

#include <QGraphicsScene>

#include "../Header.h"

class MyTextItem;
class MyItem;
class MyPathItem;
class MyNodePort;
class MyItemInfo;
class MyNodeLine;
class MyArrow;
class DragLinePoint;

class MyScene : public QGraphicsScene
{
    Q_OBJECT
public:
    MyScene(QObject * parent = 0 );
    void addItem(QGraphicsItem *item);
    void addItem(CutInfo cutInfo,bool isCopy = false);
    void addItem(QList<CutInfo *> & cutInfos);
    void addItem(GraphicsType type,QPointF pos);
    void removeItem(QGraphicsItem *item);
    void setId(QString id){this->uuid = id;}
    QString getId(){return this->uuid;}
    void setAssociation(bool flag);
    bool getAssociation(){return this->isSceneAssociated;}
    void clear();
    ~MyScene();

signals:
    void resetItemAction();
    void deleteKeyPress();
    void ctrlLockKeyPress();
    void ctrlUnLockKeyPress();
    void itemSizeChanged(int size);
    void itemPropChanged(ItemProperty);
    void editCurrItem();
    void editCurrPort();
    void ctrlPropEditKeyPress();

private slots:
    void respTextLostFocus(MyTextItem * item);
    void itemSelectedChanged();
    void showItemRotationInfo(MyItem *);
    void showItemPosInfo(MyItem *);
    void respClearOtherSelectedItems(MyItem*);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);

private:
    int findItemById(QList<MyItem *> &localItem, QString Id);
    int findItemById(QList<MyNodePort *> &localNode, QString Id);
    void addMyItemConnect(MyItem * item);
    void addMyTextConnect(MyTextItem  * item);
    void createItemInfo();
    MyArrow *createArrow(LineType type, MyNodeLine * startNode,MyNodeLine * endNode);
    MyPathItem * createPathItem(LineType type, MyNodeLine *startNode, MyNodeLine *endNode);
    QRectF getHorizonalRoundedRect(MyItem *item);
    void resetItemSelection();

    QGraphicsLineItem * insertTmpLine;
    MyPathItem * insertTmpPath;
    MyItemInfo * myItemInfo;           //控件信息

    QList<MyItem*> localItems;
    QList<MyNodePort*> localNodeports;

    bool isLocalFileOpened;            //是否是本地文件打开
    bool isDragLine;                   //是否处于从nodeline上添加线条状态
    bool isDragPathLine;               //是否处于添加折线的状态
    bool isClear;                      //是否在清空状态，用于控制clear()对MyIemInfo的影响
    bool isSceneAssociated;            //视图是否被关联，如果为false，保存时需要弹出窗口，
    QString uuid;
    QList<MyItem *> mouseItems;        //拖拽连接线时，记录鼠标位置下产生的item
    QString startMouseItemId;          //拖拽线时按下DragLinePoint的父item的id
};

#endif // MYSCENE_H
