/*************************************************
**版  权：RenGu Company
**文件名: mygraphicsview.h
**作  者: wey       Version: 1.0       Date: 2016.09.18
**描  述:
**Others:
**
**修改历史:
**20160919:wey:添加单利模式
**             添加端口编辑页面
**20160920:wey:修复拖入端口放置在控件上无效问题
**20160922:wey:增加设置view是否拖拽
**             增加清空剪切板功能
**20160923:wey:增加拖入清除控件选择
**             增加Ctrl+滚轮缩小视图
*************************************************/
#ifndef MYGRAPHICSVIEW_H
#define MYGRAPHICSVIEW_H

#include <QGraphicsView>

#include "../Header.h"

class MyNodePort;
class NodeEditDialog;
class MyScene;
class MainWindow;
class QMenu;

class MyGraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    static MyGraphicsView * instance();

    MyGraphicsView(MainWindow * parent = 0);
    ~MyGraphicsView();

    void showNodePortEdit(MyNodePort * nodePort);

    void clearItems();
    void addContextMenuItem();

    MyScene * scene();

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

private slots:
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

    void sceneScaled(int currScale);
    void updateActions();
    void editTextItem();
    void respPropertyUpdate(ItemProperty property);

private:
    void initView();

    static MyGraphicsView * viewManager;

    NodeEditDialog * nodeEdit;
    MyScene * myScene;
    MainWindow * parentWindow;
    QMenu * rightMenu;
    QMenu * viewRightMenu;            //窗口的右键菜单

    CutInfo cutTmpInfo;               //保存剪切信息

    QPoint pressPoint,movePoint;
    bool isMoving;
    bool viewIsDragable;              //窗口是否可以平移
    bool isCtrlPressed;               //Crtrl键是否被按下
};

#endif // MYGRAPHICSVIEW_H
