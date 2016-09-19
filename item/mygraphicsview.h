/*************************************************
**版  权：RenGu Company
**文件名: mygraphicsview.h
**作  者: wey       Version: 1.0       Date: 2016.09.18
**描  述:
**Others:
**
**修改历史:
**
*************************************************/
#ifndef MYGRAPHICSVIEW_H
#define MYGRAPHICSVIEW_H

#include <QGraphicsView>

class MyGraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    MyGraphicsView(QWidget * parent = 0);
    ~MyGraphicsView();
};

#endif // MYGRAPHICSVIEW_H
