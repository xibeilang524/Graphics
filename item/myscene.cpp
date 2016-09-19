#include "myscene.h"

#include <QGraphicsSceneMouseEvent>
#include <QTextCursor>
#include <QKeyEvent>
#include <QDebug>

#include "global.h"
#include "myitem.h"
#include "myarrow.h"
#include "mytextitem.h"
#include "mypathitem.h"
#include "mynodeport.h"

#include "typeinfo.h"

MyScene::MyScene(QMenu *menu, QObject * parent):
    rightMenu(menu),
    QGraphicsScene(parent)
{
    CurrAddGraType =  GRA_NONE;
    insertTmpLine = NULL;
    insertTmpPath = NULL;

}

void MyScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    SceneLastClickPoint = event->scenePos();
    if(event->button () == Qt::LeftButton)
    {
        if(CurrAddGraType == GRA_LINE)
        {
            insertTmpLine = new QGraphicsLineItem(QLineF(event->scenePos(),event->scenePos()));
            insertTmpLine->setPen(QPen(Qt::red, 2));
            addItem(insertTmpLine);
        }
        else if(CurrAddGraType == GRA_TEXT)
        {
            MyTextItem  * item = new MyTextItem(CurrAddGraType,rightMenu);
            connect(item,SIGNAL(textLostFocus(MyTextItem *)),this,SLOT(respTextLostFocus(MyTextItem *)));
            connect(item,SIGNAL(posHasChanged(MyRect)),this,SIGNAL(selectedItemPosChanged(MyRect)));

            item->setPos(event->scenePos());
            addItem(item);
        }
        else if(CurrAddGraType == GRA_VECTOR_LINE)
        {
//            insertTmpPath = new MyPathItem;
//            insertTmpPath->setPen(QPen(Qt::blue,2));
//            insertTmpPath->setPos(event->screenPos());
//            addItem(insertTmpPath);
        }
        else if(CurrAddGraType != GRA_NONE)
        {
            addItem(CurrAddGraType,event->scenePos());
        }
    }
    else if(event->button() == Qt::RightButton)
    {
        CurrAddGraType = GRA_NONE;
        emit resetItemAction();
    }

    if(CurrAddGraType != GRA_NONE && CurrAddGraType != GRA_LINE && CurrAddGraType != GRA_VECTOR_LINE)
    {
        CurrAddGraType = GRA_NONE;
        emit resetItemAction();
    }

    QGraphicsScene::mousePressEvent(event);
}

void MyScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if(CurrAddGraType == GRA_LINE && insertTmpLine)
    {
        QLineF newLine(insertTmpLine->line().p1(), event->scenePos());
        insertTmpLine->setLine(newLine);
    }
    else if(CurrAddGraType == GRA_VECTOR_LINE && insertTmpPath)
    {
        QPainterPath path;
        path.moveTo(0,0);
        path.lineTo(event->scenePos());
        insertTmpPath->setPath(path);
        qDebug()<<event->scenePos();
    }
    else
    {
        QGraphicsScene::mouseMoveEvent(event);
    }
}

void MyScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if(CurrAddGraType == GRA_LINE && insertTmpLine)
    {
        QList<QGraphicsItem *> startItems = items(insertTmpLine->line().p1());
        if (startItems.count() && startItems.first() == insertTmpLine)
        {
            startItems.removeFirst();
        }
        QList<QGraphicsItem *> endItems = items(insertTmpLine->line().p2());
        if (endItems.count() && endItems.first() == insertTmpLine)
        {
            endItems.removeFirst();
        }

        removeItem(insertTmpLine);
        delete insertTmpLine;

        if (startItems.count() > 0 && endItems.count() > 0 &&
            startItems.first() != endItems.first())
        {
            QString firstItemId = typeid(* startItems.first()).name();
            QString secondItemId = typeid(* endItems.first()).name();

            if(firstItemId == typeid(MyItem).name() && secondItemId == typeid(MyItem).name())
            {
                MyItem *startItem = qgraphicsitem_cast<MyItem *>(startItems.first());
                MyItem *endItem = qgraphicsitem_cast<MyItem *>(endItems.first());

                if(startItem && endItem)
                {
                    MyArrow *arrow = new MyArrow(startItem, endItem);
                    startItem->addArrow(arrow);
                    endItem->addArrow(arrow);
                    arrow->setZValue(-1000.0);
                    addItem(arrow);
                    arrow->updatePosition();
                }
            }
            else if(firstItemId == typeid(MyNodePort).name() && secondItemId == typeid(MyNodePort).name())
            {
                MyNodePort *startItem = qgraphicsitem_cast<MyNodePort *>(startItems.first());
                MyNodePort *endItem = qgraphicsitem_cast<MyNodePort *>(endItems.first());

                if(startItem && endItem)
                {
                    MyArrow *arrow = new MyArrow(startItem, endItem);
                    startItem->addArrow(arrow);
                    endItem->addArrow(arrow);
                    arrow->setZValue(-1000.0);
                    addItem(arrow);
                    arrow->updatePosition();
                }
            }
        }
    }
    else if(CurrAddGraType == GRA_VECTOR_LINE && insertTmpPath)
    {

    }
    insertTmpLine = 0;
    insertTmpPath = NULL;
    QGraphicsScene::mouseReleaseEvent(event);
}

void MyScene::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Delete)
    {
        emit deleteKeyPress();
    }
}

//添加子控件
void MyScene::addItem(QGraphicsItem *item)
{
    QGraphicsScene::addItem(item);

    emit itemSizeChanged(items().size());
}

void MyScene::addItem(CutInfo cutInfo, bool isCopy)
{
    if(cutInfo.graphicsType == GRA_TEXT)
    {
        MyTextItem  * item = new MyTextItem(cutInfo.graphicsType,rightMenu);
        connect(item,SIGNAL(textLostFocus(MyTextItem *)),this,SLOT(respTextLostFocus(MyTextItem *)));
        connect(item,SIGNAL(posHasChanged(MyRect)),this,SIGNAL(selectedItemPosChanged(MyRect)));

        item->setTextInteractionFlags(Qt::TextEditorInteraction);
        item->setProperty(cutInfo.itemProperty);
        if(isCopy)
        {
            item->setPos(SceneLastClickPoint);
        }
        else
        {
            item->setPos(cutInfo.itemProperty.itemRect.x,cutInfo.itemProperty.itemRect.y);
        }

        item->setPlainText(cutInfo.itemProperty.content);

        addItem(item);
    }
    else if(cutInfo.graphicsType != GRA_NONE && cutInfo.graphicsType != GRA_LINE)
    {
        MyItem * item = new MyItem(cutInfo.graphicsType,rightMenu,this);
        connect(item,SIGNAL(updateSceneDraw()),this,SLOT(update()));
        connect(item,SIGNAL(propHasChanged(ItemProperty)),this,SIGNAL(itemPropChanged(ItemProperty)));

        item->setText(cutInfo.content);
        item->setProperty(cutInfo.itemProperty);
        if(isCopy)
        {
            item->setPos(SceneLastClickPoint);
            item->setSelected(true);
            item->setDragPointVisible(true);
            foreach(NodePortProperty prop,cutInfo.nodeProperties)
            {
                item->addNodePort(prop);
            }
        }
        else
        {
            item->resetPolygon();
            item->setPos(QPointF(cutInfo.itemProperty.itemRect.x,cutInfo.itemProperty.itemRect.y));
        }

        if(!isCopy)
        {
            localItems.push_back(item);
        }

        addItem(item);
    }
    else if(cutInfo.graphicsType == GRA_LINE)
    {
        int startIndex = findItemById(localItems,cutInfo.itemProperty.startItemID);
        int endIndex = findItemById(localItems,cutInfo.itemProperty.endItemID);

        if(startIndex>=0&&startIndex<localItems.size() &&
                endIndex>=0&&endIndex<localItems.size())
        {
            MyItem * startItem = localItems.at(startIndex);
            MyItem * endItem = localItems.at(endIndex);

            MyArrow *arrow = new MyArrow(startItem, endItem);

            startItem->addArrow(arrow);
            endItem->addArrow(arrow);
            arrow->setZValue(-1000.0);

            arrow->updatePosition();
            addItem(arrow);
        }
    }
}

void MyScene::addItem(QList<CutInfo> &cutInfos)
{
    foreach (CutInfo cutInfo, cutInfos)
    {
        addItem(cutInfo);
    }
    localItems.clear();
}

void MyScene::addItem(GraphicsType type, QPointF pos)
{
    MyItem * myItem = new MyItem(type,rightMenu,this);
    myItem->setPos(pos);
    connect(myItem,SIGNAL(updateSceneDraw()),this,SLOT(update()));
    connect(myItem,SIGNAL(propHasChanged(ItemProperty)),this,SIGNAL(itemPropChanged(ItemProperty)));
    addItem(myItem);
}

int MyScene::findItemById(QList<MyItem *> &localItem,QString Id)
{
    int index = -1;

    for (int i=0;i<localItem.size();i++)
    {
        if(localItem.at(i)->getProperty().startItemID == Id)
        {
            index = i;
            break;
        }
    }
    return index;
}

//删除子控件
void MyScene::removeItem(QGraphicsItem *item)
{
    QGraphicsScene::removeItem(item);

    emit itemSizeChanged(items().size());
}

//添加文字后，光标移除，判断内容是否为空，为空则删除
void MyScene::respTextLostFocus(MyTextItem *item)
{
    QTextCursor cursor = item->textCursor();
    cursor.clearSelection();
    item->setTextCursor(cursor);

    if (item->toPlainText().isEmpty())
    {
        removeItem(item);
        item->deleteLater();
    }
}

MyScene::~MyScene()
{

}
