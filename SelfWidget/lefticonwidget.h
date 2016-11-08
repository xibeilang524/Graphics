/*************************************************
**版  权：RenGu Company
**文件名: lefticonwidget.h
**作  者: wey       Version: 1.0       Date: 2016.09.18
**描  述: 左侧可拖入控件集合
**Others:
**       1.支持可拖拽
**       2.支持分组查看
**修改历史:
**20160927:wey:增加平行四边形
**20160929:wey:增加循环上限/循环下限/并行/注解
**20161027:wey:增加三角和D端口
*************************************************/
#ifndef LEFTICONWIDGET_H
#define LEFTICONWIDGET_H

#include <QWidget>
#include "../Header.h"

class QToolBox;
class MyListWidget;

class LeftIconWidget : public QWidget
{
    Q_OBJECT
public:
    LeftIconWidget(QWidget * parent = 0);
    ~LeftIconWidget();

private:
    void initWidget();
    void initListItems();

    QToolBox * toolBox;
    MyListWidget * polygonWidget;         //矩形、圆形
#ifdef ADD_STATE_MODEL
    MyListWidget * stateWidget;           //状态机
    MyListWidget * maskWidget;            //遮罩
#endif
    MyListWidget * nodeWidget;            //端口等小部件，主要拖入到polygonWidget之上

};

#endif // LEFTICONWIDGET_H
