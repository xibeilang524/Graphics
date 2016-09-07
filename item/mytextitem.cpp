#include "mytextitem.h"

#include <QFocusEvent>
#include <QMenu>
#include <QGraphicsSceneMouseEvent>

MyTextItem::MyTextItem(GraphicsType itemType,QMenu * menu,QGraphicsItem *parent, QGraphicsScene *scene):
    menu(menu),
    type(itemType),
    QGraphicsTextItem(parent,scene)
{
    property.isFont = true;

    setFlag(QGraphicsItem::ItemIsMovable);
    setFlag(QGraphicsItem::ItemIsSelectable);
}

void MyTextItem::focusOutEvent(QFocusEvent *event)
{
    setTextInteractionFlags(Qt::NoTextInteraction);
    emit textLostFocus(this);
    QGraphicsTextItem::focusOutEvent(event);
}

void MyTextItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    if (textInteractionFlags() == Qt::NoTextInteraction)
        setTextInteractionFlags(Qt::TextEditorInteraction);
    QGraphicsTextItem::mouseDoubleClickEvent(event);
}

void MyTextItem::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    setSelected(true);
    menu->exec(event->screenPos());
}

void MyTextItem::setProperty(ItemProperty property)
{
    this->property = property;

    setFont(property.itemFont);

    setRotation(property.rotateDegree);

}

MyTextItem::~MyTextItem()
{

}
