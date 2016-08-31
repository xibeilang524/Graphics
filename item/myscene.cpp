#include "myscene.h"

#include <QGraphicsSceneMouseEvent>
#include <QDebug>

#include "global.h"
#include "myitem.h"

MyScene::MyScene(QMenu *menu, QObject * parent):
    rightMenu(menu),
    QGraphicsScene(parent)
{
    CurrAddGraType =  GRA_NONE;
}

void MyScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->button () == Qt::LeftButton)
    {
        switch(CurrAddGraType)
        {
             case GRA_SQUARE:
                                MyItem * myItem = new MyItem(rightMenu);
                                myItem->setPos(event->scenePos());
                                addItem(myItem);
                             break;
        }
    }
    else
    {
        CurrAddGraType = GRA_NONE;
        emit resetItemAction();
    }

    QGraphicsScene::mousePressEvent(event);
}

MyScene::~MyScene()
{

}
