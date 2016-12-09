/*************************************************
**版  权：RenGu Company
**文件名: mynodeline.h
**作  者: wey       Version: 1.0       Date: 2016.10.08
**描  述:可产生连接线控件的父类，用于操作箭头
**Others:
**
**修改历史:
**20161208:wey:添加折线
*************************************************/
#ifndef MYNODELINE_H
#define MYNODELINE_H

#include "mysuperitem.h"

class MyArrow;
class MyItem;
class MyPathItem;

class MyNodeLine : public MySuperItem
{
public:
    MyNodeLine(GraphicsType type, MyItem * parent1 = 0 ,QObject* parent = 0);
    ~MyNodeLine();
    void addArrow(MyArrow * arrow);
    void removeArrows();
    void removeArrow(MyArrow * arrow);

    void addPathLine(MyPathItem * item);
    void removePathLines();
    void removePathLine(MyPathItem * item);

    QList<MyArrow *> & getArrows(){return this->arrows;}

protected:
    QList<MyArrow *> arrows;           //保存添加的箭头
    QList<MyPathItem *> pathLines;     //保存添加的折线
};

#endif // MYNODELINE_H
