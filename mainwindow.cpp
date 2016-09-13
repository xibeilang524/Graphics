#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMenuBar>
#include <QAction>
#include <QGraphicsView>
#include <QToolBar>
#include <QActionGroup>
#include <QComboBox>
#include <QVBoxLayout>
#include <QKeyEvent>
#include <QClipboard>
#include <QFileDialog>
#include <QMessageBox>
#include <QStatusBar>
#include <QDebug>

#include "actionmanager.h"
#include "item/myscene.h"
#include "./item/myitem.h"
#include "./item/myarrow.h"
#include "./item/mytextitem.h"
#include "./SelfWidget/myslider.h"
#include "./SelfWidget/righttoolbox.h"
#include "./SelfWidget/mytextinput.h"
#include "fileoperate.h"
#include "global.h"

#include "typeinfo.h"

using namespace Graphics;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("模型工具");

    rightMenu = NULL;

    createActionAndMenus();

    createContextMenu();

    createSceneAndView();

    createToolBar();

    createStatusBar();

    showMaximized();

    respItemSizeChanged(0);

}

//创建窗口的菜单栏，绑定响应事件
void MainWindow::createActionAndMenus()
{
    QMenu * fileMenu = menuBar()->addMenu("文件(&F)");

    MyAction * newAction = ActionManager::instance()->crateAction(Constants::FILE_ID,QIcon(":/images/new.png"),"新建");
    newAction->setShortcut(QKeySequence("Ctrl+N"));
    ActionManager::instance()->registerAction(newAction,this,SLOT(fileOpen()));

    MyAction * saveAction = ActionManager::instance()->crateAction(Constants::SAVE_ID,QIcon(":/images/save.png"),"保存");
    saveAction->setShortcut(QKeySequence("Ctrl+S"));
    ActionManager::instance()->registerAction(saveAction,this,SLOT(fileSave()));

    MyAction * openAction = ActionManager::instance()->crateAction(Constants::OPEN_ID,QIcon(":/images/open.png"),"打开");
    openAction->setShortcut(QKeySequence("Ctrl+O"));
    ActionManager::instance()->registerAction(openAction,this,SLOT(fileOpen()));

    MyAction * clearAction = ActionManager::instance()->crateAction(Constants::CLEAR_ID,QIcon(":/images/clear.png"),"清空");
    ActionManager::instance()->registerAction(clearAction,this,SLOT(fileClear()));

    MyAction * exitAction = ActionManager::instance()->crateAction(Constants::EXIT_ID,"退出");
    exitAction->setShortcut(QKeySequence("Ctrl+E"));
    ActionManager::instance()->registerAction(exitAction,this,SLOT(exitApp()));

    fileMenu->addAction(newAction);
    fileMenu->addAction(openAction);
    fileMenu->addAction(saveAction);
    fileMenu->addAction(clearAction);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAction);

    QMenu * editMenu = menuBar()->addMenu("编辑(&E)");

    MyAction * cutAction = ActionManager::instance()->crateAction(Constants::CUT_ID,QIcon(":/images/cut.png"),"剪切");
    cutAction->setShortcut(QKeySequence("Ctrl+X"));
    ActionManager::instance()->registerAction(cutAction,this,SLOT(cutItem()));

    MyAction * copyAction = ActionManager::instance()->crateAction(Constants::COPY_ID,QIcon(":/images/copy.png"),"复制");
    copyAction->setShortcut(QKeySequence("Ctrl+C"));
    ActionManager::instance()->registerAction(copyAction,this,SLOT(copyItem()));

    MyAction * pasteAction = ActionManager::instance()->crateAction(Constants::PASTE_ID,QIcon(":/images/paste.png"),"粘贴");
    pasteAction->setShortcut(QKeySequence("Ctrl+V"));
    ActionManager::instance()->registerAction(pasteAction,this,SLOT(pasteItem()));

    MyAction * rotateLeftAction = ActionManager::instance()->crateAction(Constants::ROTATE_LEFT_ID,QIcon(":/images/rotateLeft.png"),"左转90°");
    ActionManager::instance()->registerAction(rotateLeftAction,this,SLOT(rotateItem()));

    MyAction * rotateRightAction = ActionManager::instance()->crateAction(Constants::ROTATE_RIGHT_ID,QIcon(":/images/rotateRight.png"),"右转90°");
    ActionManager::instance()->registerAction(rotateRightAction,this,SLOT(rotateItem()));

    MyAction * bringFrontAction = ActionManager::instance()->crateAction(Constants::BRING_FRONT_ID,QIcon(":/images/bringtofront.png"),"置于顶层");
    ActionManager::instance()->registerAction(bringFrontAction,this,SLOT(bringZItem()));

    MyAction * bringBackAction = ActionManager::instance()->crateAction(Constants::BRING_BACK_ID,QIcon(":/images/sendtoback.png"),"置于底层");
    ActionManager::instance()->registerAction(bringBackAction,this,SLOT(bringZItem()));

    MyAction * deleteAction = ActionManager::instance()->crateAction(Constants::DELETE_ID,QIcon(":/images/delete.png"),"删除");
    ActionManager::instance()->registerAction(deleteAction,this,SLOT(deleteItem()));

    MyAction * editTextAction = ActionManager::instance()->crateAction(Constants::EDIT_TEXT_ID,QIcon(":/images/editText.png"),"编辑文本");
    editTextAction->setShortcut(QKeySequence("Ctrl+T"));
    ActionManager::instance()->registerAction(editTextAction,this,SLOT(editTextItem()));

    editMenu->addAction(editTextAction);
    editMenu->addSeparator();
    editMenu->addAction(cutAction);
    editMenu->addAction(copyAction);
    editMenu->addAction(pasteAction);
    editMenu->addSeparator();
    editMenu->addAction(rotateLeftAction);
    editMenu->addAction(rotateRightAction);
    editMenu->addAction(bringFrontAction);
    editMenu->addAction(bringBackAction);
    editMenu->addAction(deleteAction);

    QMenu * itemMenu = menuBar()->addMenu("条目(&I)");

    MyAction * arrowAction = ActionManager::instance()->crateAction(Constants::ARROW_ID,QIcon(":/images/pointer.png"),"箭头");
    ActionManager::instance()->registerAction(arrowAction,this,SLOT(addItem()),true);
    arrowAction->setType(GRA_NONE);
    arrowAction->setChecked(true);

    MyAction * squareAction = ActionManager::instance()->crateAction(Constants::SQUARE_ID,QIcon(":/images/square.png"),"正方形");
    ActionManager::instance()->registerAction(squareAction,this,SLOT(addItem()),true);
    squareAction->setType(GRA_SQUARE);

    MyAction * rectAction = ActionManager::instance()->crateAction(Constants::RECT_ID,QIcon(":/images/rectange.png"),"矩形");
    ActionManager::instance()->registerAction(rectAction,this,SLOT(addItem()),true);
    rectAction->setType(GRA_RECT);

    MyAction * roundRectAction = ActionManager::instance()->crateAction(Constants::ROUNDRECT_ID,QIcon(":/images/roundedrect.png"),"圆角矩形");
    ActionManager::instance()->registerAction(roundRectAction,this,SLOT(addItem()),true);
    roundRectAction->setType(GRA_ROUND_RECT);

    MyAction * circleAction = ActionManager::instance()->crateAction(Constants::CIRCLE_ID,QIcon(":/images/circle.png"),"圆");
    ActionManager::instance()->registerAction(circleAction,this,SLOT(addItem()),true);
    circleAction->setType(GRA_CIRCLE);

    MyAction * ellipseAction = ActionManager::instance()->crateAction(Constants::ELLIPSE_ID,QIcon(":/images/ellipse.png"),"椭圆");
    ActionManager::instance()->registerAction(ellipseAction,this,SLOT(addItem()),true);
    ellipseAction->setType(GRA_ELLIPSE);

    MyAction * polygonAction = ActionManager::instance()->crateAction(Constants::POLYGON_ID,QIcon(":/images/diamonds.png"),"菱形");
    ActionManager::instance()->registerAction(polygonAction,this,SLOT(addItem()),true);
    polygonAction->setType(GRA_POLYGON);

    MyAction * lineAction = ActionManager::instance()->crateAction(Constants::LINE_ID,QIcon(":/images/linepointer.png"),"线条");
    ActionManager::instance()->registerAction(lineAction,this,SLOT(addItem()),true);
    lineAction->setType(GRA_LINE);

    MyAction * vectorLineAction = ActionManager::instance()->crateAction(Constants::VECTOR_LINE_ID,QIcon(":/images/vectorLine.png"),"连接线");
    ActionManager::instance()->registerAction(vectorLineAction,this,SLOT(addItem()),true);
    vectorLineAction->setType(GRA_VECTOR_LINE);

    MyAction * textAction = ActionManager::instance()->crateAction(Constants::TEXT_ID,QIcon(":/images/text.png"),"文字");
    ActionManager::instance()->registerAction(textAction,this,SLOT(addItem()),true);
    textAction->setType(GRA_TEXT);

    //用于控制一组中只有一个状态被选中
    itemGroup = new QActionGroup(this);

    itemGroup->addAction(arrowAction);
    itemGroup->addAction(squareAction);
    itemGroup->addAction(rectAction);
    itemGroup->addAction(roundRectAction);
    itemGroup->addAction(circleAction);
    itemGroup->addAction(ellipseAction);
    itemGroup->addAction(polygonAction);
    itemGroup->addAction(textAction);
    itemGroup->addAction(lineAction);
    itemGroup->addAction(vectorLineAction);

    itemMenu->addAction(arrowAction);
    itemMenu->addAction(squareAction);
    itemMenu->addAction(rectAction);
    itemMenu->addAction(roundRectAction);
    itemMenu->addAction(circleAction);
    itemMenu->addAction(ellipseAction);
    itemMenu->addAction(polygonAction);
    itemMenu->addAction(textAction);
    itemMenu->addAction(lineAction);
    itemMenu->addAction(vectorLineAction);
}

//新建空白空间
void MainWindow::fileNew()
{

}

//打开本地保存的文件，会先提示是否要保存当前添加的控件
void MainWindow::fileOpen()
{
    respRestItemAction();

    if(scene->items().size()>0)
    {
        fileClear();
    }

    QString openFileName = QFileDialog::getOpenFileName(this,"选择打开文件","","Files(*"+SaveFileSuffix+")");
    if(!openFileName.isEmpty())
    {
        QList<CutInfo> cutInfos;
        ReturnType returnType = FileOperate::instance()->openFile(openFileName,cutInfos);
        if(returnType == FILE_ILLEGAL)
        {
            respShowStatusInfo("文件格式不符，请重新选择！");
        }
        else if(returnType == RETURN_OK)
        {
            scene->addItem(cutInfos);

            respShowStatusInfo("文件解析完成!");
        }
    }
    scene->update();
}

//保存当前所添加的控件
void MainWindow::fileSave()
{
    respRestItemAction();

    QString saveFileName = QFileDialog::getSaveFileName(this,"选择路径");
    if(!saveFileName.isEmpty())
    {
        ReturnType  result = FileOperate::instance()->saveFile(saveFileName,scene->items());
        if(result == RETURN_OK)
        {
            respShowStatusInfo("文件保存成功!");
        }
    }
}

//清空当前的控件
void MainWindow::fileClear()
{
    respRestItemAction();

    int result = QMessageBox::warning(this,"警告","是否清空场景的内容?",QMessageBox::Yes,QMessageBox::No);
    if(result == QMessageBox::Yes)
    {
        scene->clear();
        respItemSizeChanged(0);
    }
}

void MainWindow::exitApp()
{
    qApp->exit();
}

//目前支持一个控件的剪切，只支持除箭头以外控件操作
void MainWindow::cutItem()
{
    QList<QGraphicsItem *> selectedItems = scene->selectedItems();

    if(selectedItems.size() == 1)
    {
        QString itemName = typeid(*(selectedItems.first())).name();

        if(itemName == typeid(MyItem).name())
        {
            MyItem * item = dynamic_cast<MyItem *>(scene->selectedItems().first());

            cutTmpInfo.graphicsType = item->getType();
            cutTmpInfo.itemProperty = item->getProperty();
            cutTmpInfo.content = item->getText();
            deleteItem();
        }
        else if(itemName == typeid(MyTextItem).name())
        {
            MyTextItem * item = dynamic_cast<MyTextItem*>(selectedItems.first());

            cutTmpInfo.graphicsType = item->getType();
            cutTmpInfo.itemProperty = item->getProperty();
            delete item;
        }
    }
}

void MainWindow::copyItem()
{
    QList<QGraphicsItem *> selectedItems = scene->selectedItems();

    if(selectedItems.size() == 1)
    {
        QString itemName = typeid(*(selectedItems.first())).name();

        if(itemName == typeid(MyItem).name())
        {
            MyItem * item = dynamic_cast<MyItem *>(scene->selectedItems().first());

            cutTmpInfo.graphicsType = item->getType();
            cutTmpInfo.itemProperty = item->getProperty();
            cutTmpInfo.content = item->getText();
        }
        else if(itemName == typeid(MyTextItem).name())
        {
            MyTextItem * item = dynamic_cast<MyTextItem*>(selectedItems.first());

            cutTmpInfo.graphicsType = item->getType();
            cutTmpInfo.itemProperty = item->getProperty();
            cutTmpInfo.content = item->toPlainText();
        }
    }
}

void MainWindow::pasteItem()
{
    scene->addItem(cutTmpInfo,true);
}

void MainWindow::rotateItem()
{
    QList<QGraphicsItem *> selectedItems = scene->selectedItems();

    if(selectedItems.size() !=  1)
    {
        return;
    }

    MyItem * graphicsTmp = dynamic_cast<MyItem *>(scene->selectedItems().first());

    QString objName = QObject::sender()->objectName();
    if(objName == QString(Constants::ROTATE_LEFT_ID))
    {
        graphicsTmp->setRotation(-90);
        graphicsTmp->updateRotation(-90);
    }
    else if(objName == QString(Constants::ROTATE_RIGHT_ID))
    {
        graphicsTmp->setRotation(90);
        graphicsTmp->updateRotation(90);
    }
}

void MainWindow::bringZItem()
{
    if (scene->selectedItems().isEmpty())
    {
         return;
    }

    QGraphicsItem *selectedItem = scene->selectedItems().first();
//    QList<QGraphicsItem *> overlapItems = selectedItem->collidingItems();     //找到与当前item有重合的items
    QList<QGraphicsItem * > allItems = scene->items();

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
    QString itemName = typeid(*selectedItem).name();
    if(itemName == typeid(MyItem).name())
    {
        MyItem * tmp = dynamic_cast<MyItem *>(selectedItem);
        tmp->setZValue(zValue);
    }
    else if(itemName == typeid(MyTextItem).name())
    {
        MyTextItem * tmp = dynamic_cast<MyTextItem *>(selectedItem);
        tmp->setZValue(zValue);
    }
}

//还需要从scene中删除item
void MainWindow::deleteItem()
{
    QList<QGraphicsItem *> selectedItems = scene->selectedItems();

    foreach(QGraphicsItem * item, selectedItems)
    {
        QString itemName = typeid(*item).name();
        if(itemName == typeid(MyArrow).name())
        {
            MyArrow * tmp = dynamic_cast<MyArrow *>(item);

            tmp->getStartItem()->removeArrow(tmp);
            tmp->getEndItem()->removeArrow(tmp);
            scene->removeItem(tmp);

            delete tmp;
        }
    }

    selectedItems = scene->selectedItems();

    foreach (QGraphicsItem * item, selectedItems)
    {
        QString itemName = typeid(*item).name();
        if(itemName == typeid(MyItem).name())
        {
            MyItem * tmp = dynamic_cast<MyItem *>(item);
            tmp->removeArrows();
            scene->removeItem(tmp);
            delete tmp;
        }
    }

    selectedItems = scene->selectedItems();

    foreach (QGraphicsItem * item, selectedItems)
    {
        QString itemName = typeid(*item).name();
        if(itemName == typeid(MyTextItem).name())
        {
            MyTextItem * tmp = dynamic_cast<MyTextItem *>(item);
            scene->removeItem(tmp);
            delete tmp;
        }
    }

    scene->update();
}

//编辑文本
void MainWindow::editTextItem()
{
    QList<QGraphicsItem *> selectedItems = scene->selectedItems();
    if(selectedItems.size() == 1)
    {
        QString itemName = typeid(*(selectedItems.first())).name();

        if(itemName == typeid(MyItem).name())
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

void MainWindow::addItem()
{
    MyAction * tmpAction = dynamic_cast<MyAction *>(QObject::sender());
    if(tmpAction)
    {
        CurrAddGraType = tmpAction->getType();
    }
}

//创建场景和视图
void MainWindow::createSceneAndView()
{
    QWidget * centralWidget = new QWidget;
    QHBoxLayout * layout = new QHBoxLayout;

    SceneWidth = SceneHeight = 5000;

    scene = new MyScene(rightMenu);
    scene->setSceneRect(0,0,SceneWidth,SceneHeight);

    connect(scene,SIGNAL(resetItemAction()),this,SLOT(respRestItemAction()));
    connect(scene, SIGNAL(selectionChanged()),this, SLOT(updateActions()));
    connect(scene,SIGNAL(deleteKeyPress()),this,SLOT(deleteItem()));
    connect(scene,SIGNAL(itemSizeChanged(int)),this,SLOT(respItemSizeChanged(int)));

    view = new QGraphicsView(this);
    view->setScene(scene);
//    view->setDragMode(QGraphicsView::RubberBandDrag);   //此行添加后会导致scene无法捕获mousemove事件
//    view->setRenderHints(QPainter::Antialiasing| QPainter::TextAntialiasing);

    rightToolBox = new RightToolBox;
    connect(this,SIGNAL(initToolBox(int,ItemProperty)),rightToolBox,SLOT(respInitToolBox(int,ItemProperty)));
    connect(rightToolBox,SIGNAL(updateProperty(ItemProperty)),this,SLOT(respPropertyUpdate(ItemProperty)));
    connect(rightToolBox,SIGNAL(deleteCurrItem()),this,SLOT(deleteItem()));
    connect(scene,SIGNAL(selectedItemPosChanged(MyRect)),rightToolBox,SLOT(respItemPosChanged(MyRect)));

    layout->addWidget(view);
    layout->addWidget(rightToolBox);
    centralWidget->setLayout(layout);

    this->setCentralWidget(centralWidget);
}

//响应item改变
void MainWindow::respItemSizeChanged(int size)
{
    bool actionEnabled = size;

    ActionManager::instance()->action(Constants::SAVE_ID)->setEnabled(actionEnabled);
    ActionManager::instance()->action(Constants::CLEAR_ID)->setEnabled(actionEnabled);

    ActionManager::instance()->action(Constants::EDIT_TEXT_ID)->setEnabled(actionEnabled);

    ActionManager::instance()->action(Constants::CUT_ID)->setEnabled(actionEnabled);
    ActionManager::instance()->action(Constants::COPY_ID)->setEnabled(actionEnabled);

    ActionManager::instance()->action(Constants::ROTATE_LEFT_ID)->setEnabled(actionEnabled);
    ActionManager::instance()->action(Constants::ROTATE_RIGHT_ID)->setEnabled(actionEnabled);
    ActionManager::instance()->action(Constants::BRING_FRONT_ID)->setEnabled(actionEnabled);
    ActionManager::instance()->action(Constants::BRING_BACK_ID)->setEnabled(actionEnabled);
    ActionManager::instance()->action(Constants::DELETE_ID)->setEnabled(actionEnabled);
}

//当在scene中右击时，将item工具栏中的状态恢复至箭头状态
void MainWindow::respRestItemAction()
{
    ActionManager::instance()->action(Constants::ARROW_ID)->setChecked(true);
}

//当选择的item状态改变后，更新action
void MainWindow::updateActions()
{
    rightMenu->addAction(ActionManager::instance()->action(Constants::ROTATE_LEFT_ID));
    rightMenu->addAction(ActionManager::instance()->action(Constants::ROTATE_RIGHT_ID));
    rightMenu->addAction(ActionManager::instance()->action(Constants::BRING_FRONT_ID));
    rightMenu->addAction(ActionManager::instance()->action(Constants::BRING_BACK_ID));
    rightMenu->addAction(ActionManager::instance()->action(Constants::DELETE_ID));

    int selectedSize = scene->selectedItems().size();

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
        ActionManager::instance()->action(Constants::DELETE_ID)->setEnabled(true);

        QString itemName = typeid(*(scene->selectedItems().first())).name();

        if(itemName == typeid(MyItem).name())
        {
            MyItem * myItem = dynamic_cast<MyItem *>(scene->selectedItems().first());
            property = myItem->getProperty();
        }
        else if(itemName == typeid(MyTextItem).name())
        {
            MyTextItem * textItem = dynamic_cast<MyTextItem *>(scene->selectedItems().first());
            property = textItem->getProperty();
        }
        else if(itemName == typeid(MyArrow).name())
        {
            MyArrow  * arrowItem = dynamic_cast<MyArrow *>(scene->selectedItems().first());
            property = arrowItem->getProperty();
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
        ActionManager::instance()->action(Constants::DELETE_ID)->setEnabled(true);
    }

    scene->update();

    emit initToolBox(selectedSize,property);
}

//更新item的属性
void MainWindow::respPropertyUpdate(ItemProperty property)
{
    if(scene->selectedItems().size() == 1)
    {
        QString itemName = typeid(*(scene->selectedItems().first())).name();

        if(itemName == typeid(MyItem).name())
        {
            MyItem * myItem = dynamic_cast<MyItem *>(scene->selectedItems().first());
            myItem->setProperty(property);
        }
        else if(itemName == typeid(MyTextItem).name())
        {
            MyTextItem * textItem = dynamic_cast<MyTextItem *>(scene->selectedItems().first());
            textItem->setProperty(property);
        }
        else if(itemName == typeid(MyArrow).name())
        {
            MyArrow  * arrowItem = dynamic_cast<MyArrow *>(scene->selectedItems().first());
            arrowItem->setProperty(property);
        }

        scene->update();
    }
}

//创建右键菜单，作为每个相具有的功能
void MainWindow::createContextMenu()
{
    rightMenu = new QMenu;

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
    rightMenu->addAction(ActionManager::instance()->action(Constants::DELETE_ID));

}

//创建工具栏
void MainWindow::createToolBar()
{
    QToolBar * fileBar = addToolBar("File");
    fileBar->addAction(ActionManager::instance()->action(Constants::FILE_ID));
    fileBar->addAction(ActionManager::instance()->action(Constants::OPEN_ID));
    fileBar->addAction(ActionManager::instance()->action(Constants::SAVE_ID));
    fileBar->addAction(ActionManager::instance()->action(Constants::CLEAR_ID));

    QToolBar * itemBar = addToolBar("Item");
    itemBar->addAction(ActionManager::instance()->action(Constants::ARROW_ID));
    itemBar->addSeparator();
    itemBar->addAction(ActionManager::instance()->action(Constants::SQUARE_ID));
    itemBar->addAction(ActionManager::instance()->action(Constants::RECT_ID));
    itemBar->addAction(ActionManager::instance()->action(Constants::ROUNDRECT_ID));
    itemBar->addAction(ActionManager::instance()->action(Constants::CIRCLE_ID));
    itemBar->addAction(ActionManager::instance()->action(Constants::ELLIPSE_ID));
    itemBar->addAction(ActionManager::instance()->action(Constants::POLYGON_ID));
    itemBar->addAction(ActionManager::instance()->action(Constants::TEXT_ID));
    itemBar->addAction(ActionManager::instance()->action(Constants::LINE_ID));
    itemBar->addAction(ActionManager::instance()->action(Constants::VECTOR_LINE_ID));

    QToolBar * editBar = addToolBar("Edit");
    editBar->addAction(ActionManager::instance()->action(Constants::CUT_ID));
    editBar->addAction(ActionManager::instance()->action(Constants::COPY_ID));
    editBar->addAction(ActionManager::instance()->action(Constants::PASTE_ID));
    editBar->addAction(ActionManager::instance()->action(Constants::ROTATE_LEFT_ID));
    editBar->addAction(ActionManager::instance()->action(Constants::ROTATE_RIGHT_ID));
    editBar->addAction(ActionManager::instance()->action(Constants::BRING_FRONT_ID));
    editBar->addAction(ActionManager::instance()->action(Constants::BRING_BACK_ID));
    editBar->addAction(ActionManager::instance()->action(Constants::DELETE_ID));

    QToolBar * sceneBar = addToolBar("Scene");

//    addToolBar(Qt::BottomToolBarArea,sceneBar);

//    sizeBox = new QComboBox;
//    QStringList list;
//    list<<"50%"<<"75%"<<"100%"<<"125%"<<"150%";
//    sizeBox->addItems(list);
//    sizeBox->setCurrentIndex(2);
//    connect(sizeBox,SIGNAL(currentIndexChanged(QString)),this,SLOT(sceneScaled(QString)));
//    sceneBar->addWidget(sizeBox);

    mySlider = new MySlider;
    connect(mySlider,SIGNAL(scaleView(int)),this,SLOT(sceneScaled(int)));
    sceneBar->addWidget(mySlider);
}

//创建状态栏
void MainWindow::createStatusBar()
{
    QStatusBar * statusBar = new QStatusBar;
    setStatusBar(statusBar);
}

//显示状态信息
void MainWindow::respShowStatusInfo(QString text,int time)
{
    statusBar()->showMessage(text);
}

//切换视图缩放
void MainWindow::sceneScaled(int currScale)
{
//     double newScale = currScale.left(currScale.indexOf(tr("%"))).toDouble() / 100.0;
     double newScale = currScale/100.0;
     QMatrix oldMatrix = view->matrix();
     view->resetMatrix();
     view->translate(oldMatrix.dx(), oldMatrix.dy());
     view->scale(newScale, newScale);
}

MainWindow::~MainWindow()
{
    delete mySlider;

    delete ui;
}
