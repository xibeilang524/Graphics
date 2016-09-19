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
*************************************************/
#ifndef MYGRAPHICSVIEW_H
#define MYGRAPHICSVIEW_H

#include <QGraphicsView>

class MyNodePort;
class NodeEditDialog;

class MyGraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    static MyGraphicsView * instance();

    MyGraphicsView(QWidget * parent = 0);
    ~MyGraphicsView();

    void showNodePortEdit(MyNodePort * nodePort);

protected:
    void dragEnterEvent(QDragEnterEvent *event);
    void dragMoveEvent(QDragMoveEvent *event);
    void dropEvent(QDropEvent *event);


private:
    static MyGraphicsView * viewManager;

    NodeEditDialog * nodeEdit;
};

#endif // MYGRAPHICSVIEW_H
