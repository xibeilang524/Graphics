#include "mytextitem.h"

#include <QFocusEvent>
#include <QMenu>
#include <QGraphicsSceneMouseEvent>
#include <QDebug>

QDataStream & operator <<(QDataStream &stream,MyTextItem * item)
{
    int type = item->type;

    stream<<type<<item->property;

    return stream;
}

QDataStream & operator >>(QDataStream &stream,MyTextItem * item)
{
    int type;
    ItemProperty prop;

    stream>>type>>prop;
    item->type = (GraphicsType)type;

    item->setProperty(prop);

    return stream;
}

MyTextItem::MyTextItem(GraphicsType itemType,QMenu * menu,QGraphicsItem *parent, QGraphicsScene *scene):
    menu(menu),
    type(itemType),
    QGraphicsTextItem(parent,scene)
{
    property.isFont = true;
    property.itemFont = QFont("黑体",15);
    setFont(property.itemFont);

    setTextInteractionFlags(Qt::TextEditorInteraction);
    setFlag(QGraphicsItem::ItemIsMovable);
    setFlag(QGraphicsItem::ItemIsSelectable);
}

void MyTextItem::focusOutEvent(QFocusEvent *event)
{
    setTextInteractionFlags(Qt::NoTextInteraction);
    property.content = toPlainText();
    emit textLostFocus(this);
    property.itemRect.x = this->x();
    property.itemRect.y = this->y();
    property.itemRect.width = getBoundRect().width();
    property.itemRect.height = getBoundRect().height();
    QGraphicsTextItem::focusOutEvent(event);
}

void MyTextItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    property.itemRect.x = this->x();
    property.itemRect.y = this->y();
    property.itemRect.width = getBoundRect().width();
    property.itemRect.height = getBoundRect().height();
    emit posHasChanged(property.itemRect);
    QGraphicsTextItem::mouseMoveEvent(event);
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

    setDefaultTextColor(property.fontColor);
    setFont(property.itemFont);
    setZValue(property.zValue);

    setRotation(property.rotateDegree);
}

void MyTextItem::setZValue(qreal z)
{
    property.zValue = z;
    QGraphicsTextItem::setZValue(z);
}

//从item中更新字体样式
void MyTextItem::updateFont(QFont font)
{
    property.itemFont = font;
    setFont(property.itemFont);
}

//根据显示的数据来动态计算宽度
QRectF MyTextItem::getBoundRect()
{
    QFontMetricsF metrics = property.itemFont;
    return metrics.boundingRect(toPlainText());
}

//QVariant MyTextItem::itemChange(GraphicsItemChange change, const QVariant &value)
//{
//    return QGraphicsTextItem::itemChange(change,value);
//}

MyTextItem::~MyTextItem()
{

}
