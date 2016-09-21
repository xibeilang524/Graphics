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
**
*************************************************/
#ifndef MYSCENE_H
#define MYSCENE_H

#include <QGraphicsScene>

#include "../Header.h"

class QMenu;
class MyTextItem;
class MyItem;
class MyPathItem;
class MyNodePort;

class MyScene : public QGraphicsScene
{
    Q_OBJECT
public:
    MyScene(QMenu * menu,QObject * parent = 0 );
    void addItem(QGraphicsItem *item);
    void addItem(CutInfo cutInfo,bool isCopy = false);
    void addItem(QList<CutInfo *> & cutInfos);
    void addItem(GraphicsType type,QPointF pos);
    void removeItem(QGraphicsItem *item);
    ~MyScene();

signals:
    void resetItemAction();
    void deleteKeyPress();
    void itemSizeChanged(int size);
    void itemPropChanged(ItemProperty);

private slots:
    void respTextLostFocus(MyTextItem * item);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);

private:
    int findItemById(QList<MyItem *> &localItem, QString Id);
    int findItemById(QList<MyNodePort *> &localNode, QString Id);

    QMenu * rightMenu;

    QGraphicsLineItem * insertTmpLine;
    MyPathItem * insertTmpPath;

    QList<MyItem*> localItems;
    QList<MyNodePort*> localNodeports;
};

#endif // MYSCENE_H
