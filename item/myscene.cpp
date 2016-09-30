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
#include "draglinepoint.h"
#include "../util.h"

#include "typeinfo.h"

MyScene::MyScene(QMenu *menu, QObject * parent):
    rightMenu(menu),
    QGraphicsScene(parent)
{
    CurrAddGraType =  GRA_NONE;
    insertTmpLine = NULL;
    insertTmpPath = NULL;
    isLocalFileOpened = false;
    isDragLine = false;

    setBackgroundBrush(QPixmap(":/images/backgroundRole.png"));
}

void MyScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    SceneLastClickPoint = event->scenePos();
    if(event->button () == Qt::LeftButton)
    {
        if(itemAt(event->scenePos()) && TYPE_ID(*itemAt(event->scenePos())) == TYPE_ID(DragLinePoint))
        {
            insertTmpLine = new QGraphicsLineItem(QLineF(event->scenePos(),event->scenePos()));
            insertTmpLine->setPen(QPen(Qt::red, 2));
            insertTmpLine->setZValue(1000);
            addItem(insertTmpLine);
            isDragLine = true;
        }

        if(!isDragLine && CurrAddGraType == GRA_LINE)
        {
            insertTmpLine = new QGraphicsLineItem(QLineF(event->scenePos(),event->scenePos()));
            insertTmpLine->setPen(QPen(Qt::red, 2));
            addItem(insertTmpLine);
        }
        else if(CurrAddGraType == GRA_TEXT)
        {
            MyTextItem  * item = new MyTextItem(CurrAddGraType,rightMenu);
            connect(item,SIGNAL(textLostFocus(MyTextItem *)),this,SLOT(respTextLostFocus(MyTextItem *)));
//            connect(item,SIGNAL(posHasChanged(MyRect)),this,SIGNAL(selectedItemPosChanged(MyRect)));

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
//    if(itemAt(event->scenePos()))
//    {
//        qDebug()<<TYPE_ID(*itemAt(event->scenePos()));
//    }

    if((isDragLine||CurrAddGraType == GRA_LINE)&& insertTmpLine)
    {
        QLineF newLine(insertTmpLine->line().p1(), event->scenePos() - QPointF(2,2));
        insertTmpLine->setLine(newLine);
    }
    else if(CurrAddGraType == GRA_VECTOR_LINE && insertTmpPath)
    {
        QPainterPath path;
        path.moveTo(0,0);
        path.lineTo(event->scenePos());
        insertTmpPath->setPath(path);
    }
    else
    {
        if(selectedItems().size() == 0)
        {

        }

        QGraphicsScene::mouseMoveEvent(event);
    }    
}

//在拖拽产生直线时，可能鼠标按在控件的Text上，那么需要进一步的判断
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
            QString firstItemId = TYPE_ID(* startItems.first());
            QString secondItemId = TYPE_ID(* endItems.first());

            MyItem *startItem = qgraphicsitem_cast<MyItem *>(startItems.first());
            MyItem *endItem = qgraphicsitem_cast<MyItem *>(endItems.first());

            if(firstItemId == TYPE_ID(MyTextItem) && startItems.size() > 1)
            {
                firstItemId = TYPE_ID(* startItems.at(1));
                startItem = qgraphicsitem_cast<MyItem *>(startItems.at(1));
            }

            if(secondItemId == TYPE_ID(MyTextItem) && endItems.size() > 1)
            {
                secondItemId = TYPE_ID(* endItems.at(1));
                endItem = qgraphicsitem_cast<MyItem *>(endItems.at(1));
            }

            if(firstItemId == TYPE_ID(MyItem) && secondItemId == TYPE_ID(MyItem) &&
                    startItem && endItem)
            {
                MyArrow *arrow = new MyArrow(startItem, endItem);
                connect(arrow,SIGNAL(editMe()),this,SIGNAL(editCurrItem()));
                connect(arrow,SIGNAL(updateSceneDraw()),this,SLOT(update()));

                startItem->addArrow(arrow);
                endItem->addArrow(arrow);
                arrow->setZValue(-1000.0);
                addItem(arrow);
                arrow->updatePosition();
            }
            else if(firstItemId == TYPE_ID(MyNodePort) && secondItemId == TYPE_ID(MyNodePort))
            {
                MyNodePort *startItem = qgraphicsitem_cast<MyNodePort *>(startItems.first());
                MyNodePort *endItem = qgraphicsitem_cast<MyNodePort *>(endItems.first());

                if(startItem && endItem)
                {
                    MyArrow *arrow = new MyArrow(startItem, endItem);
                    connect(arrow,SIGNAL(editMe()),this,SIGNAL(editCurrItem()));
                    connect(arrow,SIGNAL(updateSceneDraw()),this,SLOT(update()));

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

    insertTmpLine = NULL;
    insertTmpPath = NULL;
    QGraphicsScene::mouseReleaseEvent(event);
}

void MyScene::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Delete)
    {
        emit deleteKeyPress();
    }
    else if(event->modifiers() == Qt::ControlModifier && event->key() == Qt::Key_L)
    {
        emit ctrlLockKeyPress();
    }
    else if(event->modifiers() == Qt::ControlModifier && event->modifiers() == Qt::ShiftModifier && event->key() == Qt::Key_L)
    {
        emit ctrlUnLockKeyPress();
    }
    QGraphicsScene::keyPressEvent(event);
}

//添加子控件
void MyScene::addItem(QGraphicsItem *item)
{
    QGraphicsScene::addItem(item);

    emit itemSizeChanged(items().size());
}

///*****************************************************
///**Function:
///**Description:1:用于复制、剪切时调用.2:用于本地打开时调用
///**Input:
///**Output:
///**Return:
///**Others:
///****************************************************/
void MyScene::addItem(CutInfo cutInfo, bool isCopy)
{
    if(cutInfo.graphicsType == GRA_TEXT)
    {
        MyTextItem  * item = new MyTextItem(cutInfo.graphicsType,rightMenu);
        addMyTextConnect(item);
//        connect(item,SIGNAL(posHasChanged(MyRect)),this,SIGNAL(selectedItemPosChanged(MyRect)));

        item->setTextInteractionFlags(Qt::TextEditorInteraction);

        //不是本地打开，需要手动的为每个图层指定一个纵向深度值。为了下次打开时候可以按照保存样式恢复
        if(!isLocalFileOpened)
        {
            cutInfo.itemProperty.zValue = Util::getGlobalZValue();
        }

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
        addMyItemConnect(item);

        if(!isLocalFileOpened)
        {
            cutInfo.itemProperty.zValue = Util::getGlobalZValue();
        }

        item->setText(cutInfo.content);
        item->setProperty(cutInfo.itemProperty);
        if(isCopy)
        {
            item->setPos(SceneLastClickPoint);
            item->setSelected(true);
            item->setDragPointVisible(true);
            item->resetItemUUID();         //拷贝时，要将新控件的ID号进行更新【!!!!】
        }
        else
        {
            item->resetPolygon();
            item->setPos(QPointF(cutInfo.itemProperty.itemRect.x,cutInfo.itemProperty.itemRect.y));
        }

        item->setMoveable(cutInfo.itemProperty.isMoveable);

        //本地打开时，由于创建时会产生新的ID，但需要手动的更新
        foreach(NodePortProperty prop,cutInfo.nodeProperties)
        {
            MyNodePort * tmp = item->addNodePort(prop);
            if(!isCopy)
            {
                tmp->updatePortID(prop.startItemID);
                localNodeports.push_back(tmp);
            }
        }

        //本地打开时需要保存
        if(!isCopy)
        {
            localItems.push_back(item);
        }

        addItem(item);
    }
    else if(cutInfo.graphicsType == GRA_LINE)
    {

        if(cutInfo.itemProperty.lineType == LINE_MYITEM)
        {
            int startIndex = findItemById(localItems,cutInfo.itemProperty.startItemID);
            int endIndex = findItemById(localItems,cutInfo.itemProperty.endItemID);

            if(startIndex>=0&&startIndex<localItems.size() &&
                    endIndex>=0&&endIndex<localItems.size())
            {
                MyItem * startItem = localItems.at(startIndex);
                MyItem * endItem = localItems.at(endIndex);

                MyArrow *arrow = new MyArrow(startItem, endItem);
                connect(arrow,SIGNAL(editMe()),this,SIGNAL(editCurrItem()));
                connect(arrow,SIGNAL(updateSceneDraw()),this,SLOT(update()));

                startItem->addArrow(arrow);
                endItem->addArrow(arrow);
                arrow->setZValue(-1000.0);
                arrow->setProperty(cutInfo.itemProperty);

                arrow->updatePosition();
                addItem(arrow);
            }
        }
        else if(cutInfo.itemProperty.lineType == LINE_NODEPORT)
        {
            int startIndex = findItemById(localNodeports,cutInfo.itemProperty.startItemID);
            int endIndex = findItemById(localNodeports,cutInfo.itemProperty.endItemID);
            if(startIndex>=0&&startIndex<localNodeports.size() &&
                    endIndex>=0&&endIndex<localNodeports.size())
            {

                MyNodePort * startNode = localNodeports.at(startIndex);
                MyNodePort * endNode = localNodeports.at(endIndex);

                MyArrow *arrow = new MyArrow(startNode, endNode);
                connect(arrow,SIGNAL(editMe()),this,SIGNAL(editCurrItem()));
                connect(arrow,SIGNAL(updateSceneDraw()),this,SLOT(update()));

                startNode->addArrow(arrow);
                endNode->addArrow(arrow);
                arrow->setZValue(-1000.0);
                arrow->setProperty(cutInfo.itemProperty);

                arrow->updatePosition();
                addItem(arrow);
            }
        }
    }
}

///*****************************************************
///**Function:
///**Description:本地打开保存文件后，对解析的内容进行界面显示
///**Input:
///**Output:
///**Return:
///**Others:20160929:wey:调整对直线的解析，将直线放在其它控件之后添加
///****************************************************/
void MyScene::addItem(QList<CutInfo *> &cutInfos)
{
    isLocalFileOpened = true;

    QList<CutInfo *> lines;

    foreach (CutInfo * cutInfo, cutInfos)
    {
        if(cutInfo->graphicsType == GRA_LINE)
        {
            lines.push_back(cutInfo);
        }
        else
        {
            addItem(*cutInfo);
        }
    }

    foreach (CutInfo * cutInfo, lines)
    {
        addItem(*cutInfo);
    }

    isLocalFileOpened = false;
    localItems.clear();
    localNodeports.clear();
}

void MyScene::addItem(GraphicsType type, QPointF pos)
{
    if(type == GRA_TEXT)
    {
        MyTextItem  * item = new MyTextItem(type,rightMenu);
        if(!isLocalFileOpened)
        {
            item->setZValue(Util::getGlobalZValue());
        }
        addMyTextConnect(item);
        item->setPos(pos);
        item->setSelected(true);
        addItem(item);
    }
    else
    {
        MyItem * myItem = new MyItem(type,rightMenu,this);
        if(!isLocalFileOpened)
        {
            myItem->setZValue(Util::getGlobalZValue());
        }
        myItem->setPos(pos);
        addMyItemConnect(myItem);
        addItem(myItem);
    }
}

//建立MyItem的信号槽关系
void MyScene::addMyItemConnect(MyItem * item)
{
    connect(item,SIGNAL(updateSceneDraw()),this,SLOT(update()));
    connect(item,SIGNAL(propHasChanged(ItemProperty)),this,SIGNAL(itemPropChanged(ItemProperty)));
    connect(item,SIGNAL(editMe()),this,SIGNAL(editCurrItem()));
}

//建立MyText的信号槽关系
void MyScene::addMyTextConnect(MyTextItem  * item)
{
    connect(item,SIGNAL(textLostFocus(MyTextItem *)),this,SLOT(respTextLostFocus(MyTextItem *)));
}

//添加箭头时，找出箭头的父节点
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

//添加箭头时，找出箭头的父端口
int MyScene::findItemById(QList<MyNodePort *> &localNode, QString Id)
{
    int index = -1;

    for (int i=0;i<localNode.size();i++)
    {
        if(localNode.at(i)->getNodeProperty().startItemID == Id)
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
