/*************************************************
**版  权：RenGu Company
**文件名: mytextitem.h
**作  者: wey       Version: 1.0       Date: 2016.09.06
**描  述: 自定义文字控件
**Others: 1：支持文字的双击编辑
**        2：支持样式的设定，如文字字体的格式
**
**修改历史:
**20160921:wey:修复输入文字无法换行问题
**             添加文字初始信息提示并选中状态
**20160927:wey:修复QFontMetrics计算多行文本不正确问题
*************************************************/
#ifndef MYTEXTITEM_H
#define MYTEXTITEM_H

#include <QGraphicsTextItem>
#include <QObject>
#include <QDataStream>

#include "../Header.h"

class MyTextItem : public QGraphicsTextItem
{
    Q_OBJECT
public:
    MyTextItem(GraphicsType itemType,QGraphicsItem *parent2 = 0, QGraphicsScene *scene = 0);
    ~MyTextItem();

    void setProperty(ItemProperty property);
    ItemProperty getProperty(){return this->property;}

    void updateFont(QFont font);
    void cleartText();

    GraphicsType getType(){return this->type;}

    void setTextExistType(TextExistType type);
    TextExistType getTextExistType(){return this->existType;}

    void setZValue(qreal z);

    void setPos(const QPointF &pos);
    void setPos(qreal x, qreal y);
    void setCentralPos(QPointF pos);

    QString getText();
    void setText(QString text);

    void setMoveable(bool lockState);
    bool isMoveable(){return this->property.isMoveable;}

    QRectF getBoundRect();

    friend QDataStream & operator <<(QDataStream &,MyTextItem * item);

    QVariant itemChange(GraphicsItemChange change, const QVariant &value);

signals:
    void textLostFocus(MyTextItem *item);
    void posHasChanged(MyRect);
    void updateTextGeometry();

protected:
    void focusOutEvent(QFocusEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);
    void keyPressEvent(QKeyEvent *event);

private:
    GraphicsType type;
    ItemProperty property;
    TextExistType existType;
    QPixmap pixmap;
};

#endif // MYTEXTITEM_H
