#include "myscene.h"

#include <QGraphicsSceneMouseEvent>
#include <QTextCursor>
#include <QKeyEvent>
#include <QDebug>

#include <qglobal.h>

#include "global.h"
#include "myitem.h"
#include "myarrow.h"
#include "mytextitem.h"
#include "mypathitem.h"
#include "mynodeport.h"
#include "myiteminfo.h"
#include "draglinepoint.h"
#include "../util.h"

MyScene::MyScene(QMenu *menu, QObject * parent):
    rightMenu(menu),
    QGraphicsScene(parent)
{
    CurrAddGraType =  GRA_NONE;
    insertTmpLine = NULL;
    insertTmpPath = NULL;
    isLocalFileOpened = false;
    isDragLine = false;
    isClear = false;
    myItemInfo = NULL;
    createItemInfo();

    connect(this,SIGNAL(selectionChanged()),this,SLOT(itemSelectedChanged()));

    setBackgroundBrush(QPixmap(":/images/backgroundRole.png"));
}

void MyScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    SceneLastClickPoint = event->scenePos();

    if(event->button () == Qt::LeftButton)
    {
        //从控件四边中点拖拽产生直线
        if(itemAt(event->scenePos()) && TYPE_ID(*itemAt(event->scenePos())) == TYPE_ID(DragLinePoint))
        {
            DragLinePoint * tmpDrag = dynamic_cast<DragLinePoint *>(itemAt(event->scenePos()));
            QGraphicsItem * pItem = tmpDrag->getParentItem();
            startMouseItemId = dynamic_cast<MyItem *>(pItem)->getProperty().startItemID;

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
//            insertTmpPath->setPos(event->scenePos());
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
    //从线段拖拽点中产生的线段，在产生鼠标移动事件时因鼠标一直按下，无法主动的对其它控件产生鼠标进入事件，因此需要对事件进行分发
    if((isDragLine||CurrAddGraType == GRA_LINE)&& insertTmpLine)
    {
        QLineF newLine(insertTmpLine->line().p1(), event->scenePos());
        insertTmpLine->setLine(newLine);

        QList<QGraphicsItem *> currPosItems = items(event->scenePos());
        foreach (QGraphicsItem * item, currPosItems)
        {
            if(TYPE_ID(*item) == TYPE_ID(MyItem))
            {
                mouseItems.push_back(dynamic_cast<MyItem *>(item));
                //主动对鼠标点位置下Item产生进入事件
                QEvent * eve = new QEvent(QEvent::GraphicsSceneHoverEnter);
                sendEvent(item,eve);
            }
        }

        if(currPosItems.size() == 1)
        {
            resetItemSelection();
        }
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
    if(isDragLine)
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

            DragLinePoint *startItem = qgraphicsitem_cast<DragLinePoint *>(startItems.first());
            DragLinePoint *endItem = qgraphicsitem_cast<DragLinePoint *>(endItems.first());

            if(firstItemId == TYPE_ID(DragLinePoint) && secondItemId == TYPE_ID(DragLinePoint) &&
                    startItem && endItem)
            {
                MyArrow * arrow = createArrow(LINE_MYITEM,startItem,endItem);
                arrow->setStartPointType(startItem->getPointType());
                arrow->setEndPointType(endItem->getPointType());
            }
        }
    }
    else if(!isDragLine && CurrAddGraType == GRA_LINE && insertTmpLine)
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

            if(firstItemId == TYPE_ID(MyNodePort) && secondItemId == TYPE_ID(MyNodePort))
            {
                MyNodePort *startItem = qgraphicsitem_cast<MyNodePort *>(startItems.first());
                MyNodePort *endItem = qgraphicsitem_cast<MyNodePort *>(endItems.first());

                createArrow(LINE_NODEPORT,startItem,endItem);
            }
        }
    }
    else if(CurrAddGraType == GRA_VECTOR_LINE && insertTmpPath)
    {

    }

    isDragLine = false;
    insertTmpLine = NULL;
    insertTmpPath = NULL;
    QGraphicsScene::mouseReleaseEvent(event);
}

void MyScene::keyPressEvent(QKeyEvent *event)
{
    if(GlobalWindowState == WINDOW_BUILD_MODEL)
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
        else if(event->modifiers() == Qt::ControlModifier && event->key() == Qt::Key_E)
        {
            emit ctrlPropEditKeyPress();
        }
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
                DragLinePoint * startItem = localItems.at(startIndex)->getDragLinePoint(cutInfo.itemProperty.startPointType);
                DragLinePoint * endItem = localItems.at(endIndex)->getDragLinePoint(cutInfo.itemProperty.endPointType);

                MyArrow * arrow = createArrow(LINE_MYITEM,startItem,endItem);
                arrow->setProperty(cutInfo.itemProperty);
            }
        }
        else if(cutInfo.itemProperty.lineType == LINE_NODEPORT)
        {
            int startIndex = findItemById(localNodeports,cutInfo.itemProperty.startItemID);
            int endIndex = findItemById(localNodeports,cutInfo.itemProperty.endItemID);

            if(startIndex>=0&&startIndex<localNodeports.size() &&
                    endIndex>=0&&endIndex<localNodeports.size())
            {
                MyNodePort * startItem = localNodeports.at(startIndex);
                MyNodePort * endItem = localNodeports.at(endIndex);

                MyArrow * arrow = createArrow(LINE_NODEPORT,startItem,endItem);
                arrow->setProperty(cutInfo.itemProperty);
            }
        }
    }
}

///*****************************************************
///**Function:
///**Description:统一创建连接线
///**Input:
///**Output:
///**Return:
///**Others:
///****************************************************/
MyArrow * MyScene::createArrow(LineType type, MyNodeLine *startNode, MyNodeLine *endNode)
{
    MyArrow *arrow = new MyArrow;
    connect(arrow,SIGNAL(editMe()),this,SIGNAL(editCurrItem()));
    connect(arrow,SIGNAL(updateSceneDraw()),this,SLOT(update()));

    startNode->addArrow(arrow);
    endNode->addArrow(arrow);

    arrow->setStartItem(startNode);
    arrow->setEndItem(endNode);

    //对于从拖拽点产生的线条，需要保存其父类的ID和线条在父类的位置
    if( type == LINE_MYITEM)
    {
        MyItem * pStart = dynamic_cast<MyItem *>(startNode->getParentItem());
        MyItem * pEnd = dynamic_cast<MyItem *>(endNode->getParentItem());
        arrow->setStartItemID(pStart->getProperty().startItemID);
        arrow->setEndItemID(pEnd->getProperty().startItemID);
    }
    else if( type == LINE_NODEPORT)
    {
        MyNodePort * pStart = dynamic_cast<MyNodePort *>(startNode);
        MyNodePort * pEnd = dynamic_cast<MyNodePort *>(endNode);

        arrow->setStartItemID(pStart->getNodeProperty().startItemID);
        arrow->setEndItemID(pEnd->getNodeProperty().startItemID);
    }

    arrow->setZValue(-1000.0);
    arrow->setLineType(type);

    arrow->updatePosition();
    addItem(arrow);

    return arrow;
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

    foreach (CutInfo * info, cutInfos)
    {
        delete info;
    }

    myItemInfo->setVisible(false);
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
    connect(item,SIGNAL(itemPosChanged(MyItem *)),this,SLOT(showItemPosInfo(MyItem *)));
    connect(item,SIGNAL(itemRotationChanged(MyItem *)),this,SLOT(showItemRotationInfo(MyItem *)));
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

//显示选中item改变的位置信息
void MyScene::showItemPosInfo(MyItem * item)
{
    if(!myItemInfo)
    {
        createItemInfo();
    }
    QRectF minRect = getHorizonalRoundedRect(item);
    myItemInfo->setVisible(true);
    qreal itemWidth = minRect.width();
    qreal itemHeight = minRect.height();

    myItemInfo->setPosInfo(minRect.x(),minRect.y());
    myItemInfo->setPos(minRect.x() + itemWidth/2,minRect.y() + itemHeight + 30);
}

//显示选中item改变的旋转信息
void MyScene::showItemRotationInfo(MyItem *item)
{
    if(!myItemInfo)
    {
        createItemInfo();
    }

    QRectF minRect = getHorizonalRoundedRect(item);
    myItemInfo->setVisible(true);
    qreal itemWidth = minRect.width();
    qreal itemHeight = minRect.height();

    myItemInfo->setRotationInfo(item->rotation());
    myItemInfo->setPos(minRect.x() + itemWidth/2,minRect.y() + itemHeight + 30);
}

//显示旋转和位置信息
void MyScene::createItemInfo()
{
    myItemInfo = new MyItemInfo(GRA_ITEM_INFO);
    myItemInfo->setVisible(false);
    myItemInfo->setZValue(1000);

    addItem(myItemInfo);
}

//获取最小包围控件的矩形，因旋转后boundRect无法表示rect在scene中的位置
QRectF MyScene::getHorizonalRoundedRect(MyItem *item)
{
    QRectF rect;

    if(!item)
    {
        return rect;
    }

    QPointF p1 = item->mapToScene(item->boundingRect().topLeft().x(),item->boundingRect().topLeft().y());
    QPointF p2 = item->mapToScene(item->boundingRect().topRight().x(),item->boundingRect().topRight().y());
    QPointF p3 = item->mapToScene(item->boundingRect().bottomLeft().x(),item->boundingRect().bottomLeft().y());
    QPointF p4 = item->mapToScene(item->boundingRect().bottomRight().x(),item->boundingRect().bottomRight().y());

    qreal minX = p1.x();
    minX = qMin(minX,p2.x());
    minX = qMin(minX,p3.x());
    minX = qMin(minX,p4.x());

    qreal minY = p1.y();
    minY = qMin(minY,p2.y());
    minY = qMin(minY,p3.y());
    minY = qMin(minY,p4.y());

    qreal maxX = p1.x();
    maxX = qMax(maxX,p2.x());
    maxX = qMax(maxX,p3.x());
    maxX = qMax(maxX,p4.x());

    qreal maxY = p1.y();
    maxY = qMax(maxY,p2.y());
    maxY = qMax(maxY,p3.y());
    maxY = qMax(maxY,p4.y());

    rect.setX(minX);
    rect.setY(minY);
    rect.setWidth(maxX - minX);
    rect.setHeight(maxY - minY);
    return rect;
}

//item选择状态改变【清空时因此控件是一部分被清空，导致无法访问】
void MyScene::itemSelectedChanged()
{
    if( !isClear && myItemInfo)
    {
        myItemInfo->setVisible(false);
    }
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

//产生控件连接线时，如果当前鼠标位置下没有控件，则清空先前选中的item
void MyScene::resetItemSelection()
{
    foreach(MyItem * item,mouseItems)
    {
        if(item && item->getProperty().startItemID != startMouseItemId)
        {
            item->setDragLineVisible(false);
        }
    }
    mouseItems.clear();
    update();
}

//清空item，但不清除第一个Item(MyItemInfo)【!!!】
void MyScene::clear()
{
    isClear = true;
    QGraphicsScene::clear();
    isClear = false;

    myItemInfo = NULL;
}

MyScene::~MyScene()
{

}
