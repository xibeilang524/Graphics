#include "mygraphicsview.h"

#include <QDropEvent>
#include <QMimeData>
#include <QFileDialog>
#include <QDataStream>
#include <QMenu>
#include <QMouseEvent>
#include <QScrollBar>
#include <QDebug>
#include <QKeyEvent>
#include <QApplication>
#include <QUrl>

#include "myscene.h"
#include "../SelfWidget/nodeeditdialog.h"
#include "../SelfWidget/mytextinput.h"
#include "../SelfWidget/mypropertyedit.h"
#include "../SelfWidget/myconditionsetting.h"
#include "../SelfWidget/myportinitialdialog.h"
#include "../SelfWidget/myportoutputdialog.h"
#include "../SelfWidget/mystateinternaleventdialog.h"
#include "../SelfWidget/mystatemodeldialog.h"
#include "../SelfWidget/mystatesetdialog.h"
#include "../SelfWidget/mystatestartdialog.h"
#include "../SelfWidget/mypageswitch.h"
#include "../SelfWidget/myloopdialog.h"
#include "../SelfWidget/myoutputdialog.h"
#include "../simulate/simulateutil.h"
#include "../Header.h"
#include "../global.h"
#include "../mainwindow.h"
#include "../manager/actionmanager.h"
#include "../manager/mylinecombobox.h"
#include "../manager/menumanager.h"
#include "../Constants.h"
#include "../fileoperate.h"
#include "../util.h"
#include "mytextitem.h"
#include "myarrow.h"
#include "myitem.h"
#include "mynodeport.h"
#include "rotateline.h"
#include "draglinepoint.h"
#include "mypathitem.h"

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
    sceneScaleFactor = 100;

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
    MenuManager::instance()->createMenu(Constants::MENU_ITEM_RIGHT_MENU);
    MenuManager::instance()->createMenu(Constants::MENU_GRAPHICS_VIEW);
}

//创建新的scene
MyScene * MyGraphicsView::addScene(QString id)
{
    MyScene  * tmpScene = new MyScene();
    tmpScene->setSceneRect(0,0,SceneWidth,SceneHeight);
    tmpScene->setId(id);

    showScene(tmpScene);

    connect(tmpScene,SIGNAL(resetItemAction()),parentWindow,SLOT(respRestItemAction()));
    connect(tmpScene, SIGNAL(selectionChanged()),this, SLOT(updateActions()));
    connect(tmpScene,SIGNAL(deleteKeyPress()),this,SLOT(deleteItem()));
    connect(tmpScene,SIGNAL(ctrlLockKeyPress()),this,SLOT(respCtrlLockKeyPress()));
    connect(tmpScene,SIGNAL(ctrlUnLockKeyPress()),this,SLOT(respCtrlLockKeyPress()));
    connect(tmpScene,SIGNAL(itemSizeChanged(int)),this,SLOT(respItemSizeChanged(int)));
    connect(tmpScene,SIGNAL(itemPropChanged(ItemProperty)),this,SIGNAL(itemPropChanged(ItemProperty)));
    connect(tmpScene,SIGNAL(editCurrItem()),this,SLOT(editTextItem()));
    connect(tmpScene,SIGNAL(editCurrPort()),this,SLOT(respEditText()));
    connect(tmpScene,SIGNAL(ctrlPropEditKeyPress()),this,SLOT(editPropertyItem()));

    return tmpScene;
}

//设置当前的场景
void MyGraphicsView::showScene(MyScene *scene)
{
    myScene = scene;
    MyGraphicsView::setScene(myScene);
}

//初始化右键菜单
void MyGraphicsView::addContextMenuItem()
{
    QMenu * rightMenu = MenuManager::instance()->menu(Constants::MENU_ITEM_RIGHT_MENU);

    rightMenu->clear();
    if(GlobalWindowState == WINDOW_BUILD_MODEL)
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
    }
    else if(GlobalWindowState == WINDOW_SIMULATE)
    {
        rightMenu->addAction(ActionManager::instance()->action(Constants::PROPERTY_EDIT_ID));
    }
}

//设置视图的右键菜单
void MyGraphicsView::addViewContextMenu()
{
    QMenu * viewRightMenu = MenuManager::instance()->menu(Constants::MENU_GRAPHICS_VIEW);

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
    else if(event->modifiers() == Qt::AltModifier && (event->key() == Qt::Key_Left || event->key() == Qt::Key_Right))
    {
        event->ignore();
        return;
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
    MY_ASSERT(myScene)
    if(viewIsDragable && myScene->selectedItems().size()==0 )
    {
        pressPoint = event->pos();

        isMoving = true;
        setCursor(Qt::ClosedHandCursor);
    }

    QGraphicsView::mousePressEvent(event);
}

void MyGraphicsView::mouseMoveEvent(QMouseEvent *event)
{
    MY_ASSERT(myScene)
    if(viewIsDragable && myScene->selectedItems().size() == 0 && isMoving &&CurrAddGraType == GRA_NONE)
    {
        movePoint = event->pos();

        QPoint tmpPoint = movePoint - pressPoint;

        horizontalScrollBar()->setValue(horizontalScrollBar()->value()-tmpPoint.x());
        verticalScrollBar()->setValue(verticalScrollBar()->value()-tmpPoint.y());

        pressPoint = movePoint;
    }

    QGraphicsView::mouseMoveEvent(event);
}

//先交由系统处理，在控件上鼠标释放时，控件先捕获。
void MyGraphicsView::mouseReleaseEvent(QMouseEvent *event)
{
    QGraphicsView::mouseReleaseEvent(event);
    isMoving = false;
    setCursor(Qt::ArrowCursor);
}

//右键菜单事件【先判断scene当前鼠标点下是否存在item，如果没有view再响应】
void MyGraphicsView::contextMenuEvent(QContextMenuEvent *event)
{
    MY_ASSERT(myScene)
    QPointF scenePos = mapToScene(event->pos().x(),event->pos().y());
    int curItemSize = myScene->items(scenePos).size();

    QGraphicsView::contextMenuEvent(event);

    if(curItemSize == 0)
    {
        MenuManager::instance()->menu(Constants::MENU_GRAPHICS_VIEW)->exec(mapToGlobal(event->pos()));
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
    else if(event->mimeData()->hasUrls())
    {
        event->acceptProposedAction();
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
    else if(event->mimeData()->hasUrls())
    {
        event->acceptProposedAction();
    }
}

/*!
*【拖入事件】：
*【1】View用于接收大控件
*【2】端口控件，view不处理，将事件抛出交由每个控件处理drop
*【3】20161201增加拖入本地文件打开
*
*/
void MyGraphicsView::dropEvent(QDropEvent *event)
{
    MY_ASSERT(myScene)
    if(event->mimeData()->hasFormat("MyItem"))
    {
        QByteArray array = event->mimeData()->data("MyItem");
        QDataStream stream(&array, QIODevice::ReadOnly);//流，读数据

        int graphicsType;
        stream>>graphicsType;

        if(graphicsType == GRA_LINE || graphicsType == GRA_VECTOR_LINE || graphicsType == GRA_NODE_PORT ||
                graphicsType == GRA_NODE_HALF_CIRCLE || graphicsType == GRA_NODE_TRIANGLE_IN ||graphicsType == GRA_NODE_TRIANGLE_OUT
                ||graphicsType == GRA_NODE_CIRCLE)
        {
            QGraphicsView::dropEvent(event);
        }
        else
        {
            myScene->clearSelection();
            myScene->addItem((GraphicsType)graphicsType,mapToScene(event->pos()));
        }
    }
    else if(event->mimeData()->hasUrls())
    {
        QList<QString> acceptUrls;
        foreach(QUrl url,event->mimeData()->urls())
        {
            QFileInfo info(url.toLocalFile());
            if(info.isFile())
            {
                if(info.suffix() == SaveFileSuffix.right(SaveFileSuffix.size() - 1))
                {
                    QFile file(url.toLocalFile());
                    if(file.open(QFile::ReadWrite))
                    {
                        QDataStream stream;
                        stream.setDevice(&file);

                        ReturnType  rtype = Util::isIllegalFile(stream);

                        if( rtype == RETURN_SUCCESS)
                        {
                            acceptUrls.push_back(url.toLocalFile());
                        }
                    }
                }
            }
        }

        if(acceptUrls.size() > 0)
        {
            foreach(QString fileName,acceptUrls)
            {
                if(!MyPageSwitch::instance()->hasContainFile(fileName))
                {
                    MyPageSwitch::instance()->addPage();
                }
                openLocalFile(fileName);
            }
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
    MY_ASSERT(myScene)
    QList<QGraphicsItem *> selectedItems = myScene->selectedItems();

    if(selectedItems.size() == 1)
    {
        QString itemName = TYPE_ID(*(selectedItems.first()));

        if(itemName == TYPE_ID(MyItem))
        {
            MyItem * item = dynamic_cast<MyItem *>(myScene->selectedItems().first());

            cutTmpInfo.hasContent = true;
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

            cutTmpInfo.hasContent = true;
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
    MY_ASSERT(myScene)
    QList<QGraphicsItem *> selectedItems = myScene->selectedItems();

    if(selectedItems.size() == 1)
    {
        QString itemName = TYPE_ID(*(selectedItems.first()));

        if(itemName == TYPE_ID(MyItem))
        {
            MyItem * item = dynamic_cast<MyItem *>(myScene->selectedItems().first());

            cutTmpInfo.hasContent = true;
            cutTmpInfo.graphicsType = item->getType();
            cutTmpInfo.itemProperty = item->getProperty();
            cutTmpInfo.content = item->getText();
            cutTmpInfo.nodeProperties.clear();

            foreach (MyNodePort * node, item->getNodePorts())
            {
                NodePortProperty  props;
                props.content = node->getText();
                props.portType = node->getType();
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

            cutTmpInfo.hasContent = true;
            cutTmpInfo.graphicsType = item->getType();
            cutTmpInfo.itemProperty = item->getProperty();
            cutTmpInfo.content = item->toPlainText();
            cutTmpInfo.nodeProperties.clear();
            ActionManager::instance()->action(Constants::PASTE_ID)->setEnabled(true);
            ActionManager::instance()->action(Constants::CLEAR_PASTE_ID)->setEnabled(true);
        }
    }
}

//粘贴
void MyGraphicsView::pasteItem()
{
    MY_ASSERT(myScene)
    MY_BUILD_MODEL_ONLY
    myScene->addItem(cutTmpInfo,true);
}

//清空剪切板
void MyGraphicsView::clearPasteItem()
{
    cutTmpInfo.hasContent = false;
    ActionManager::instance()->action(Constants::PASTE_ID)->setEnabled(false);
    ActionManager::instance()->action(Constants::CLEAR_PASTE_ID)->setEnabled(false);
}

//还需要从scene中删除item
void MyGraphicsView::deleteItem()
{
    MY_ASSERT(myScene)
    MY_BUILD_MODEL_ONLY

    //删除箭头
    foreach(QGraphicsItem * item, myScene->selectedItems())
    {
        QString itemName = TYPE_ID(*item);
        if(itemName == TYPE_ID(MyArrow))
        {
            MyArrow * tmp = dynamic_cast<MyArrow *>(item);

            tmp->getStartItem()->removeArrow(tmp);
            tmp->getEndItem()->removeArrow(tmp);
            myScene->removeItem(tmp);

            delete tmp;
        }
    }

    //删除折线
    foreach(QGraphicsItem * item, myScene->selectedItems())
    {
        QString itemName = TYPE_ID(*item);
        if(itemName == TYPE_ID(MyPathItem))
        {
            MyPathItem * tmp = dynamic_cast<MyPathItem *>(item);

            tmp->getStartItem()->removePathLine(tmp);
            tmp->getEndItem()->removePathLine(tmp);
            myScene->removeItem(tmp);

            delete tmp;
        }
    }

    //删除item
    foreach (QGraphicsItem * item, myScene->selectedItems())
    {
        QString itemName = TYPE_ID(*item);
        if(itemName == TYPE_ID(MyItem))
        {
            MyItem * tmp = dynamic_cast<MyItem *>(item);
            tmp->removeDragLineArrows();
            myScene->removeItem(tmp);
            delete tmp;
        }
    }

    //删除文本
    foreach (QGraphicsItem * item, myScene->selectedItems())
    {
        QString itemName = TYPE_ID(*item);
        if(itemName == TYPE_ID(MyTextItem))
        {
            MyTextItem * tmp = dynamic_cast<MyTextItem *>(item);
            myScene->removeItem(tmp);
            delete tmp;
        }
    }

    //删除端口
    foreach (QGraphicsItem * item, myScene->selectedItems())
    {
        QString itemName = TYPE_ID(*item);
        if(itemName == TYPE_ID(MyNodePort))
        {
            MyNodePort * tmp = dynamic_cast<MyNodePort *>(item);
            tmp->removeArrows();
            //从父类集合中删除
            dynamic_cast<MyItem *>(tmp->getParentItem())->removeNodePort(tmp);
            myScene->removeItem(tmp);
            delete tmp;
        }
    }

    myScene->update();
}

//左、右旋转
void MyGraphicsView::rotateItem()
{
    MY_ASSERT(myScene)
    MY_BUILD_MODEL_ONLY
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
            graphicsTmp->updateRotation(270);
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
    myScene->update();
}

//置顶或置于底层
void MyGraphicsView::bringZItem()
{
    MY_ASSERT(myScene)
    MY_BUILD_MODEL_ONLY
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
    MY_BUILD_MODEL_ONLY
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
    MY_BUILD_MODEL_ONLY
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
    MY_ASSERT(myScene)
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
    MY_ASSERT(myScene)
    //当选择旋转点后，会同时选中其下面的item
    if(myScene->selectedItems().size() == 1 || myScene->selectedItems().size() ==2)
    {
        QString itemName = TYPE_ID(*(myScene->selectedItems().first()));

        if((itemName == TYPE_ID(MyItem)) || (itemName == TYPE_ID(RotateLine)))
        {
            MyItem * myItem = NULL;
            foreach (QGraphicsItem * selectItem, myScene->selectedItems())
            {
                if(TYPE_ID(MyItem) == TYPE_ID(*selectItem))
                {
                     myItem = dynamic_cast<MyItem *>(selectItem);
                     break;
                }
            }
            if(property.isMoveable)
            {
                MY_ASSERT(myItem);
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
     sceneScaleFactor = currScale;
     QMatrix oldMatrix = matrix();
     resetMatrix();
     translate(oldMatrix.dx(), oldMatrix.dy());
     scale(newScale, newScale);
}

//当选择的item状态改变后，更新action
void MyGraphicsView::updateActions()
{
    MY_ASSERT(myScene)
    int selectedSize = myScene->selectedItems().size();

    ItemProperty  property;

    if(selectedSize == 0)
    {
        ActionManager::instance()->action(Constants::EDIT_TEXT_ID)->setEnabled(false);
        ActionManager::instance()->action(Constants::CUT_ID)->setEnabled(false);
        ActionManager::instance()->action(Constants::COPY_ID)->setEnabled(false);

        if(!cutTmpInfo.hasContent)
        {
            ActionManager::instance()->action(Constants::PASTE_ID)->setEnabled(false);
            ActionManager::instance()->action(Constants::CLEAR_PASTE_ID)->setEnabled(false);
        }

        ActionManager::instance()->action(Constants::ROTATE_LEFT_ID)->setEnabled(false);
        ActionManager::instance()->action(Constants::ROTATE_RIGHT_ID)->setEnabled(false);
        ActionManager::instance()->action(Constants::BRING_FRONT_ID)->setEnabled(false);
        ActionManager::instance()->action(Constants::BRING_BACK_ID)->setEnabled(false);
        ActionManager::instance()->action(Constants::LOCK_ID)->setEnabled(false);
        ActionManager::instance()->action(Constants::UNLOCK_ID)->setEnabled(false);
        ActionManager::instance()->action(Constants::DELETE_ID)->setEnabled(false);
        ComboBoxManager::instance()->item(Constants::LEFT_LINE_ID)->setEnabled(false);
        ComboBoxManager::instance()->item(Constants::RIGHT_LINE_ID)->setEnabled(false);
    }
    else if(selectedSize == 1)
    {
        QString itemName = TYPE_ID(*(myScene->selectedItems().first()));

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
        ComboBoxManager::instance()->item(Constants::LEFT_LINE_ID)->setEnabled(false);
        ComboBoxManager::instance()->item(Constants::RIGHT_LINE_ID)->setEnabled(false);

        if(itemName == TYPE_ID(MyItem))
        {
            MyItem * myItem = dynamic_cast<MyItem *>(myScene->selectedItems().first());
            property = myItem->getProperty();
            getSelectedLockState();
        }
        else if(itemName == TYPE_ID(MyTextItem))
        {
            MyTextItem * textItem = dynamic_cast<MyTextItem *>(myScene->selectedItems().first());
            property = textItem->getProperty();

            ActionManager::instance()->action(Constants::LOCK_ID)->setEnabled(false);
            ActionManager::instance()->action(Constants::UNLOCK_ID)->setEnabled(false);
        }
        else if(itemName == TYPE_ID(MyArrow) || itemName == TYPE_ID(MyPathItem))
        {
            AddLineType startType,endType;
            if(itemName == TYPE_ID(MyArrow))
            {
                MyArrow  * arrowItem = dynamic_cast<MyArrow *>(myScene->selectedItems().first());
                property = arrowItem->getProperty();
                startType = arrowItem->getStartLineType();
                endType = arrowItem->getEndLineType();
            }
            else
            {
                MyPathItem  * pathItem = dynamic_cast<MyPathItem *>(myScene->selectedItems().first());
                property = pathItem->getProperty();
                startType = pathItem->getStartLineType();
                endType = pathItem->getEndLineType();
            }

            ActionManager::instance()->action(Constants::EDIT_TEXT_ID)->setEnabled(true);
            ActionManager::instance()->action(Constants::CUT_ID)->setEnabled(false);
            ActionManager::instance()->action(Constants::COPY_ID)->setEnabled(false);

            ActionManager::instance()->action(Constants::ROTATE_LEFT_ID)->setEnabled(false);
            ActionManager::instance()->action(Constants::ROTATE_RIGHT_ID)->setEnabled(false);
            ActionManager::instance()->action(Constants::BRING_FRONT_ID)->setEnabled(false);
            ActionManager::instance()->action(Constants::BRING_BACK_ID)->setEnabled(false);
            ActionManager::instance()->action(Constants::LOCK_ID)->setEnabled(false);
            ActionManager::instance()->action(Constants::UNLOCK_ID)->setEnabled(false);

            ComboBoxManager::instance()->item(Constants::LEFT_LINE_ID)->updateIndex((int)startType);
            ComboBoxManager::instance()->item(Constants::RIGHT_LINE_ID)->updateIndex((int)endType);

            ComboBoxManager::instance()->item(Constants::LEFT_LINE_ID)->setEnabled(true);
            ComboBoxManager::instance()->item(Constants::RIGHT_LINE_ID)->setEnabled(true);
        }
        else if(itemName == TYPE_ID(MyNodePort) || itemName == TYPE_ID(DragLinePoint))
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
        ComboBoxManager::instance()->item(Constants::LEFT_LINE_ID)->setEnabled(false);
        ComboBoxManager::instance()->item(Constants::RIGHT_LINE_ID)->setEnabled(false);
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
    MY_ASSERT(myScene)
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

//设置选中线条的样式
void MyGraphicsView::setSelectedLineType(int type)
{
    MY_ASSERT(myScene)
    MY_BUILD_MODEL_ONLY
    if(myScene->selectedItems().size() == 1)
    {
        QString itemName = TYPE_ID(*(myScene->selectedItems().first()));
        if(itemName == TYPE_ID(MyArrow))
        {
            MyArrow  * arrowItem = dynamic_cast<MyArrow *>(myScene->selectedItems().first());

            QString objName = QObject::sender()->objectName();
            if(objName == QString(Constants::LEFT_LINE_ID))
            {
                arrowItem->setStartLineType(type);
            }
            else if(objName == QString(Constants::RIGHT_LINE_ID))
            {
                arrowItem->setEndLineType(type);
            }
        }
    }
}

//响应右键菜单编辑
void MyGraphicsView::respEditText()
{
    MY_ASSERT(myScene)
    MY_BUILD_MODEL_ONLY
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
        else if(itemName == TYPE_ID(MyArrow))
        {
            MyArrow * item = dynamic_cast<MyArrow*>(selectedItems.first());
            MyTextInput textInput(this);

            textInput.setTex(item->getText());
            textInput.exec();

            item->setText(textInput.getText());
        }
        else if(itemName == TYPE_ID(MyPathItem))
        {
            MyPathItem * item = dynamic_cast<MyPathItem*>(selectedItems.first());
            MyTextInput textInput(this);

            textInput.setTex(item->getText());
            textInput.exec();

            item->setText(textInput.getText());
        }
        else if(itemName == TYPE_ID(MyNodePort))
        {
            MyNodePort * item = dynamic_cast<MyNodePort*>(selectedItems.first());

            MyTextInput textInput(this);

            textInput.setTex(item->getText());
            textInput.exec();

            item->setText(textInput.getText());
        }
    }
}

//响应控件双击
void MyGraphicsView::editTextItem()
{
    MY_ASSERT(myScene)
    MY_BUILD_MODEL_ONLY
    QList<QGraphicsItem *> selectedItems = myScene->selectedItems();

    if(selectedItems.size() == 1)
    {
        QString itemName = TYPE_ID(*(selectedItems.first()));

        if(itemName == TYPE_ID(MyItem))
        {
            MyItem * item = dynamic_cast<MyItem*>(selectedItems.first());
#ifdef ADD_STATE_MODEL
            GraphicsType gType = item->getType();

            if(gType == GRA_STATE_START)
            {
                MyStateStartDialog dialog(this);
                dialog.setContent(item->getStartProp().content);
                dialog.exec();
                StateStartProperty prop;
                prop.content = dialog.getContent();
                item->setStartProp(prop);
            }
            else if(gType == GRA_STATE_PROCESS)
            {
                MyStateModelDialog dialog(this);
                dialog.setModelProp(item->getModelProp());
                dialog.exec();
                item->setModelProp(dialog.getModelProp());
            }
            else if(gType == GRA_MASK_RECT || gType == GRA_MASK_BOUND_RECT || gType == GRA_MASK_CIRCLE || gType == GRA_NODE_PROCESS)
            {
                QString localAtomID = item->getProperty().associativeID;
                QString currLocalPath = MyPageSwitch::instance()->currPageMapping()->pathName;
                if(currLocalPath.size() == 0)
                {
                    Util::showWarn("此文件未添加至工程中，无法进行托管!");
                    return;
                }
                QString localFileName = currLocalPath+"/"+ localAtomID+ QString("_v"+QString::number(M_VERTION)) +SaveFileSuffix;

                QFile file(localFileName);
                if(!file.open(QFile::ReadWrite))
                {
                    Util::showWarn("原子组件图打开失败!");
                }
                bool isExisted = MyPageSwitch::instance()->openPage(localAtomID);
                MyPageSwitch::instance()->updateCurrMappingInfo(localFileName);

                if(file.size() > 0 && !isExisted)
                {
                    openLocalFile(localFileName,true);
                }
            }
            else
            {
#endif
                MyTextInput textInput(this);

                textInput.setTex(item->getText());
                textInput.exec();

                item->setText(textInput.getText());

                //当修改文字后，需要重新将信息发送至右侧的工具栏。
                emit initToolBox(selectedItems.size(),item->getProperty());
#ifdef ADD_STATE_MODEL
            }
#endif
        }
        else if(itemName == TYPE_ID(MyArrow))
        {
            MyArrow * item = dynamic_cast<MyArrow*>(selectedItems.first());

            MyStateSetDialog dialog(this);
            dialog.exec();

//            MyTextInput textInput(this);

//            textInput.setTex(item->getText());
//            textInput.exec();

//            item->setText(textInput.getText());
        }
        else if(itemName == TYPE_ID(MyNodePort))
        {
            MyNodePort * item = dynamic_cast<MyNodePort*>(selectedItems.first());

            GraphicsType gType = item->getType();

            //输出
            if(gType == GRA_NODE_TRIANGLE_OUT)
            {
                MyPortOutputDialog dialog(this);
                dialog.setInOutState(false);
                StateInOutProperty prop = item->getStateInOutProp();
                dialog.setProp(prop);
                dialog.exec();
                item->setPortInoutProp(dialog.getProp());
            }
            //输入
            else if(gType == GRA_NODE_HALF_CIRCLE)
            {
                MyPortOutputDialog dialog(this);
                dialog.setInOutState(true);
                StateInOutProperty prop = item->getStateInOutProp();
                dialog.setProp(prop);
                dialog.exec();
                item->setPortInoutProp(dialog.getProp());
            }
            //圆形端口
            else if(gType == GRA_NODE_CIRCLE)
            {
                MyPortInitialDialog dialog(this);
                StatePortProperty prop = item->getStatePortProp();
                dialog.setProp(prop);
                dialog.exec();
                item->setPortProp(dialog.getProp());
            }
            else
            {
                MyTextInput textInput(this);

                textInput.setTex(item->getText());
                textInput.exec();

                item->setText(textInput.getText());
            }
        }
    }
}

//编辑控件的属性
void MyGraphicsView::editPropertyItem()
{
    MY_ASSERT(myScene)
    QList<QGraphicsItem *> selectedItems = myScene->selectedItems();
    if(selectedItems.size() == 1)
    {
        QString itemName = TYPE_ID(*(selectedItems.first()));

        if(itemName == TYPE_ID(MyItem))
        {
            MyItem * item = dynamic_cast<MyItem*>(selectedItems.first());
            if(item)
            {
                showSelectedItemPropEdit(item);
            }
        }
    }
}

//右键显示选择控件的属性编辑窗口
void MyGraphicsView::showSelectedItemPropEdit(MyItem * item, QVariant userRoleData, bool isPanelEditable)
{
    GraphicsType type = item->getType();
    if(type == GRA_ROUND_RECT )
    {

    }
    //判断框
    else if(type == GRA_POLYGON)
    {
        ProcessType ptype = item->getProcessType();

        QMessageBox::StandardButton result;

        if(ptype == PRO_NONE)
        {
            result = QMessageBox::information(this,"提示","是否将此控件设置为判断框? \n【Yes】作为判断框\n【No】作为循环框",
                                       QMessageBox::Yes|QMessageBox::No,QMessageBox::Yes);
        }
        else if(ptype == PRO_LOOP)
        {
            result = QMessageBox::No;
        }
        else if(ptype == PRO_JUDGE)
        {
            result = QMessageBox::Yes;
        }

        if(result == QMessageBox::Yes)
        {
            MyConditionSetting dialog(this);
            dialog.setJudgeProp(item,isPanelEditable);
            dialog.exec();
        }
        else
        {
            MyLoopDialog dialog(this);
            dialog.setLoopItemProp(item->getLoopProp(),userRoleData,isPanelEditable);
            dialog.exec();
        }
    }
    else if(type == GRA_PARALLELOGRAM)
    {
        MyOutputDialog dialog(this);
        dialog.setOutput(item,isPanelEditable);
        dialog.exec();
    }
    else
    {
        MyPropertyEdit propertyEdit(this);
        propertyEdit.updateDelegateList(SimulateUtil::instance()->getCurrParentItem(item));
        propertyEdit.initProp(item->getServiceProp(),isPanelEditable);

        propertyEdit.exec();
    }
}

//获取当前模型区域内的item数量
int MyGraphicsView::getItemSize()
{
     if(myScene)
     {
         return myScene->items().size();
     }
     return 0;
}

//清空所有的item
void MyGraphicsView::clearItems()
{
    MY_ASSERT(myScene)
    myScene->clear();
}

//当删除最后一个工作区后，将scene置为空
void MyGraphicsView::deleteScene()
{
    myScene = NULL;
}

//获取水平视图的位置
qreal MyGraphicsView::getHorizonalValue()
{
    return horizontalScrollBar()->value();
}

//获取水平视图的位置
qreal MyGraphicsView::getVertiaclValue()
{
    return verticalScrollBar()->value();
}

//获取水平视图的位置
qreal MyGraphicsView::getScaleValue()
{
    return sceneScaleFactor;
}

//工作区切换，依照不同工作区设置缩放比例
void MyGraphicsView::setScaleValue(int value)
{
    sceneScaled(value);
    emit scaleValue(value);
}

//移动视图
void MyGraphicsView::transformView(qreal hValue, qreal value)
{
    horizontalScrollBar()->setValue(hValue);
    verticalScrollBar()->setValue(value);
}

//模拟开始前，将上次模拟的状态清空
void MyGraphicsView::respResetSimluate()
{
    if(myScene)
    {
        foreach(QGraphicsItem * item,myScene->items())
        {
            if(TYPE_ID(*item) == TYPE_ID(MyItem))
            {
                MyItem * tmp = dynamic_cast<MyItem *>(item);
                if(tmp && tmp->isHightShow())
                {
                    tmp->hightLightItem(LEVEL_NORMAL,false);
                }
            }
        }
    }
}

//获取被选中最上层被选中item的名称
QString MyGraphicsView::getFirstSelectedItem()
{
    if(myScene->selectedItems().size() == 1)
    {
       return TYPE_ID(*(myScene->selectedItems().first()));
    }
    return "";
}

//响应scene中item数量的改变
void MyGraphicsView::respItemSizeChanged(int size)
{
    MY_ASSERT(myScene)
    bool actionEnabled = true;

    //默认每个scene的第一个item为【MyItemInfo】
    if(size <= 1)
    {
        actionEnabled = false;
    }

    ActionManager::instance()->action(Constants::SAVE_ID)->setEnabled(actionEnabled);
    ActionManager::instance()->action(Constants::CLEAR_ID)->setEnabled(actionEnabled);

    ActionManager::instance()->action(Constants::EDIT_TEXT_ID)->setEnabled(actionEnabled);


    if(myScene->selectedItems().size() > 0)
    {
        if(myScene->selectedItems().size() == 1)
        {
            QString itemName = getFirstSelectedItem();
            if(itemName == TYPE_ID(MyNodePort) || itemName == TYPE_ID(DragLinePoint))
            {
                actionEnabled = false;
            }
        }
        ActionManager::instance()->action(Constants::CUT_ID)->setEnabled(actionEnabled);
        ActionManager::instance()->action(Constants::COPY_ID)->setEnabled(actionEnabled);
        ActionManager::instance()->action(Constants::PASTE_ID)->setEnabled(actionEnabled);

        ActionManager::instance()->action(Constants::ROTATE_LEFT_ID)->setEnabled(actionEnabled);
        ActionManager::instance()->action(Constants::ROTATE_RIGHT_ID)->setEnabled(actionEnabled);
        ActionManager::instance()->action(Constants::BRING_FRONT_ID)->setEnabled(actionEnabled);
        ActionManager::instance()->action(Constants::BRING_BACK_ID)->setEnabled(actionEnabled);
        ActionManager::instance()->action(Constants::DELETE_ID)->setEnabled(actionEnabled);
    }
}

//切换工作区后，将工具栏上的按钮恢复到初始状态，有的要根据是否有子item来设定状态
void MyGraphicsView::setToolBarState()
{
    MY_ASSERT(myScene)

    if(myScene->items().size() <=1 )
    {
        ActionManager::instance()->action(Constants::SAVE_ID)->setEnabled(false);
        ActionManager::instance()->action(Constants::CLEAR_ID)->setEnabled(false);
    }
    else
    {
        ActionManager::instance()->action(Constants::SAVE_ID)->setEnabled(true);
        ActionManager::instance()->action(Constants::CLEAR_ID)->setEnabled(true);
    }

    updateActions();
}

//
//保存当前所添加的控件
void MyGraphicsView::fileSave()
{
    MY_ASSERT(myScene)
    MY_BUILD_MODEL_ONLY
    if(myScene->items().size() <= 1)
    {
        return;
    }

    ActionManager::instance()->action(Constants::ARROW_ID)->setChecked(true);

    QString saveFileName;

    if(MyPageSwitch::instance()->currPageMapping()->fullPathName.isEmpty())
    {
        saveFileName = QFileDialog::getSaveFileName(this,"选择路径");
    }
    else
    {
        saveFileName = MyPageSwitch::instance()->currPageMapping()->fullPathName;
    }
    if(!saveFileName.isEmpty())
    {
        ReturnType  result = FileOperate::instance()->saveFile(saveFileName,MyGraphicsView::instance()->scene()->items());
        if(result == RETURN_SUCCESS)
        {
            MyPageSwitch::instance()->updateCurrMappingInfo(saveFileName);
            MyPageSwitch::instance()->emitSwitchPage();
        }
    }
}

//另存为
void MyGraphicsView::fileSaveAs()
{
    MY_ASSERT(myScene)
    MY_BUILD_MODEL_ONLY

    if(myScene->items().size() <= 1)
    {
        return;
    }
    QString saveFileName = QFileDialog::getSaveFileName(this,"选择路径");
    if(!saveFileName.isEmpty())
    {
        ReturnType  result = FileOperate::instance()->saveFile(saveFileName,MyGraphicsView::instance()->scene()->items());
        if(result == RETURN_SUCCESS)
        {
            MyPageSwitch::instance()->updateCurrMappingInfo(saveFileName);
            MyPageSwitch::instance()->emitSwitchPage();
        }
    }
}

//打开本地文件，将文件的信息保存至每个场景中
//【20161201】新增对所打开的文件先判断是否已存在
void MyGraphicsView::openLocalFile(QString fileName, bool openFile)
{
    if(!openFile && MyPageSwitch::instance()->hasContainFile(fileName))
    {
        MyPageSwitch::instance()->switchToPageByFileName(fileName);
        return;
    }

    QList<CutInfo *> cutInfos;
    ReturnType returnType = FileOperate::instance()->openFile(fileName,cutInfos);
    if(returnType == FILE_ILLEGAL)
    {
        Util::showWarn("文件格式不符，请重新选择！");
    }
    else if(returnType == FILE_VERSION)
    {
        Util::showWarn("所选文件与当前软件版本不一致!");
    }
    else if(returnType == RETURN_SUCCESS)
    {
        scene()->addItem(cutInfos);

        foreach (CutInfo * info, cutInfos)
        {
            delete info->loopProp;
            delete info->judgeProp;
            delete info;
        }

        setKeyCtrlStated(false);

        MyPageSwitch::instance()->updateCurrMappingInfo(fileName);
        MyPageSwitch::instance()->emitSwitchPage();
    }
    scene()->update();
}

//新建工程
void MyGraphicsView::addPage(QString proPath,QString proName)
{
    QString saveFileName = proPath+"/"+proName + QString("_v"+QString::number(M_VERTION)) + SaveFileSuffix;
    MyPageSwitch::instance()->addPage();
    QFile file(saveFileName);
    if(!file.open(QFile::ReadWrite))
    {

    }
    MyPageSwitch::instance()->updateCurrMappingInfo(saveFileName);
}

//从【建模】->【推演】，将控件的处理单元类型进行重置
void MyGraphicsView::resetItemProcessType()
{
    MY_ASSERT(myScene)
    QList<QGraphicsItem *> items = myScene->items();
    foreach(QGraphicsItem * item,items)
    {
        if(TYPE_ID(*item) == TYPE_ID(MyItem))
        {
            MyItem * tmpItem = dynamic_cast<MyItem *>(item);
            if(tmpItem)
            {
//                tmpItem->setProcessType(PRO_NONE);
            }
        }
    }
}

//获取当前视图被选中的顶层item
MyItem * MyGraphicsView::getTopSelectedItem()
{
    if(myScene && myScene->selectedItems().size() == 1)
    {
        QGraphicsItem * topItem = myScene->selectedItems().at(0);
        if(TYPE_ID(*topItem) == TYPE_ID(MyItem))
        {
            MyItem * item = dynamic_cast<MyItem *>(topItem);
            return item;
        }
    }
    return NULL;
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
