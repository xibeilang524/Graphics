/*************************************************
**版  权：RenGu Company
**文件名: myportoutputdialog.h
**作  者: wey       Version: 1.0       Date: 2016.11.08
**描  述: 输出端口设置
**Others:
**
**修改历史:
**
*************************************************/
#ifndef MYPORTOUTPUTDIALOG_H
#define MYPORTOUTPUTDIALOG_H

#include <QDialog>

namespace Ui {
class MyPortOutputDialog;
}

class MyPortOutputDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit MyPortOutputDialog(QWidget *parent = 0);
    ~MyPortOutputDialog();
    
private:
    Ui::MyPortOutputDialog *ui;
};

#endif // MYPORTOUTPUTDIALOG_H
