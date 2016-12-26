/*************************************************
**版  权：RenGu Company
**文件名: mygraphicsview.h
**作  者: wey       Version: 1.0       Date: 2016.09.18
**描  述:
**Others:
**
**修改历史:
**20160919:wey:添加单例模式
**             添加端口编辑页面
**20160920:wey:修复拖入端口放置在控件上无效问题
**20160922:wey:增加设置view是否拖拽
**             增加清空剪切板功能
**20160923:wey:增加拖入清除控件选择
**             增加Ctrl+滚轮缩小视图
**20160928:wey:增加更新选中线条类型
**20161003:wey:增加控件属性编辑窗口
**20161017: gx:修复点击旋转点后，改变右侧样式属性，所选控件不更改样式问题。
**          gx:修复工具栏左旋导致角度为负问题
**20161018:wey:修复快捷键Ctrl+O打开本地选择文件后，Ctrl键盘释放无法捕获，导致滚轮缩放事件激活
**20161021:wey:修复删除item后，清空和本地保存按钮仍然可用问题
**             修复产生连接线时，粘贴按钮被置为可用问题
**             添加工作区切换后，工具栏依照相应的控件来自动的更改状态
**20161026:wey:修复旋转控件时，因刷新不及时导致线条显示重叠问题
**20161108:wey:添加对状态图支持
**20161109:wey:添加对复合模型组件图支持(双击打开下一层)
**20161110:wey:调增分离控件右击和双击的事件响应
**20161130:wey:增加支持文件拖入功能
**20161201:wey:增加本地打开文件先判断是否已经打开
**20161209:wey:增加删除折线
**20161215:wey:修复创建原子图时自动创建的文件名未添加版本后缀问题
**20161223:wey:增加折线样式设定、设置起始箭头
*************************************************/
#ifndef MYGRAPHICSVIEW_H
#define MYGRAPHICSVIEW_H

#include <QGraphicsView>

#include "../Header.h"

class MyNodePort;
class NodeEditDialog;
class MyScene;
class MainWindow;
class MyItem;

class MyGraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    static MyGraphicsView * instance();

    MyGraphicsView(MainWindow * parent = 0);
    ~MyGraphicsView();

    MyItem * getTopSelectedItem();

    void showNodePortEdit(MyNodePort * nodePort);
    void openLocalFile(QString fileName,bool openFile = false);

    QString getFirstSelectedItem();

    void clearItems();
    void addContextMenuItem();
    void addViewContextMenu();

    MyScene * scene();
    MyScene * addScene(QString id);
    void showScene(MyScene *scene);
    void deleteScene();

    qreal getHorizonalValue();
    qreal getVertiaclValue();
    qreal getScaleValue();
    void  transformView(qreal hValue,qreal value);
    void  setScaleValue(int);
    void  setKeyCtrlStated(bool isPressed){this->isCtrlPressed = isPressed;}

    int  getItemSize();
    void resetItemProcessType();

protected:
    void dragEnterEvent(QDragEnterEvent *event);
    void dragMoveEvent(QDragMoveEvent *event);
    void dropEvent(QDropEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void contextMenuEvent(QContextMenuEvent *event);
    void wheelEvent(QWheelEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);

signals:
    void initToolBox(int selectedNum,ItemProperty property);
    void itemPropChanged(ItemProperty);
    void zoomIn();
    void zoomOut();
    void scaleValue(int);

public slots:
    void sceneScaled(int currScale);
    void respResetSimluate();
    void respItemSizeChanged(int size);
    void showSelectedItemPropEdit(MyItem * item,QVariant userRoleData = "",bool isPanelEditable = true);

private slots:
    void addPage(QString proPath,QString proName);
    void fileSave();
    void fileSaveAs();

    void undoAndRedoItem();
    void cutItem();
    void copyItem();
    void pasteItem();
    void clearPasteItem();

    void rotateItem();
    void bringZItem();
    void lockAndunlockItem();
    void deleteItem();
    void setViewDragEnable(bool enable);
    void respCtrlLockKeyPress();

    void updateActions();
    void editTextItem();
    void respEditText();
    void editPropertyItem();
    void respPropertyUpdate(ItemProperty property);
    void setSelectedLineType(int type);
    void setToolBarState();

private:
    void initView();
    void getSelectedLockState();
    void setSelectedItemLockState(bool flag);

    static MyGraphicsView * viewManager;

    NodeEditDialog * nodeEdit;
    MyScene * myScene;
    MainWindow * parentWindow;
    CutInfo cutTmpInfo;               //保存剪切信息

    QPoint pressPoint,movePoint;
    bool isMoving;
    bool viewIsDragable;              //窗口是否可以平移
    bool isCtrlPressed;               //Crtrl键是否被按下
    int sceneScaleFactor;             //视图缩放因子

    ItemLockState lockState;          //选择控件锁定状态
};

#endif // MYGRAPHICSVIEW_H
