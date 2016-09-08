/*************************************************
**版  权：RenGu Company
**文件名: mytextitem.h
**作  者: wey       Version: 1.0       Date: 2016.09.06
**描  述: 自定义文字控件
**Others: 1：支持文字的双击编辑
**        2：支持样式的设定，如文字字体的格式
**
**修改历史:
**
*************************************************/
#ifndef MYTEXTITEM_H
#define MYTEXTITEM_H

#include <QGraphicsTextItem>
#include <QObject>

class QMenu;

#include "../Header.h"

class MyTextItem : public QGraphicsTextItem
{
    Q_OBJECT
public:
    MyTextItem(GraphicsType itemType,QMenu * menu,QGraphicsItem *parent2 = 0, QGraphicsScene *scene = 0);
    ~MyTextItem();

    void setProperty(ItemProperty property);
    ItemProperty getProperty(){return this->property;}

    void updateFont(QFont font);

    GraphicsType getType(){return this->type;}

    int getWidth();

//    QVariant itemChange(GraphicsItemChange change, const QVariant &value);

signals:
    void textLostFocus(MyTextItem *item);

protected:
    void focusOutEvent(QFocusEvent *event);
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);

private:
    GraphicsType type;

    ItemProperty property;

    QMenu * menu;
};

#endif // MYTEXTITEM_H
