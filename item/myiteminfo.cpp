#include "myiteminfo.h"

#include <QPainter>

MyItemInfo::MyItemInfo(GraphicsType type, QGraphicsItem *parent1, QObject *parent2):
    MySuperItem(type,parent1,parent2)
{
    radius = 22;

    prepareGeometryChange();

    boundRect = QRectF(-2*radius,-radius,radius * 4,radius);

    QPainterPath path;
    path.addRoundedRect(boundRect,5,5);
    itemPolygon = path.toFillPolygon();
    setPolygon(itemPolygon);

    setFlags(QGraphicsItem::ItemSendsGeometryChanges);
}

QRectF MyItemInfo::boundingRect()const
{
    return boundRect;
}

void MyItemInfo::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget)
    Q_UNUSED(option)
    painter->save();

    painter->setBrush(GLOBAL_ITEM_BRUSH);

    painter->drawPolygon(itemPolygon);

    painter->setPen(QPen(Qt::black,10));
    painter->drawText(boundRect,Qt::AlignCenter,showText);

    painter->restore();
}

//设置显示旋转信息
void MyItemInfo::setRotationInfo(int degree)
{
    showText = QString("%1°").arg(degree);
    update();
}

//设置显示位置信息
void MyItemInfo::setPosInfo(int x, int y)
{
    showText = QString("X:%1,Y:%2").arg(x).arg(y);
    update();
}

MyItemInfo::~MyItemInfo()
{

}
