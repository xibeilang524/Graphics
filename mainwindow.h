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
**20161003:wey:添加节点属性编辑
**20161011:wey:添加工作区类型切换，建模/推演类型
**20161017:wey:添加快捷键Ctrl+M/Ctrl+Shift+M快速切换工作类型
**20161018:wey:修复切换工作模式后，显示/隐藏右侧推演栏无效问题
**20161020:wey:添加关闭提示窗口
**             添加程序图标
**20161026:wey:修复工作区切换键盘事件多次响应问题
**20161115:wey:添加另存为功能
**20161118:wey:修复推演模式下切换至建模状态，建模按钮可以切换的问题。
**20161201:wey:增加切换工作区后，窗口显示工作区信息
**             增加快捷键列表显示
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
class SimulateControlPanel;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    void keyPress(QKeyEvent * event);
    void respShowStatusInfo(QString);
    ~MainWindow();

signals:
    void updateProperty(ItemProperty property);

protected:
    void keyPressEvent(QKeyEvent *event);
    void closeEvent(QCloseEvent *event);

private slots:
    void fileOpen();
    void fileClear();
    void recordClickedItem();
    void respRestItemAction();
    void switchFullScreen();
    void hideSubWidget();
    void getTecSupport();
    void showAbout();
    void respDeletePage();
    void switchWorkModel();
    void updateWindowTitle();
    void showAssisantList();

private:
    void createActionAndMenus();
    void createSceneAndView();
    void createToolBar();
    void createStatusBar();
    void createLineComboBox();
    void resetEditActionState(bool state);
    void setIconWidgetState();
    void setToolWidgetState(bool flag = true);
    void setWorkspaceState();
    void setSimulateWidgetState(bool flag = true);

    Ui::MainWindow *ui;
    QActionGroup * itemGroup;
    QActionGroup * workModelGroup;

    MySlider * mySlider;
    MyGraphicsView * view;
    LeftIconWidget * leftIconWidget;
    RightToolBox * rightToolBox;
    SimulateControlPanel * simulatePanel;

    QToolBar * fileBar;
    QToolBar * itemBar;
    QToolBar * editBar;
    QToolBar * sceneBar;
    QToolBar * databaseBar;

    CutInfo cutTmpInfo;               //保存剪切信息

    QStringList windowTitles;         //窗口标题
    QString currWorkspaceFullPath;    //当前工作区全路径
};

#endif // MAINWINDOW_H
