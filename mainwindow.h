/*************************************************
**版  权：RenGu Company
**文件名: mainwindow.h
**作  者: wey       Version: 1.0       Date: 2016.09.01
**描  述: 主窗口
**Others:1：提供工具栏，支持不同类型控件的添加
**
**
**修改历史:
**20160908:wey:修复删除只选中线条时奔溃问题
**             添加文件工具栏，包含新建、删除
**             添加文字信息删除
**             添加剪切、复制、粘贴
*************************************************/
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "Header.h"

class QGraphicsView;
class MyScene;
class QActionGroup;
class MySlider;
class RightToolBox;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

signals:
    void initToolBox(int selectedNum,ItemProperty property);
    void updateProperty(ItemProperty property);

private slots:
    void fileNew();
    void fileOpen();
    void fileSave();
    void fileClear();
    void exitApp();

    void cutItem();
    void copyItem();
    void pasteItem();

    void rotateItem();
    void bringZItem();
    void deleteItem();

    void editTextItem();

    void addItem();

    void respRestItemAction();
    void updateActions();
    void sceneScaled(int currScale);
    void respPropertyUpdate(ItemProperty property);
    void respItemSizeChanged(int size);
    
private:
    void createActionAndMenus();
    void createSceneAndView();
    void createContextMenu();
    void createToolBar();

    Ui::MainWindow *ui;

    MyScene * scene;
    QGraphicsView * view;

    QMenu * rightMenu;

    QActionGroup * itemGroup;

    MySlider * mySlider;
    RightToolBox * rightToolBox;

    CutInfo cutTmpInfo;               //保存剪切信息
};

#endif // MAINWINDOW_H
