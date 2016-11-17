/*************************************************
**版  权：RenGu Company
**文件名: MyStateSetDialog.h
**作  者: wey       Version: 1.0       Date: 2016.11.08
**描  述: 状态设置
**Others:
**
**修改历史:
**
*************************************************/
#ifndef MYSTATESETDIALOG_H
#define MYSTATESETDIALOG_H

#include <QDialog>
class MyChooseBar;

namespace Ui {
class MyStateSetDialog;
}

class MyStateSetDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit MyStateSetDialog(QWidget *parent = 0);
    ~MyStateSetDialog();
    
private:
    Ui::MyStateSetDialog *ui;
    MyChooseBar * chooseBar;
};

#endif // MYSTATESETDIALOG_H
