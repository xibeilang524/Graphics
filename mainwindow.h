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
**20160909:wey:添加控件的保存、打开(二进制文件)、增加文件头判断
**             修复选中某个item，编辑完文字后，点击右侧工具栏，文字消失问题。
**             增加对深度(Z方向值)的保存和解析
**20160913:wey:修复左右旋转90度导致旋转出错问题
**20160919:wey:剪切、复制增加对端口功能
**20160920:wey:调整工具栏，将控件点击创建统一改成拖入创建
**             修复删除nodeport线条奔溃问题
**20160922:wey:添加控件、端口的锁定/解锁功能
**             对MainWindow和MyGraphicsView进行了重构，使得两者功能相对独立【！！！】
**20160925:wey:增加左右工具栏可显示隐藏
**             增加控件和帮助菜单栏
*************************************************/
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "Header.h"

class MyGraphicsView;
class QActionGroup;
class MySlider;
class RightToolBox;
class LeftIconWidget;
class HideSplit;

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
    void updateProperty(ItemProperty property);

protected:
    void keyPressEvent(QKeyEvent *event);

private slots:
    void fileNew();
    void fileOpen();
    void fileSave();
    void fileClear();
    void exitApp();
    void recordClickedItem();
    void respRestItemAction();
    void respItemSizeChanged(int size);
    void switchFullScreen();
    void hideSubWidget();
    void getTecSupport();
    void showAbout();

private:
    void createActionAndMenus();
    void createSceneAndView();
    void createToolBar();
    void createStatusBar();
    void createLineComboBox();
    void respShowStatusInfo(QString);
    void resetEditActionState(bool state);

    Ui::MainWindow *ui;
    QActionGroup * itemGroup;

    MySlider * mySlider;
    MyGraphicsView * view;
    LeftIconWidget * leftIconWidget;
    RightToolBox * rightToolBox;

    CutInfo cutTmpInfo;               //保存剪切信息
};

#endif // MAINWINDOW_H
