/*************************************************
**版  权：RenGu Company
**文件名: mypageitemscrollarea.h
**作  者: wey       Version: 1.0       Date: 2016.11.06
**描  述: 自定义的滑动滚动窗口
**Others:
**
**修改历史:
**
*************************************************/
#ifndef MYSCROLLAREA_H
#define MYSCROLLAREA_H

#include <QWidget>

class QHBoxLayout;

namespace Ui {
class MyScrollArea;
}

class MyScrollArea : public QWidget
{
    Q_OBJECT
    
public:
    explicit MyScrollArea(QWidget *parent = 0);
    ~MyScrollArea();

    void resizeWidgetPos();

    void insertWidget(int index,QWidget * item);
    
protected:
    void resizeEvent(QResizeEvent *);

private:
    Ui::MyScrollArea *ui;
    QWidget * widget;

    QHBoxLayout * layout;
    QWidget * itemContainer;
};

#endif // MYSCROLLAREA_H
