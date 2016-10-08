#ifndef MYNODELINE_H
#define MYNODELINE_H

#include "mysuperitem.h"

class MyArrow;
class MyItem;

class MyNodeLine : public MySuperItem
{
public:
    MyNodeLine(GraphicsType type, MyItem * parent1 = 0 ,QObject* parent = 0);
    ~MyNodeLine();
    void addArrow(MyArrow * arrow);
    void removeArrows();
    void removeArrow(MyArrow * arrow);

protected:
    QList<MyArrow *> arrows;           //保存添加的箭头
};

#endif // MYNODELINE_H
