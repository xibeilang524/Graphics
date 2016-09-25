/*************************************************
**版  权：RenGu Company
**文件名: hidesplit.h
**作  者: wey       Version: 1.0       Date: 2016.09.25
**描  述:
**Others:右侧面板隐藏显示控制窗口
**
**修改历史:
**
*************************************************/
#ifndef HIDESPLIT_H
#define HIDESPLIT_H

#include <QWidget>

#include "../Header.h"

namespace Ui {
class HideSplit;
}

class HideSplit : public QWidget
{
    Q_OBJECT
    
public:
    explicit HideSplit(SpiltDirect direct = SPLIT_LEFT,QWidget *parent = 0);

    static HideSplit *addWidget(SpiltDirect direct,QWidget * widget);

    void setFixedWidth(int w);

    void addWidget(QWidget * widget);
    QWidget * getContainer();

    ~HideSplit();

private slots:
    void setContainerVisible();
    
private:
    void init();

    Ui::HideSplit *ui;
    SpiltDirect splitDirect;        //隐藏工具栏和容器的位置关系
    int fixWidth;

};

#endif // HIDESPLIT_H
