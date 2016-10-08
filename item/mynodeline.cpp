#include "mynodeline.h"

#include "myarrow.h"
#include "myitem.h"

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
}

//解除某个箭头的引用，未删除箭头做准备
void MyNodeLine::removeArrow(MyArrow *arrow)
{
    int index = arrows.indexOf(arrow);

    if (index != -1)
        arrows.removeAt(index);
}

MyNodeLine::~MyNodeLine()
{

}
