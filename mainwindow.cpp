#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMenuBar>
#include <QAction>
#include <QToolBar>
#include <QActionGroup>
#include <QVBoxLayout>
#include <QFileDialog>
#include <QMessageBox>
#include <QStatusBar>
#include <QKeyEvent>
#include <QDebug>
#include <QMessageBox>

#include "./manager/actionmanager.h"
#include "./manager/menumanager.h"
#include "./item/myscene.h"
#include "./item/mygraphicsview.h"
#include "./SelfWidget/myslider.h"
#include "./SelfWidget/lefticonwidget.h"
#include "./SelfWidget/hidesplit.h"
#include "./SelfWidget/righttoolbox.h"
#include "./SelfWidget/mypageswitch.h"
#include "./SelfWidget/serviceview.h"
#include "./SelfWidget/aboutsoft.h"
#include "./manager/mylinecombobox.h"
#include "./simulate/simulatecontrolpanel.h"
#include "./project/myprowizard.h"
#include "fileoperate.h"
#include "global.h"
#include "util.h"

using namespace Graphics;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    GlobalMainWindow = this;
    GlobalIsSimulateState = false;

    mySlider = NULL;
    leftIconWidget = NULL;
    rightToolBox = NULL;
    view = NULL;
    simulatePanel = NULL;

    fileBar = NULL;
    itemBar = NULL;
    sceneBar = NULL;
    editBar = NULL;
    databaseBar = NULL;

    GlobalItemZValue = 0;

    createSceneAndView();

    createLineComboBox();
    createActionAndMenus();
    createToolBar();
    createStatusBar();

    MyPageSwitch::instance()->addSwitchContextMenu();
    MyPageSwitch::instance()->addPage();

    view->addContextMenuItem();
    view->addViewContextMenu();
    view->respItemSizeChanged(0);

    showMaximized();

    resetEditActionState(false);

    ActionManager::instance()->action(Constants::LOCK_ID)->setEnabled(false);
    ActionManager::instance()->action(Constants::UNLOCK_ID)->setEnabled(false);
    SplitManager::instance()->split(QString(Constants::HIDE_TOOL_ID))->setContainerVisible();

    ActionManager::instance()->action(Constants::BUILD_MODEL_ID)->setChecked(true);
}

//创建窗口的菜单栏，绑定响应事件
void MainWindow::createActionAndMenus()
{
    MyAction * newProAction = ActionManager::instance()->crateAction(Constants::PROJECT_ID,QIcon(":/images/project.png"),"新建工程");
    ActionManager::instance()->registerAction(newProAction,MyProWizard::instance(),SLOT(exec()));

    MyAction * newFileAction = ActionManager::instance()->crateAction(Constants::FILE_ID,QIcon(":/images/new.png"),"新建工作区");
    newFileAction->setShortcut(QKeySequence("Ctrl+N"));
    ActionManager::instance()->registerAction(newFileAction,MyPageSwitch::instance(),SLOT(addPage()));

    MyAction * saveAction = ActionManager::instance()->crateAction(Constants::SAVE_ID,QIcon(":/images/save.png"),"保存");
    saveAction->setShortcut(QKeySequence("Ctrl+S"));
    ActionManager::instance()->registerAction(saveAction,MyGraphicsView::instance(),SLOT(fileSave()));

    MyAction * saveAsAction = ActionManager::instance()->crateAction(Constants::SAVE_AS_ID,QIcon(""),"另存为");
    saveAsAction->setShortcut(QKeySequence("Ctrl+Shift+S"));
    ActionManager::instance()->registerAction(saveAsAction,MyGraphicsView::instance(),SLOT(fileSaveAs()));

    MyAction * openAction = ActionManager::instance()->crateAction(Constants::OPEN_ID,QIcon(":/images/open.png"),"打开");
    openAction->setShortcut(QKeySequence("Ctrl+O"));
    ActionManager::instance()->registerAction(openAction,this,SLOT(fileOpen()));

    MyAction * clearAction = ActionManager::instance()->crateAction(Constants::CLEAR_ID,QIcon(":/images/clear.png"),"清空");
    ActionManager::instance()->registerAction(clearAction,this,SLOT(fileClear()));

    MyAction * exitAction = ActionManager::instance()->crateAction(Constants::EXIT_ID,"退出");
    exitAction->setShortcut(QKeySequence("Ctrl+Q"));
    ActionManager::instance()->registerAction(exitAction,this,SLOT(close()));

    MyMenu * fileMenu = MenuManager::instance()->createMenu(Constants::MENUBAR_FILE_MENU,QString("文件(&F)"));
    menuBar()->addMenu(fileMenu);

    fileMenu->addAction(newProAction);
    fileMenu->addAction(newFileAction);
    fileMenu->addAction(openAction);
    fileMenu->addAction(saveAction);
    fileMenu->addAction(saveAsAction);
    fileMenu->addAction(clearAction);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAction);

    MyAction * undoAction = ActionManager::instance()->crateAction(Constants::UNDO_ID,QIcon(":/images/undo.png"),"撤销");
    ActionManager::instance()->registerAction(undoAction,MyGraphicsView::instance(),SLOT(undoAndRedoItem()));
    undoAction->setShortcut(QKeySequence("Ctrl+Z"));
    undoAction->setEnabled(false);

    MyAction * redoAction = ActionManager::instance()->crateAction(Constants::REDO_ID,QIcon(":/images/redo.png"),"重做");
    ActionManager::instance()->registerAction(redoAction,MyGraphicsView::instance(),SLOT(undoAndRedoItem()));
    redoAction->setShortcut(QKeySequence("Ctrl+Y"));
    redoAction->setEnabled(false);

    MyAction * cutAction = ActionManager::instance()->crateAction(Constants::CUT_ID,QIcon(":/images/cut.png"),"剪切");
    cutAction->setShortcut(QKeySequence("Ctrl+X"));
    ActionManager::instance()->registerAction(cutAction,MyGraphicsView::instance(),SLOT(cutItem()));

    MyAction * copyAction = ActionManager::instance()->crateAction(Constants::COPY_ID,QIcon(":/images/copy.png"),"复制");
    copyAction->setShortcut(QKeySequence("Ctrl+C"));
    ActionManager::instance()->registerAction(copyAction,MyGraphicsView::instance(),SLOT(copyItem()));

    MyAction * pasteAction = ActionManager::instance()->crateAction(Constants::PASTE_ID,QIcon(":/images/paste.png"),"粘贴");
    pasteAction->setShortcut(QKeySequence("Ctrl+V"));
    ActionManager::instance()->registerAction(pasteAction,MyGraphicsView::instance(),SLOT(pasteItem()));

    MyAction * clearPasteAction = ActionManager::instance()->crateAction(Constants::CLEAR_PASTE_ID,QIcon(":/images/clearPaste.png"),"清空剪切板");
    ActionManager::instance()->registerAction(clearPasteAction,MyGraphicsView::instance(),SLOT(clearPasteItem()));
    ActionManager::instance()->action(Constants::CLEAR_PASTE_ID)->setEnabled(false);

    MyAction * rotateLeftAction = ActionManager::instance()->crateAction(Constants::ROTATE_LEFT_ID,QIcon(":/images/rotateLeft.png"),"旋转90°(逆时针)");
    ActionManager::instance()->registerAction(rotateLeftAction,MyGraphicsView::instance(),SLOT(rotateItem()));

    MyAction * rotateRightAction = ActionManager::instance()->crateAction(Constants::ROTATE_RIGHT_ID,QIcon(":/images/rotateRight.png"),"旋转90°(顺时针)");
    ActionManager::instance()->registerAction(rotateRightAction,MyGraphicsView::instance(),SLOT(rotateItem()));

    MyAction * bringFrontAction = ActionManager::instance()->crateAction(Constants::BRING_FRONT_ID,QIcon(":/images/sendtoback.png"),"置于顶层");
    ActionManager::instance()->registerAction(bringFrontAction,MyGraphicsView::instance(),SLOT(bringZItem()));

    MyAction * bringBackAction = ActionManager::instance()->crateAction(Constants::BRING_BACK_ID,QIcon(":/images/bringtofront.png"),"置于底层");
    ActionManager::instance()->registerAction(bringBackAction,MyGraphicsView::instance(),SLOT(bringZItem()));

    MyAction * lockAction = ActionManager::instance()->crateAction(Constants::LOCK_ID,QIcon(":/images/lock.png"),"锁定");
    lockAction->setShortcut(QKeySequence("Ctrl+L"));
    ActionManager::instance()->registerAction(lockAction,MyGraphicsView::instance(),SLOT(lockAndunlockItem()));

    MyAction * unlockAction = ActionManager::instance()->crateAction(Constants::UNLOCK_ID,QIcon(":/images/unlock.png"),"解锁");
    unlockAction->setShortcut(QKeySequence("Ctrl+Shift+L"));
    ActionManager::instance()->registerAction(unlockAction,MyGraphicsView::instance(),SLOT(lockAndunlockItem()));

    MyAction * deleteAction = ActionManager::instance()->crateAction(Constants::DELETE_ID,QIcon(":/images/delete.png"),"删除");
    ActionManager::instance()->registerAction(deleteAction,MyGraphicsView::instance(),SLOT(deleteItem()));
    deleteAction->setShortcut(QKeySequence("Delete"));

    MyAction * editTextAction = ActionManager::instance()->crateAction(Constants::EDIT_TEXT_ID,QIcon(":/images/editText.png"),"编辑名称");
    editTextAction->setShortcut(QKeySequence("Ctrl+T"));
    ActionManager::instance()->registerAction(editTextAction,MyGraphicsView::instance(),SLOT(respEditText()));

    MyAction * propertyEditAction = ActionManager::instance()->crateAction(Constants::PROPERTY_EDIT_ID,QIcon(":/images/editProp.png"),"编辑服务");
    propertyEditAction->setShortcut(QKeySequence("Ctrl+E"));
    ActionManager::instance()->registerAction(propertyEditAction,MyGraphicsView::instance(),SLOT(editPropertyItem()));

    MyMenu * editMenu = MenuManager::instance()->createMenu(Constants::MENUBAR_EDIT_MENU,QString("编辑(&E)"));
    menuBar()->addMenu(editMenu);

    editMenu->addAction(editTextAction);
    editMenu->addSeparator();
    editMenu->addAction(undoAction);
    editMenu->addAction(redoAction);
    editMenu->addAction(cutAction);
    editMenu->addAction(copyAction);
    editMenu->addAction(pasteAction);
    editMenu->addSeparator();
    editMenu->addAction(rotateLeftAction);
    editMenu->addAction(rotateRightAction);
    editMenu->addAction(bringFrontAction);
    editMenu->addAction(bringBackAction);
    editMenu->addAction(lockAction);
    editMenu->addAction(unlockAction);
    editMenu->addAction(deleteAction);

    MyAction * dragAbleAction = ActionManager::instance()->crateAction(Constants::DRAG_ABLE_ID,QIcon(":/images/dragable.png"),"窗口允许拖拽");
    ActionManager::instance()->registerAction(dragAbleAction,MyGraphicsView::instance(),SLOT(setViewDragEnable(bool)),true);
    ActionManager::instance()->action(Constants::DRAG_ABLE_ID)->setChecked(true);

    MyAction * arrowAction = ActionManager::instance()->crateAction(Constants::ARROW_ID,QIcon(":/images/pointer.png"),"箭头");
    ActionManager::instance()->registerAction(arrowAction,this,SLOT(recordClickedItem()),true);
    arrowAction->setType(GRA_NONE);
    arrowAction->setChecked(true);

    MyAction * lineAction = ActionManager::instance()->crateAction(Constants::LINE_ID,QIcon(":/images/linepointer.png"),"线条");
    ActionManager::instance()->registerAction(lineAction,this,SLOT(recordClickedItem()),true);
    lineAction->setType(GRA_LINE);

    MyAction * vectorLineAction = ActionManager::instance()->crateAction(Constants::VECTOR_LINE_ID,QIcon(":/images/vectorLine.png"),"连接线");
    ActionManager::instance()->registerAction(vectorLineAction,this,SLOT(recordClickedItem()),true);
    vectorLineAction->setType(GRA_VECTOR_LINE);

    MyAction * textAction = ActionManager::instance()->crateAction(Constants::TEXT_ID,QIcon(":/images/text.png"),"文字");
    ActionManager::instance()->registerAction(textAction,this,SLOT(recordClickedItem()),true);
    textAction->setType(GRA_TEXT);

    //用于控制一组中只有一个状态被选中
    itemGroup = new QActionGroup(this);

    itemGroup->addAction(arrowAction);
    itemGroup->addAction(textAction);
    itemGroup->addAction(lineAction);
    itemGroup->addAction(vectorLineAction);

    MyMenu * itemMenu = MenuManager::instance()->createMenu(Constants::MENUBAR_ITEM_MENU,QString("类型(&I)"));
    menuBar()->addMenu(itemMenu);

    itemMenu->addAction(arrowAction);
    itemMenu->addAction(textAction);
    itemMenu->addAction(lineAction);
    itemMenu->addAction(vectorLineAction);

    MyAction * buildModelAction = ActionManager::instance()->crateAction(Constants::BUILD_MODEL_ID,QIcon(":/images/buildmodel.png"),"建模");
    buildModelAction->setShortcut(QKeySequence("Ctrl+M"));
    ActionManager::instance()->registerAction(buildModelAction,this,SLOT(switchWorkModel()),true);

    MyAction *simulateAction = ActionManager::instance()->crateAction(Constants::SIMLUATE_ID,QIcon(":/images/simulate.png"),"推演");
    simulateAction->setShortcut(QKeySequence("Ctrl+Shift+M"));
    ActionManager::instance()->registerAction(simulateAction,this,SLOT(switchWorkModel()),true);

    workModelGroup = new QActionGroup(this);
    workModelGroup->addAction(buildModelAction);
    workModelGroup->addAction(simulateAction);

    //【窗口菜单栏】
    MyAction * fullScreenAction = ActionManager::instance()->crateAction(Constants::FULL_SCREEN_ID,QIcon(":/images/fullscreen.png"),"全屏");
    fullScreenAction->setShortcut(QKeySequence("Ctrl+Shift+F11"));
    ActionManager::instance()->registerAction(fullScreenAction,this,SLOT(switchFullScreen()));

    MyAction * hideIconAction = ActionManager::instance()->crateAction(Constants::HIDE_ICON_ID,QIcon(""),"隐藏控件区");
    hideIconAction->setShortcut(QKeySequence("Alt+L"));
    ActionManager::instance()->registerAction(hideIconAction,this,SLOT(hideSubWidget()));

    MyAction * hideToolAction = ActionManager::instance()->crateAction(Constants::HIDE_TOOL_ID,QIcon(""),"隐藏属性编辑区");
    hideToolAction->setShortcut(QKeySequence("Alt+R"));
    ActionManager::instance()->registerAction(hideToolAction,this,SLOT(hideSubWidget()));

    MyAction * hideSpaceAction = ActionManager::instance()->crateAction(Constants::HIDE_WORKSPACE_ID,QIcon(""),"隐藏工作区");
    hideSpaceAction->setShortcut(QKeySequence("Alt+U"));
    ActionManager::instance()->registerAction(hideSpaceAction,this,SLOT(hideSubWidget()));

    MyMenu * widgetMenu = MenuManager::instance()->createMenu(Constants::MENUBAR_WIDGET_MENU,QString("窗口(&W)"));
    menuBar()->addMenu(widgetMenu);
    widgetMenu->addAction(fullScreenAction);
    widgetMenu->addAction(hideIconAction);
    widgetMenu->addAction(hideToolAction);
    widgetMenu->addAction(hideSpaceAction);
    widgetMenu->addSeparator();
    widgetMenu->addAction(buildModelAction);
    widgetMenu->addAction(simulateAction);

    //【帮助菜单栏】
    MyAction * supportAction = ActionManager::instance()->crateAction(Constants::TEC_SUPPORT_ID,QIcon(":/images/getsupport.png"),"技术支持");
//    supportAction->setShortcut(QKeySequence("Ctrl+Q"));
    ActionManager::instance()->registerAction(supportAction,this,SLOT(getTecSupport()));

    MyAction * aboutAction = ActionManager::instance()->crateAction(Constants::ABOUT_ID,QIcon(":/images/about.png"),"关于");
    aboutAction->setShortcut(QKeySequence("Ctrl+P"));
    ActionManager::instance()->registerAction(aboutAction,this,SLOT(showAbout()));

    MyMenu * helpMenu = MenuManager::instance()->createMenu(Constants::MENUBAR_HELP_MENU,QString("帮助(&H)"));
    menuBar()->addMenu(helpMenu);
    helpMenu->addAction(supportAction);
    helpMenu->addAction(aboutAction);

    MyAction * databaseViewAction = ActionManager::instance()->crateAction(Constants::DATABASE_VIEW,QIcon(":/images/database_view.png"),"查看数据库");
//    databaseViewAction->setShortcut(QKeySequence("Ctrl+P"));
    ActionManager::instance()->registerAction(databaseViewAction,ServiceView::instance(),SLOT(viewDatabaseContent()));

    MyAction * databaseRefreshAction = ActionManager::instance()->crateAction(Constants::DATABASE_REFRESH,QIcon(":/images/database_refresh.png"),"刷新数据库");
    ActionManager::instance()->registerAction(databaseRefreshAction,ServiceView::instance(),SLOT(refreshDatabaseContent()));

    //【工作区间右键菜单】
    MyAction * closeWorkAction = ActionManager::instance()->crateAction(Constants::CLOSE_WORKSPACE,QIcon(""),"关闭工作区");
    ActionManager::instance()->registerAction(closeWorkAction,MyPageSwitch::instance(),SLOT(closePage()));

    MyAction * saveWorkAction = ActionManager::instance()->crateAction(Constants::SAVE_WORKSPACE,QIcon(":/images/save.png"),"保存");
    ActionManager::instance()->registerAction(saveWorkAction,MyGraphicsView::instance(),SLOT(fileSave()));

    MyAction * closeLeftWorkAction = ActionManager::instance()->crateAction(Constants::CLOSE_LEFT_WORKSPACE,QIcon(""),"关闭左侧所有");
    ActionManager::instance()->registerAction(closeLeftWorkAction,MyPageSwitch::instance(),SLOT(closeLeftPage()));

    MyAction * closeRightWorkAction = ActionManager::instance()->crateAction(Constants::CLOSE_RIGHT_WORKSPACE,QIcon(""),"关闭右侧所有");
    ActionManager::instance()->registerAction(closeRightWorkAction,MyPageSwitch::instance(),SLOT(closeRightPage()));
}

//直接处理，无需再出发KeyPressEvent，否则一个事件会处理两遍
void MainWindow::keyPress(QKeyEvent *event)
{
//    keyPressEvent(event);
    //左右切换工作区间
    if(event->modifiers() == Qt::AltModifier && event->key() == Qt::Key_Left)
    {
        if(GlobalWindowState == WINDOW_BUILD_MODEL)
        {
            MyPageSwitch::instance()->switchFrontBack(true);
        }
    }
    else if(event->modifiers() == Qt::AltModifier && event->key() == Qt::Key_Right)
    {
        if(GlobalWindowState == WINDOW_BUILD_MODEL)
        {
            MyPageSwitch::instance()->switchFrontBack(false);
        }
    }
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    //全屏/非全屏
    if(event->modifiers() == Qt::ControlModifier && event->modifiers() == Qt::ShiftModifier && event->key() == Qt::Key_F11)
    {
        switchFullScreen();
    }
    //隐藏左侧
    else if(event->modifiers() == Qt::AltModifier && event->key() == Qt::Key_L)
    {
        if(GlobalWindowState == WINDOW_BUILD_MODEL)
        {
            setIconWidgetState();
        }
    }
    //隐藏右侧
    else if(event->modifiers() == Qt::AltModifier && event->key() == Qt::Key_R)
    {
        if(GlobalWindowState == WINDOW_BUILD_MODEL)
        {
            setToolWidgetState();
        }
        else
        {
            setSimulateWidgetState();
        }
    }
    //隐藏工作区
    else if(event->modifiers() == Qt::AltModifier && event->key() == Qt::Key_U)
    {
        if(GlobalWindowState == WINDOW_BUILD_MODEL)
        {
            setWorkspaceState();
        }
    }
    //关闭工作区
    else if(event->modifiers() == Qt::ControlModifier && event->key() == Qt::Key_W)
    {
        if(GlobalWindowState == WINDOW_BUILD_MODEL)
        {
            MyPageSwitch::instance()->closePage();
        }
    }
    //工作模式切换[推演->建模]
    else if(event->modifiers() == Qt::ControlModifier && event->key() == Qt::Key_M)
    {
        if(!GlobalIsSimulateState)
        {
            if(!ActionManager::instance()->action(Constants::SIMLUATE_ID)->isChecked())
            {
                ActionManager::instance()->action(Constants::SIMLUATE_ID)->setChecked(true);
            }
        }
        else
        {
            Util::showWarn("当前在推演状态，无法功能切换!");
        }
    }
    //[建模->推演]
    else if(event->modifiers() == Qt::ControlModifier && event->modifiers() == Qt::ShiftModifier && event->key() == Qt::Key_M)
    {
        if(!GlobalIsSimulateState)
        {
            if(!ActionManager::instance()->action(Constants::BUILD_MODEL_ID)->isChecked())
            {
                ActionManager::instance()->action(Constants::BUILD_MODEL_ID)->setChecked(true);
            }
        }
    }

    QMainWindow::keyPressEvent(event);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    int result = Util::getWarnChoice("是否退出程序");

    if(result == QMessageBox::Yes)
    {
        event->accept();
    }
    else
    {
        event->ignore();
    }
}

//切换全屏
void MainWindow::switchFullScreen()
{
    if(isFullScreen())
    {
        showMaximized();
        ActionManager::instance()->action(Constants::FULL_SCREEN_ID)->setIcon(QIcon(":/images/fullscreen.png"));
        ActionManager::instance()->action(Constants::FULL_SCREEN_ID)->setText("全屏");
    }
    else
    {
        showFullScreen();
        ActionManager::instance()->action(Constants::FULL_SCREEN_ID)->setIcon(QIcon(":/images/exit_fullscreen.png"));
        ActionManager::instance()->action(Constants::FULL_SCREEN_ID)->setText("退出全屏");
    }
}

//显示/隐藏左右的控件
void MainWindow::hideSubWidget()
{
    QString objName = QObject::sender()->objectName();

    if(GlobalWindowState == WINDOW_BUILD_MODEL)
    {
        if(objName == QString(Constants::HIDE_ICON_ID))
        {
            setIconWidgetState();
        }
        else if(objName == QString(Constants::HIDE_TOOL_ID))
        {
            setToolWidgetState();
        }
        else if(objName == QString(Constants::HIDE_WORKSPACE_ID))
        {
            setWorkspaceState();
        }
    }
    else
    {
        if(objName == QString(Constants::HIDE_TOOL_ID))
        {
            setSimulateWidgetState();
        }
    }
}

void MainWindow::setIconWidgetState()
{
    SplitManager::instance()->split(QString(Constants::HIDE_ICON_ID))->setContainerVisible();
    if(SplitManager::instance()->split(QString(Constants::HIDE_ICON_ID))->getContainer()->isVisible())
    {
        ActionManager::instance()->action(Constants::HIDE_ICON_ID)->setText("隐藏控件区");
    }
    else
    {
        ActionManager::instance()->action(Constants::HIDE_ICON_ID)->setText("显示控件区");
    }
}

void MainWindow::setToolWidgetState(bool flag)
{
    if(flag)
    {
        SplitManager::instance()->split(QString(Constants::HIDE_TOOL_ID))->setContainerVisible();
    }

    if(SplitManager::instance()->split(QString(Constants::HIDE_TOOL_ID))->getContainer()->isVisible())
    {
        ActionManager::instance()->action(Constants::HIDE_TOOL_ID)->setText("隐藏属性编辑区");
    }
    else
    {
        ActionManager::instance()->action(Constants::HIDE_TOOL_ID)->setText("显示属性编辑区");
    }
}

void MainWindow::setWorkspaceState()
{
    bool isVisible = MyPageSwitch::instance()->isVisible();
    MyPageSwitch::instance()->setVisible(!isVisible);
    if(MyPageSwitch::instance()->isVisible())
    {
        ActionManager::instance()->action(Constants::HIDE_WORKSPACE_ID)->setText("隐藏工作区");
    }
    else
    {
        ActionManager::instance()->action(Constants::HIDE_WORKSPACE_ID)->setText("显示工作区");
    }
}

void MainWindow::setSimulateWidgetState(bool flag)
{
    if(flag)
    {
        SplitManager::instance()->split(QString(Constants::HIDE_SIMULATE_ID))->setContainerVisible();
    }

    if(SplitManager::instance()->split(QString(Constants::HIDE_SIMULATE_ID))->getContainer()->isVisible())
    {
        ActionManager::instance()->action(Constants::HIDE_TOOL_ID)->setText("隐藏推演区");
    }
    else
    {
        ActionManager::instance()->action(Constants::HIDE_TOOL_ID)->setText("显示推演区");
    }
}

//获取技术支持
void MainWindow::getTecSupport()
{

}

//显示关于
void MainWindow::showAbout()
{
    AboutSoft dialog(this);
    dialog.exec();
//    qApp->aboutQt();
}

//切换工作模式【建模/推演】
void MainWindow::switchWorkModel()
{
    QString objName = QObject::sender()->objectName();
    if(GlobalIsSimulateState)
    {
        if(objName == QString(Constants::SIMLUATE_ID))
        {
            return;
        }
        ActionManager::instance()->action(Constants::SIMLUATE_ID)->setChecked(true);

//        Util::showWarn("推演尚未结束，无法切换至建模状态!");
        return;
    }
    bool enable = false;

    MyGraphicsView::instance()->respResetSimluate();

    if(objName == QString(Constants::BUILD_MODEL_ID))
    {
        enable = true;
        GlobalWindowState = WINDOW_BUILD_MODEL;
        setWindowTitle("多组件模型在线协同调用工具-建模");
        setToolWidgetState(false);
    }
    else
    {
        GlobalWindowState = WINDOW_SIMULATE;
        setWindowTitle("多组件模型在线协同调用工具-推演");
        setSimulateWidgetState(false);
        simulatePanel->resetSimluateFlag();
        MyGraphicsView::instance()->resetItemProcessType();
    }

    MyGraphicsView::instance()->addContextMenuItem();

    SplitManager::instance()->split(QString(Constants::HIDE_ICON_ID))->setVisible(enable);
    SplitManager::instance()->split(QString(Constants::HIDE_TOOL_ID))->setVisible(enable);
    SplitManager::instance()->split(QString(Constants::HIDE_SIMULATE_ID))->setVisible(!enable);
    MyPageSwitch::instance()->setVisible(enable);

    if(!fileBar)
    {
        return;
    }

    MenuManager::instance()->menu(Constants::MENUBAR_FILE_MENU)->setEnabled(enable);
    MenuManager::instance()->menu(Constants::MENUBAR_EDIT_MENU)->setEnabled(enable);
    MenuManager::instance()->menu(Constants::MENUBAR_ITEM_MENU)->setEnabled(enable);

    fileBar->setEnabled(enable);
    itemBar->setEnabled(enable);
    editBar->setEnabled(enable);
    databaseBar->setEnabled(!enable);
}

//打开本地保存的文件，会先提示是否要保存当前添加的控件
void MainWindow::fileOpen()
{
    MY_BUILD_MODEL_ONLY

    respRestItemAction();

    MY_ASSERT(MyGraphicsView::instance()->scene())

    fileClear();

    QString openFileName = QFileDialog::getOpenFileName(this,"选择打开文件","","Files(*"+SaveFileSuffix+")");
    if(!openFileName.isEmpty())
    {
        MyGraphicsView::instance()->openLocalFile(openFileName);
    }
}

//清空当前的控件
void MainWindow::fileClear()
{
    MY_BUILD_MODEL_ONLY

    respRestItemAction();

    if(view->getItemSize() > 1)
    {
        int result = Util::getWarnChoice("是否清空场景的内容?");
        if(result == QMessageBox::Yes)
        {
            view->clearItems();
            view->respItemSizeChanged(0);
            resetEditActionState(false);
            Util::resetGlobalZValue();
        }
    }
}

//获取点击控件的类型
void MainWindow::recordClickedItem()
{
    MyAction * tmpAction = dynamic_cast<MyAction *>(QObject::sender());
    if(tmpAction)
    {
        CurrAddGraType = tmpAction->getType();
    }
}

//页面删除后，进行相关状态切换【控件栏、右侧控件】
void MainWindow::respDeletePage()
{
    if(MyGraphicsView::instance()->scene() == NULL)
    {
        ActionManager::instance()->action(Constants::CLEAR_ID)->setEnabled(false);
        ActionManager::instance()->action(Constants::SAVE_ID)->setEnabled(false);
    }

    rightToolBox->enableButtState(false);
}

//创建场景和视图
void MainWindow::createSceneAndView()
{
    QWidget * centralWidget = new QWidget;
    QHBoxLayout * layout = new QHBoxLayout;
    layout->setContentsMargins(1,1,1,1);
    layout->setSpacing(LAYOUT_SPACING);

    QVBoxLayout * vLayout = new QVBoxLayout;
    vLayout->setContentsMargins(1,1,1,1);
    view = new MyGraphicsView(this);
    connect(MyProWizard::instance(),SIGNAL(proInfo(QString,QString)),MyGraphicsView::instance(),SLOT(addPage(QString,QString)));

    vLayout->addWidget(MyPageSwitch::instance());
    vLayout->addWidget(view);

    connect(MyPageSwitch::instance(),SIGNAL(switchPage()),MyGraphicsView::instance(),SLOT(setToolBarState()));
    connect(MyPageSwitch::instance(),SIGNAL(deletePage()),this,SLOT(respDeletePage()));

    leftIconWidget = new LeftIconWidget;

    rightToolBox = new RightToolBox;
    connect(MyGraphicsView::instance(),SIGNAL(initToolBox(int,ItemProperty)),rightToolBox,SLOT(respInitToolBox(int,ItemProperty)));
    connect(rightToolBox,SIGNAL(updateProperty(ItemProperty)),MyGraphicsView::instance(),SLOT(respPropertyUpdate(ItemProperty)));
    connect(rightToolBox,SIGNAL(deleteCurrItem()),MyGraphicsView::instance(),SLOT(deleteItem()));
    connect(MyGraphicsView::instance(),SIGNAL(itemPropChanged(ItemProperty)),rightToolBox,SLOT(respItemPropChanged(ItemProperty)));

    simulatePanel = new SimulateControlPanel;
    connect(simulatePanel,SIGNAL(resetSimluate()),view,SLOT(respResetSimluate()));

    layout->addWidget(SplitManager::instance()->addSplit(QString(Constants::HIDE_ICON_ID),SPLIT_RIGHT,leftIconWidget));
    layout->addLayout(vLayout);
    layout->addWidget(SplitManager::instance()->addSplit(QString(Constants::HIDE_TOOL_ID),SPLIT_LEFT,rightToolBox));
    layout->addWidget(SplitManager::instance()->addSplit(QString(Constants::HIDE_SIMULATE_ID),SPLIT_LEFT,simulatePanel));
    centralWidget->setLayout(layout);

    SplitManager::instance()->split(QString(Constants::HIDE_ICON_ID))->setFixedWidth(162);
    SplitManager::instance()->split(QString(Constants::HIDE_TOOL_ID))->setFixedWidth(310);
    SplitManager::instance()->split(QString(Constants::HIDE_SIMULATE_ID))->setFixedWidth(310);

    this->setCentralWidget(centralWidget);
}

//重置编辑action的状态
void MainWindow::resetEditActionState(bool state)
{
    ActionManager::instance()->action(Constants::CUT_ID)->setEnabled(state);
    ActionManager::instance()->action(Constants::COPY_ID)->setEnabled(state);
    ActionManager::instance()->action(Constants::PASTE_ID)->setEnabled(state);

    ActionManager::instance()->action(Constants::ROTATE_LEFT_ID)->setEnabled(state);
    ActionManager::instance()->action(Constants::ROTATE_RIGHT_ID)->setEnabled(state);
    ActionManager::instance()->action(Constants::BRING_FRONT_ID)->setEnabled(state);
    ActionManager::instance()->action(Constants::BRING_BACK_ID)->setEnabled(state);
    ActionManager::instance()->action(Constants::DELETE_ID)->setEnabled(state);
}

//当在scene中右击时，将item工具栏中的状态恢复至箭头状态
void MainWindow::respRestItemAction()
{
    ActionManager::instance()->action(Constants::ARROW_ID)->setChecked(true);
}

//创建线条自定义下拉框，如果不需要使用文字，那么直接添加NULL
void MainWindow::createLineComboBox()
{
    MyLineComboBox * startLineBox = ComboBoxManager::instance()->addItem(Constants::LEFT_LINE_ID);
    MyLineComboBox * endLineBox = ComboBoxManager::instance()->addItem(Constants::RIGHT_LINE_ID);

    startLineBox->addItem(QIcon(":/images/arrow/leftLine.png"),NULL);
    startLineBox->addItem(QIcon(":/images/arrow/leftArrow.png"),NULL);
    startLineBox->addItem(QIcon(":/images/arrow/leftSolidTriangle.png"),NULL);

    endLineBox->addItem(QIcon(":/images/arrow/rightLine.png"),NULL);
    endLineBox->addItem(QIcon(":/images/arrow/rightArrow.png"),NULL);
    endLineBox->addItem(QIcon(":/images/arrow/rightSolidTriangle.png"),NULL);

    connect(startLineBox,SIGNAL(indexChanged(int)),MyGraphicsView::instance(),SLOT(setSelectedLineType(int)));
    connect(endLineBox,SIGNAL(indexChanged(int)),MyGraphicsView::instance(),SLOT(setSelectedLineType(int)));
    startLineBox->setEnabled(false);
    endLineBox->setEnabled(false);
}

//创建工具栏
void MainWindow::createToolBar()
{
    QToolBar * modelBar = addToolBar("Model");
    modelBar->addAction(ActionManager::instance()->action(Constants::BUILD_MODEL_ID));
    modelBar->addAction(ActionManager::instance()->action(Constants::SIMLUATE_ID));

    fileBar = addToolBar("File");
    fileBar->addAction(ActionManager::instance()->action(Constants::FILE_ID));
    fileBar->addAction(ActionManager::instance()->action(Constants::OPEN_ID));
    fileBar->addAction(ActionManager::instance()->action(Constants::SAVE_ID));
    fileBar->addAction(ActionManager::instance()->action(Constants::CLEAR_ID));

    itemBar = addToolBar("Item");
    itemBar->addAction(ActionManager::instance()->action(Constants::ARROW_ID));
    itemBar->addSeparator();
    itemBar->addAction(ActionManager::instance()->action(Constants::TEXT_ID));
    itemBar->addAction(ActionManager::instance()->action(Constants::LINE_ID));
    itemBar->addAction(ActionManager::instance()->action(Constants::VECTOR_LINE_ID));

    editBar = addToolBar("Edit");
    editBar->addAction(ActionManager::instance()->action(Constants::UNDO_ID));
    editBar->addAction(ActionManager::instance()->action(Constants::REDO_ID));
    editBar->addAction(ActionManager::instance()->action(Constants::CUT_ID));
    editBar->addAction(ActionManager::instance()->action(Constants::COPY_ID));
    editBar->addAction(ActionManager::instance()->action(Constants::PASTE_ID));
    editBar->addAction(ActionManager::instance()->action(Constants::CLEAR_PASTE_ID));
    editBar->addAction(ActionManager::instance()->action(Constants::ROTATE_LEFT_ID));
    editBar->addAction(ActionManager::instance()->action(Constants::ROTATE_RIGHT_ID));
    editBar->addAction(ActionManager::instance()->action(Constants::BRING_FRONT_ID));
    editBar->addAction(ActionManager::instance()->action(Constants::BRING_BACK_ID));
    editBar->addAction(ActionManager::instance()->action(Constants::LOCK_ID));
    editBar->addAction(ActionManager::instance()->action(Constants::UNLOCK_ID));
    editBar->addWidget(ComboBoxManager::instance()->item(Constants::LEFT_LINE_ID));
    editBar->addWidget(ComboBoxManager::instance()->item(Constants::RIGHT_LINE_ID));
    editBar->addAction(ActionManager::instance()->action(Constants::DELETE_ID));

    sceneBar = addToolBar("View");

    mySlider = new MySlider;
    connect(mySlider,SIGNAL(scaleView(int)),MyGraphicsView::instance(),SLOT(sceneScaled(int)));
    connect(view,SIGNAL(scaleValue(int)),mySlider,SLOT(respSetScaleValue(int)));
    connect(view,SIGNAL(zoomIn()),mySlider,SLOT(respZoomIn()));
    connect(view,SIGNAL(zoomOut()),mySlider,SLOT(respZoomOut()));

    sceneBar->addAction(ActionManager::instance()->action(Constants::DRAG_ABLE_ID));
    sceneBar->addSeparator();
    sceneBar->addWidget(mySlider);

    databaseBar = addToolBar("Database");
    databaseBar->addAction(ActionManager::instance()->action(Constants::DATABASE_VIEW));
    databaseBar->addAction(ActionManager::instance()->action(Constants::DATABASE_REFRESH));
}

//创建状态栏
void MainWindow::createStatusBar()
{
    QStatusBar * statusBar = new QStatusBar;
    setStatusBar(statusBar);
}

//显示状态信息
void MainWindow::respShowStatusInfo(QString text)
{
    statusBar()->showMessage(text);
}

MainWindow::~MainWindow()
{
    if(mySlider)
    {
        delete mySlider;
        mySlider = NULL;
    }

    if(leftIconWidget)
    {
        delete leftIconWidget;
        leftIconWidget = NULL;
    }

    if(view)
    {
        delete view;
        view = NULL;
    }

    foreach(ServiceProperty * prop,GlobalServiceProperties)
    {
        foreach (Parameter * para, prop->inputParas)
        {
            delete para;
        }
        prop->inputParas.clear();

        foreach (Parameter * para, prop->outputParas)
        {
            delete para;
        }
        prop->outputParas.clear();

        delete prop;
    }

    delete ui;
}
