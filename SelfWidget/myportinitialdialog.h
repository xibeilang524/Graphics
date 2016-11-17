/*************************************************
**版  权：RenGu Company
**文件名: myportinitialdialog.h
**作  者: wey       Version: 1.0       Date: 2016.11.08
**描  述: 初始化端口设置窗口
**Others:
**
**修改历史:
**
*************************************************/
#ifndef MYPORTINITIALDIALOG_H
#define MYPORTINITIALDIALOG_H

#include <QDialog>

namespace Ui {
class MyPortInitialDialog;
}

class MyChooseBar;

#include "../Header.h"

class MyPortInitialDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit MyPortInitialDialog(QWidget *parent = 0);
    ~MyPortInitialDialog();

    void setProp(StatePortProperty & prop);
    StatePortProperty & getProp();

private slots:
    void updateProp();
    
private:
    Ui::MyPortInitialDialog *ui;

    StatePortProperty sprop;
    MyChooseBar * chooseBar;
};

#endif // MYPORTINITIALDIALOG_H
