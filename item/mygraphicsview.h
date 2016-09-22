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

signals:
    void initToolBox(int selectedNum,ItemProperty property);
    void itemPropChanged(ItemProperty);

private slots:
    void cutItem();
    void copyItem();
    void pasteItem();

    void rotateItem();
    void bringZItem();
    void lockAndunlockItem();
    void deleteItem();

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

    CutInfo cutTmpInfo;               //保存剪切信息
};

#endif // MYGRAPHICSVIEW_H
