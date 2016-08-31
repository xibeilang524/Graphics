#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QGraphicsView;
class MyScene;
class QActionGroup;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void fileOpen();
    void exitApp();

    void rotateItem();
    void bringZItem();
    void deleteItem();

    void addItem();

    void respRestItemAction();
    
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
};

#endif // MAINWINDOW_H
