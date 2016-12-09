#include "mynodeline.h"

#include "myarrow.h"
#include "myitem.h"
#include "mypathitem.h"

MyNodeLine::MyNodeLine(GraphicsType type, MyItem *parent1, QObject *parent2):
    MySuperItem(type,parent1,parent2)
{

}

//保存添加的箭头
void MyNodeLine::addArrow(MyArrow *arrow)
{
    arrows.push_back(arrow);
}

//删除此控件时，移除所有的箭头
void MyNodeLine::removeArrows()
{
    foreach (MyArrow *arrow, arrows)
    {
        arrow->getStartItem()->removeArrow(arrow);
        arrow->getEndItem()->removeArrow(arrow);

        delete arrow;
    }
    arrows.clear();
}

//解除某个箭头的引用，未删除箭头做准备
void MyNodeLine::removeArrow(MyArrow *arrow)
{
    int index = arrows.indexOf(arrow);

    if (index != -1)
        arrows.removeAt(index);
}

//保存折线
void MyNodeLine::addPathLine(MyPathItem *item)
{
    pathLines.push_back(item);
}

//移除所有的折线
void MyNodeLine::removePathLines()
{
    foreach (MyPathItem * pathItem, pathLines)
    {
        pathItem->getStartItem()->removePathLine(pathItem);
        pathItem->getEndItem()->removePathLine(pathItem);

        delete pathItem;
    }
    pathLines.clear();
}

//解除某个折线的引用，未删除箭头做准备
void MyNodeLine::removePathLine(MyPathItem * item)
{
    int index = pathLines.indexOf(item);

    if (index != -1)
        pathLines.removeAt(index);
}

MyNodeLine::~MyNodeLine()
{

}
