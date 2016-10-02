#include "mysuperitem.h"

MySuperItem::MySuperItem(GraphicsType currItemType,QGraphicsItem *parent1, QObject *parent2):
    currItemType(currItemType),
    parentItem(parent1),
    QGraphicsPolygonItem(parent1),
    QObject(parent2)
{

}

MySuperItem::~MySuperItem()
{

}
