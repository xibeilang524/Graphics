/*************************************************
**版  权：RenGu Company
**文件名: mypropertyedit.h
**作  者: wey       Version: 1.0       Date: 2016.10.03
**描  述:
**Others: 控件属性编辑窗口
**
**修改历史:
**
*************************************************/
#ifndef MYPROPERTYEDIT_H
#define MYPROPERTYEDIT_H

#include <QDialog>

namespace Ui {
class MyPropertyEdit;
}

class MyPropertyEdit : public QDialog
{
    Q_OBJECT
    
public:
    explicit MyPropertyEdit(QWidget *parent = 0);
    ~MyPropertyEdit();
    
private:
    Ui::MyPropertyEdit *ui;
};

#endif // MYPROPERTYEDIT_H
