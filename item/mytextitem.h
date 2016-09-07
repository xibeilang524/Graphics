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

    GraphicsType getType(){return this->type;}

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
