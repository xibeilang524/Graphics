#include "mygraphicsview.h"

#include <QDropEvent>
#include <QMimeData>
#include <QDataStream>
#include <QMenu>
#include <QMouseEvent>
#include <QScrollBar>
#include <QDebug>
#include <QKeyEvent>

#include "myscene.h"
#include "../SelfWidget/nodeeditdialog.h"
#include "../SelfWidget/mytextinput.h"
#include "../Header.h"
#include "../global.h"
#include "../mainwindow.h"
#include "../actionmanager.h"
#include "../Constants.h"
#include "mytextitem.h"
#include "myarrow.h"
#include "myitem.h"
#include "mynodeport.h"

using namespace Graphics;

MyGraphicsView * MyGraphicsView::viewManager = NULL;

MyGraphicsView * MyGraphicsView::instance()
{
   return viewManager;
}

MyGraphicsView::MyGraphicsView(MainWindow * parent):
    parentWindow(parent),
    QGraphicsView(parent)
{
    viewManager = this;

    nodeEdit = NULL;
    isMoving = false;
    isCtrlPressed = false;
    viewIsDragable = true;
    lockState = ITEM_ALL_UNLOCK;

    setAcceptDrops(true);
    initView();
    setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
}

MyScene * MyGraphicsView::scene()
{
    return this->myScene;
}

void MyGraphicsView::initView()
{
    SceneWidth = SceneHeight = 5000;

    rightMenu = new QMenu;
    viewRightMenu = new QMenu;

    myScene = new MyScene(rightMenu);
    myScene->setSceneRect(0,0,SceneWidth,SceneHeight);
    setScene(myScene);

    connect(myScene,SIGNAL(resetItemAction()),parentWindow,SLOT(respRestItemAction()));
    connect(myScene, SIGNAL(selectionChanged()),this, SLOT(updateActions()));
    connect(myScene,SIGNAL(deleteKeyPress()),this,SLOT(deleteItem()));
    connect(myScene,SIGNAL(ctrlLockKeyPress()),this,SLOT(respCtrlLockKeyPress()));
    connect(myScene,SIGNAL(ctrlUnLockKeyPress()),this,SLOT(respCtrlLockKeyPress()));
    connect(myScene,SIGNAL(itemSizeChanged(int)),parentWindow,SLOT(respItemSizeChanged(int)));
    connect(myScene,SIGNAL(itemPropChanged(ItemProperty)),this,SIGNAL(itemPropChanged(ItemProperty)));
}

//初始化右键菜单
void MyGraphicsView::addContextMenuItem()
{
    rightMenu->addAction(ActionManager::instance()->action(Constants::EDIT_TEXT_ID));
    rightMenu->addSeparator();
    rightMenu->addAction(ActionManager::instance()->action(Constants::CUT_ID));
    rightMenu->addAction(ActionManager::instance()->action(Constants::COPY_ID));
    rightMenu->addAction(ActionManager::instance()->action(Constants::PASTE_ID));
    rightMenu->addSeparator();
    rightMenu->addAction(ActionManager::instance()->action(Constants::ROTATE_LEFT_ID));
    rightMenu->addAction(ActionManager::instance()->action(Constants::ROTATE_RIGHT_ID));
    rightMenu->addAction(ActionManager::instance()->action(Constants::BRING_FRONT_ID));
    rightMenu->addAction(ActionManager::instance()->action(Constants::BRING_BACK_ID));
    rightMenu->addSeparator();
    rightMenu->addAction(ActionManager::instance()->action(Constants::LOCK_ID));
    rightMenu->addAction(ActionManager::instance()->action(Constants::UNLOCK_ID));
    rightMenu->addSeparator();
    rightMenu->addAction(ActionManager::instance()->action(Constants::DELETE_ID));

    viewRightMenu->addAction(ActionManager::instance()->action(Constants::UNDO_ID));
    viewRightMenu->addAction(ActionManager::instance()->action(Constants::REDO_ID));
    viewRightMenu->addSeparator();
    viewRightMenu->addAction(ActionManager::instance()->action(Constants::PASTE_ID));
    viewRightMenu->addAction(ActionManager::instance()->action(Constants::CLEAR_PASTE_ID));
    viewRightMenu->addSeparator();
    viewRightMenu->addAction(ActionManager::instance()->action(Constants::DRAG_ABLE_ID));
}

//设置view是否可以拖拽
void MyGraphicsView::setViewDragEnable(bool enable)
{
    viewIsDragable = enable;
    if(enable)
    {
        ActionManager::instance()->action(Constants::DRAG_ABLE_ID)->setIcon(QIcon(":/images/dragable.png"));
        ActionManager::instance()->action(Constants::DRAG_ABLE_ID)->setText("窗口允许拖拽");
    }
    else
    {
        ActionManager::instance()->action(Constants::DRAG_ABLE_ID)->setIcon(QIcon(":/images/drageunable.png"));
        ActionManager::instance()->action(Constants::DRAG_ABLE_ID)->setText("窗口禁止拖拽");
    }
}

void MyGraphicsView::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Control)
    {
        isCtrlPressed = true;
    }
    QGraphicsView::keyPressEvent(event);
}

void MyGraphicsView::keyReleaseEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Control)
    {
        isCtrlPressed = false;
    }
    QGraphicsView::keyReleaseEvent(event);
}

//鼠标滚轮事件，结合键盘Ctrl事件进行视图的缩放
void MyGraphicsView::wheelEvent(QWheelEvent *event)
{
    if(isCtrlPressed)
    {
        int direct = event->delta();
        if(direct > 0)
        {
            emit zoomIn();
        }
        else if(direct < 0)
        {
            emit zoomOut();
        }
    }
    else
    {
        QGraphicsView::wheelEvent(event);
    }
}

void MyGraphicsView::mousePressEvent(QMouseEvent *event)
{
    if(viewIsDragable)
    {
        pressPoint = event->pos();

        isMoving = true;

        setCursor(Qt::ClosedHandCursor);
    }

    QGraphicsView::mousePressEvent(event);
}

void MyGraphicsView::mouseMoveEvent(QMouseEvent *event)
{
    if(viewIsDragable && scene()->selectedItems().size() == 0 && isMoving &&CurrAddGraType == GRA_NONE)
    {
        movePoint = event->pos();

        QPoint tmpPoint = movePoint - pressPoint;

        horizontalScrollBar()->setValue(horizontalScrollBar()->value()-tmpPoint.x());
        verticalScrollBar()->setValue(verticalScrollBar()->value()-tmpPoint.y());

        pressPoint = movePoint;
    }

    QGraphicsView::mouseMoveEvent(event);
}

void MyGraphicsView::mouseReleaseEvent(QMouseEvent *event)
{
    isMoving = false;
    setCursor(Qt::ArrowCursor);
    QGraphicsView::mouseReleaseEvent(event);
}

//右键菜单事件【先判断scene当前鼠标点下是否存在item，如果没有view再响应】
void MyGraphicsView::contextMenuEvent(QContextMenuEvent *event)
{
    QPointF scenePos = mapToScene(event->pos().x(),event->pos().y());
    int curItemSize = myScene->items(scenePos).size();

    QGraphicsView::contextMenuEvent(event);

    if(curItemSize == 0)
    {
        viewRightMenu->exec(mapToGlobal(event->pos()));
    }
}

//左侧控件拖入，对拖入的类型进行判断
void MyGraphicsView::dragEnterEvent(QDragEnterEvent *event)
{
    if(event->mimeData()->hasFormat("MyItem"))
    {
        QByteArray array = event->mimeData()->data("MyItem");
        QDataStream stream(&array, QIODevice::ReadOnly);//流，读数据

        int graphicsType;
        stream>>graphicsType;
        if(graphicsType == GRA_LINE || graphicsType == GRA_VECTOR_LINE || graphicsType == GRA_NODE_PORT)
        {
            //此处如果不希望GraphicsView处理事件，那么需要将此事件交由系统处理，不能直接忽略【1】
            QGraphicsView::dragEnterEvent(event);
        }
        else
        {
           event->acceptProposedAction();
        }
    }
    else
    {
        event->ignore();
    }
}

//在执行拖入移动时，先将事件交由父类处理，这样时间可以被其它控件捕获
void MyGraphicsView::dragMoveEvent(QDragMoveEvent *event)
{
    QGraphicsView::dragMoveEvent(event);
    if(event->mimeData()->hasFormat("MyItem"))
    {
        event->acceptProposedAction();
    }
}

void MyGraphicsView::dropEvent(QDropEvent *event)
{
    if(event->mimeData()->hasFormat("MyItem"))
    {
        QByteArray array = event->mimeData()->data("MyItem");
        QDataStream stream(&array, QIODevice::ReadOnly);//流，读数据

        int graphicsType;
        stream>>graphicsType;

        if(graphicsType == GRA_LINE || graphicsType == GRA_VECTOR_LINE || graphicsType == GRA_NODE_PORT)
        {
            QGraphicsView::dropEvent(event);
        }
        else
        {
            myScene->clearSelection();
            myScene->addItem((GraphicsType)graphicsType,mapToScene(event->pos()));
        }
    }
}

//显示端口编辑窗口
void MyGraphicsView::showNodePortEdit(MyNodePort *nodePort)
{
    if(!nodeEdit)
    {
        nodeEdit = new NodeEditDialog(this);
    }
    nodeEdit->initNodeInfo(nodePort);
    nodeEdit->exec();
}

//撤销、前进
void MyGraphicsView::undoAndRedoItem()
{

}

//目前支持一个控件的剪切，只支持除箭头以外控件操作
void MyGraphicsView::cutItem()
{
    QList<QGraphicsItem *> selectedItems = myScene->selectedItems();

    if(selectedItems.size() == 1)
    {
        QString itemName = TYPE_ID(*(selectedItems.first()));

        if(itemName == TYPE_ID(MyItem))
        {
            MyItem * item = dynamic_cast<MyItem *>(myScene->selectedItems().first());

            cutTmpInfo.graphicsType = item->getType();
            cutTmpInfo.itemProperty = item->getProperty();
            cutTmpInfo.content = item->getText();
            foreach (MyNodePort * node, item->getNodePorts())
            {
                NodePortProperty  props;
                props.direct = node->getDragDirect();
                props.scaleFactor = node->getScaleFactor();
                cutTmpInfo.nodeProperties.push_back(props);
            }
            deleteItem();
            ActionManager::instance()->action(Constants::PASTE_ID)->setEnabled(true);
            ActionManager::instance()->action(Constants::CLEAR_PASTE_ID)->setEnabled(true);
        }
        else if(itemName == TYPE_ID(MyTextItem))
        {
            MyTextItem * item = dynamic_cast<MyTextItem*>(selectedItems.first());

            cutTmpInfo.graphicsType = item->getType();
            cutTmpInfo.itemProperty = item->getProperty();
            delete item;
            ActionManager::instance()->action(Constants::PASTE_ID)->setEnabled(true);
            ActionManager::instance()->action(Constants::CLEAR_PASTE_ID)->setEnabled(true);
        }
    }
}

//复制Item，支持对其样式的、端口的复制
void MyGraphicsView::copyItem()
{
    QList<QGraphicsItem *> selectedItems = myScene->selectedItems();

    if(selectedItems.size() == 1)
    {
        QString itemName = TYPE_ID(*(selectedItems.first()));

        if(itemName == TYPE_ID(MyItem))
        {
            MyItem * item = dynamic_cast<MyItem *>(myScene->selectedItems().first());

            cutTmpInfo.graphicsType = item->getType();
            cutTmpInfo.itemProperty = item->getProperty();
            cutTmpInfo.content = item->getText();
            cutTmpInfo.nodeProperties.clear();
            foreach (MyNodePort * node, item->getNodePorts())
            {
                NodePortProperty  props;
                props.direct = node->getDragDirect();
                props.scaleFactor = node->getScaleFactor();
                cutTmpInfo.nodeProperties.push_back(props);
            }
            ActionManager::instance()->action(Constants::PASTE_ID)->setEnabled(true);
            ActionManager::instance()->action(Constants::CLEAR_PASTE_ID)->setEnabled(true);
        }
        else if(itemName == TYPE_ID(MyTextItem))
        {
            MyTextItem * item = dynamic_cast<MyTextItem*>(selectedItems.first());

            cutTmpInfo.graphicsType = item->getType();
            cutTmpInfo.itemProperty = item->getProperty();
            cutTmpInfo.content = item->toPlainText();
            cutTmpInfo.nodeProperties.clear();
            ActionManager::instance()->action(Constants::PASTE_ID)->setEnabled(true);
            ActionManager::instance()->action(Constants::CLEAR_PASTE_ID)->setEnabled(true);
        }
    }
}

//黏贴
void MyGraphicsView::pasteItem()
{
    myScene->addItem(cutTmpInfo,true);
}

//清空剪切板
void MyGraphicsView::clearPasteItem()
{
    ActionManager::instance()->action(Constants::PASTE_ID)->setEnabled(false);
    ActionManager::instance()->action(Constants::CLEAR_PASTE_ID)->setEnabled(false);
}

//还需要从scene中删除item
void MyGraphicsView::deleteItem()
{
    QList<QGraphicsItem *> selectedItems = myScene->selectedItems();

    foreach(QGraphicsItem * item, selectedItems)
    {
        QString itemName = TYPE_ID(*item);
        if(itemName == TYPE_ID(MyArrow))
        {
            MyArrow * tmp = dynamic_cast<MyArrow *>(item);

            if(tmp->getLineType() == LINE_MYITEM)
            {
                tmp->getStartItem()->removeArrow(tmp);
                tmp->getEndItem()->removeArrow(tmp);
            }
            else if(tmp->getLineType() == LINE_NODEPORT)
            {
                tmp->getStartNodePort()->removeArrow(tmp);
                tmp->getEndNodePort()->removeArrow(tmp);
            }
            myScene->removeItem(tmp);

            delete tmp;
        }
    }

    selectedItems = myScene->selectedItems();

    foreach (QGraphicsItem * item, selectedItems)
    {
        QString itemName = TYPE_ID(*item);
        if(itemName == TYPE_ID(MyItem))
        {
            MyItem * tmp = dynamic_cast<MyItem *>(item);
            tmp->removeArrows();
            myScene->removeItem(tmp);
            delete tmp;
        }
    }

    selectedItems = myScene->selectedItems();

    foreach (QGraphicsItem * item, selectedItems)
    {
        QString itemName = TYPE_ID(*item);
        if(itemName == TYPE_ID(MyTextItem))
        {
            MyTextItem * tmp = dynamic_cast<MyTextItem *>(item);
            myScene->removeItem(tmp);
            delete tmp;
        }
    }

    selectedItems = myScene->selectedItems();

    foreach (QGraphicsItem * item, selectedItems)
    {
        QString itemName = TYPE_ID(*item);
        if(itemName == TYPE_ID(MyNodePort))
        {
            MyNodePort * tmp = dynamic_cast<MyNodePort *>(item);
            tmp->removeArrows();
            //从父类集合中删除
            tmp->getParentItem()->removeNodePort(tmp);
            myScene->removeItem(tmp);
            delete tmp;
        }
    }

    myScene->update();
}

//左、右旋转
void MyGraphicsView::rotateItem()
{
    QList<QGraphicsItem *> selectedItems = myScene->selectedItems();

    if(selectedItems.size() !=  1)
    {
        return;
    }

    QString itemName = TYPE_ID(*(myScene->selectedItems().first()));

    if(itemName == TYPE_ID(MyItem))
    {
        MyItem * graphicsTmp = dynamic_cast<MyItem *>(myScene->selectedItems().first());

        QString objName = QObject::sender()->objectName();
        if(objName == QString(Constants::ROTATE_LEFT_ID))
        {
            graphicsTmp->updateRotation(-90);
        }
        else if(objName == QString(Constants::ROTATE_RIGHT_ID))
        {
            graphicsTmp->updateRotation(90);
        }
        graphicsTmp->setRotation(graphicsTmp->getProperty().rotateDegree);
    }
    else
    {

    }
}

//置顶或置于底层
void MyGraphicsView::bringZItem()
{
    if (myScene->selectedItems().isEmpty())
    {
         return;
    }

    QGraphicsItem *selectedItem = myScene->selectedItems().first();
//    QList<QGraphicsItem *> overlapItems = selectedItem->collidingItems();     //找到与当前item有重合的items
    QList<QGraphicsItem * > allItems = myScene->items();

    bool isFront = false;
    QString objName = QObject::sender()->objectName();
    if(objName == QString(Constants::BRING_FRONT_ID))
    {
        isFront = true;
    }

    qreal zValue = 0;
    foreach (QGraphicsItem *item, allItems)
    {
         if(isFront)
         {
             if (item->zValue() >= zValue)
             {
                 zValue = item->zValue() + 0.1;
             }
         }
         else
         {
             if (item->zValue() <= zValue)
             {
                 zValue = item->zValue() - 0.1;
             }
         }
     }
    QString itemName = TYPE_ID(*selectedItem);
    if(itemName == TYPE_ID(MyItem))
    {
        MyItem * tmp = dynamic_cast<MyItem *>(selectedItem);
        tmp->setZValue(zValue);
    }
    else if(itemName == TYPE_ID(MyTextItem))
    {
        MyTextItem * tmp = dynamic_cast<MyTextItem *>(selectedItem);
        tmp->setZValue(zValue);
    }
}

//键盘快捷键Ctrl+L/U(快速锁定/解锁)
void MyGraphicsView::respCtrlLockKeyPress()
{
    QString objName = QObject::sender()->objectName();

    getSelectedLockState();

    if(objName == QString(Constants::LOCK_ID))
    {
        if(lockState == ITEM_NOT_ALL_LOCK || lockState == ITEM_ALL_UNLOCK)
        {
            ActionManager::instance()->action(Constants::LOCK_ID)->setEnabled(false);
            ActionManager::instance()->action(Constants::UNLOCK_ID)->setEnabled(true);
            setSelectedItemLockState(true);
        }
    }
    else if(objName == QString(Constants::UNLOCK_ID))
    {
        if(lockState == ITEM_ALL_LOCK)
        {
            ActionManager::instance()->action(Constants::LOCK_ID)->setEnabled(true);
            ActionManager::instance()->action(Constants::UNLOCK_ID)->setEnabled(false);
            setSelectedItemLockState(false);
        }
    }
}

//锁定与解锁
void MyGraphicsView::lockAndunlockItem()
{
    QString objName = QObject::sender()->objectName();

    bool moveable = false;

    if(objName == QString(Constants::LOCK_ID))
    {
        moveable = false;
    }
    else if(objName == QString(Constants::UNLOCK_ID))
    {
        moveable = true;
    }

    ActionManager::instance()->action(Constants::LOCK_ID)->setEnabled(moveable);
    ActionManager::instance()->action(Constants::UNLOCK_ID)->setEnabled(!moveable);

    setSelectedItemLockState(moveable);
}

//设置选中控件锁定状态
void MyGraphicsView::setSelectedItemLockState(bool flag)
{
    QList<QGraphicsItem *> selectedItems = myScene->selectedItems();

    if(selectedItems.size() > 0)
    {
        foreach (QGraphicsItem * item, selectedItems)
        {
            QString itemName = TYPE_ID(*item);
            if(itemName == TYPE_ID(MyItem))
            {
                MyItem * tmp = dynamic_cast<MyItem*>(item);
                tmp->setMoveable(flag);
            }
            else if(itemName == TYPE_ID(MyTextItem))
            {
                MyTextItem * tmp = dynamic_cast<MyTextItem*>(item);
                tmp->setMoveable(flag);
            }
        }
        myScene->update();
    }
}

//更新item的属性
void MyGraphicsView::respPropertyUpdate(ItemProperty property)
{
    if(myScene->selectedItems().size() == 1)
    {
        QString itemName = TYPE_ID(*(myScene->selectedItems().first()));

        if(itemName == TYPE_ID(MyItem))
        {
            MyItem * myItem = dynamic_cast<MyItem *>(myScene->selectedItems().first());
            if(property.isMoveable)
            {
                myItem->setProperty(property);
            }
        }
        else if(itemName == TYPE_ID(MyTextItem))
        {
            MyTextItem * textItem = dynamic_cast<MyTextItem *>(myScene->selectedItems().first());
            textItem->setProperty(property);
        }
        else if(itemName == TYPE_ID(MyArrow))
        {
            MyArrow  * arrowItem = dynamic_cast<MyArrow *>(myScene->selectedItems().first());
            arrowItem->setProperty(property);
        }

        myScene->update();
    }
}

//切换视图缩放
void MyGraphicsView::sceneScaled(int currScale)
{
     double newScale = currScale/100.0;
     QMatrix oldMatrix = matrix();
     resetMatrix();
     translate(oldMatrix.dx(), oldMatrix.dy());
     scale(newScale, newScale);
}

//当选择的item状态改变后，更新action
void MyGraphicsView::updateActions()
{
    int selectedSize = myScene->selectedItems().size();

    ItemProperty  property;

    if(selectedSize == 0)
    {
        ActionManager::instance()->action(Constants::EDIT_TEXT_ID)->setEnabled(false);
        ActionManager::instance()->action(Constants::CUT_ID)->setEnabled(false);
        ActionManager::instance()->action(Constants::COPY_ID)->setEnabled(false);

        ActionManager::instance()->action(Constants::ROTATE_LEFT_ID)->setEnabled(false);
        ActionManager::instance()->action(Constants::ROTATE_RIGHT_ID)->setEnabled(false);
        ActionManager::instance()->action(Constants::BRING_FRONT_ID)->setEnabled(false);
        ActionManager::instance()->action(Constants::BRING_BACK_ID)->setEnabled(false);
        ActionManager::instance()->action(Constants::LOCK_ID)->setEnabled(false);
        ActionManager::instance()->action(Constants::UNLOCK_ID)->setEnabled(false);
        ActionManager::instance()->action(Constants::DELETE_ID)->setEnabled(false);
    }
    else if(selectedSize == 1)
    {
        ActionManager::instance()->action(Constants::EDIT_TEXT_ID)->setEnabled(true);
        ActionManager::instance()->action(Constants::CUT_ID)->setEnabled(true);
        ActionManager::instance()->action(Constants::COPY_ID)->setEnabled(true);

        ActionManager::instance()->action(Constants::ROTATE_LEFT_ID)->setEnabled(true);
        ActionManager::instance()->action(Constants::ROTATE_RIGHT_ID)->setEnabled(true);
        ActionManager::instance()->action(Constants::BRING_FRONT_ID)->setEnabled(true);
        ActionManager::instance()->action(Constants::BRING_BACK_ID)->setEnabled(true);
        ActionManager::instance()->action(Constants::LOCK_ID)->setEnabled(true);
        ActionManager::instance()->action(Constants::UNLOCK_ID)->setEnabled(true);
        ActionManager::instance()->action(Constants::DELETE_ID)->setEnabled(true);

        QString itemName = TYPE_ID(*(myScene->selectedItems().first()));

        if(itemName == TYPE_ID(MyItem))
        {
            MyItem * myItem = dynamic_cast<MyItem *>(myScene->selectedItems().first());
            property = myItem->getProperty();

//            bool lock = myItem->isMoveable();
//            ActionManager::instance()->action(Constants::LOCK_ID)->setEnabled(lock);
//            ActionManager::instance()->action(Constants::UNLOCK_ID)->setEnabled(!lock);
            getSelectedLockState();
        }
        else if(itemName == TYPE_ID(MyTextItem))
        {
            MyTextItem * textItem = dynamic_cast<MyTextItem *>(myScene->selectedItems().first());
            property = textItem->getProperty();

            ActionManager::instance()->action(Constants::LOCK_ID)->setEnabled(false);
            ActionManager::instance()->action(Constants::UNLOCK_ID)->setEnabled(false);
        }
        else if(itemName == TYPE_ID(MyArrow))
        {
            MyArrow  * arrowItem = dynamic_cast<MyArrow *>(myScene->selectedItems().first());
            property = arrowItem->getProperty();

            ActionManager::instance()->action(Constants::EDIT_TEXT_ID)->setEnabled(false);
            ActionManager::instance()->action(Constants::CUT_ID)->setEnabled(false);
            ActionManager::instance()->action(Constants::COPY_ID)->setEnabled(false);

            ActionManager::instance()->action(Constants::ROTATE_LEFT_ID)->setEnabled(false);
            ActionManager::instance()->action(Constants::ROTATE_RIGHT_ID)->setEnabled(false);
            ActionManager::instance()->action(Constants::BRING_FRONT_ID)->setEnabled(false);
            ActionManager::instance()->action(Constants::BRING_BACK_ID)->setEnabled(false);
            ActionManager::instance()->action(Constants::LOCK_ID)->setEnabled(false);
            ActionManager::instance()->action(Constants::UNLOCK_ID)->setEnabled(false);
        }
        else if(itemName == TYPE_ID(MyNodePort))
        {
//            MyNodePort  * nodePort = dynamic_cast<MyNodePort *>(myScene->selectedItems().first());
//            property = nodePort->getProperty();
        }
    }
    else
    {
        ActionManager::instance()->action(Constants::EDIT_TEXT_ID)->setEnabled(false);
        ActionManager::instance()->action(Constants::CUT_ID)->setEnabled(false);
        ActionManager::instance()->action(Constants::COPY_ID)->setEnabled(false);

        ActionManager::instance()->action(Constants::ROTATE_LEFT_ID)->setEnabled(false);
        ActionManager::instance()->action(Constants::ROTATE_RIGHT_ID)->setEnabled(false);
        ActionManager::instance()->action(Constants::BRING_FRONT_ID)->setEnabled(false);
        ActionManager::instance()->action(Constants::BRING_BACK_ID)->setEnabled(false);
        ActionManager::instance()->action(Constants::LOCK_ID)->setEnabled(false);
        ActionManager::instance()->action(Constants::UNLOCK_ID)->setEnabled(false);
        ActionManager::instance()->action(Constants::DELETE_ID)->setEnabled(true);

        //获取锁定状态
        getSelectedLockState();
    }

    myScene->update();

    emit initToolBox(selectedSize,property);
}

//获取选择的item锁定的状态
void MyGraphicsView::getSelectedLockState()
{
    int myItemNum = 0;
    int myItemLockNum = 0;
    int myItemUnLockNum = 0;

    foreach (QGraphicsItem * item, myScene->selectedItems())
    {
        QString itemName = TYPE_ID(*item);
        if(itemName == TYPE_ID(MyItem))
        {
            myItemNum ++;

            MyItem * myItem = dynamic_cast<MyItem *>(item);
            if(myItem->isMoveable())
            {
                myItemUnLockNum++;
            }
            else
            {
                myItemLockNum++;
            }
        }
    }

    //全部没锁定
    if(myItemLockNum == myItemNum)
    {
        lockState = ITEM_ALL_UNLOCK;
        ActionManager::instance()->action(Constants::LOCK_ID)->setEnabled(false);
        ActionManager::instance()->action(Constants::UNLOCK_ID)->setEnabled(true);
    }
    //全部锁定
    else if(myItemUnLockNum == myItemNum)
    {
        lockState = ITEM_ALL_LOCK;
        ActionManager::instance()->action(Constants::LOCK_ID)->setEnabled(true);
        ActionManager::instance()->action(Constants::UNLOCK_ID)->setEnabled(false);
    }
    //锁定一部分
    else
    {
        lockState = ITEM_NOT_ALL_LOCK;
        ActionManager::instance()->action(Constants::LOCK_ID)->setEnabled(true);
        ActionManager::instance()->action(Constants::UNLOCK_ID)->setEnabled(true);
    }
}

//编辑文本
void MyGraphicsView::editTextItem()
{
    QList<QGraphicsItem *> selectedItems = myScene->selectedItems();
    if(selectedItems.size() == 1)
    {
        QString itemName = TYPE_ID(*(selectedItems.first()));

        if(itemName == TYPE_ID(MyItem))
        {
            MyItem * item = dynamic_cast<MyItem*>(selectedItems.first());

            MyTextInput textInput(this);

            textInput.setTex(item->getText());
            textInput.exec();

            item->setText(textInput.getText());

            //当修改文字后，需要重新将信息发送至右侧的工具栏。
            emit initToolBox(selectedItems.size(),item->getProperty());
        }
    }
}

//清空所有的item
void MyGraphicsView::clearItems()
{
    myScene->clear();
}

MyGraphicsView::~MyGraphicsView()
{
    if(nodeEdit)
    {
        delete nodeEdit;
        nodeEdit = NULL;
    }

    if(myScene)
    {
        delete myScene;
        myScene = NULL;
    }
}
