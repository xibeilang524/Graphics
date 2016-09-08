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
**
*************************************************/
#ifndef MYSCENE_H
#define MYSCENE_H

#include <QGraphicsScene>

#include "../Header.h"

class QMenu;
class MyTextItem;

class MyScene : public QGraphicsScene
{
    Q_OBJECT
public:
    MyScene(QMenu * menu,QObject * parent = 0 );
    ~MyScene();

signals:
    void resetItemAction();
    void deleteKeyPress();

private slots:
    void respTextLostFocus(MyTextItem * item);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);

private:
    QMenu * rightMenu;

    QGraphicsLineItem * insertTmpLine;

};

#endif // MYSCENE_H
