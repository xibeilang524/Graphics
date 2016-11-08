/*************************************************
**版  权：RenGu Company
**文件名: mystateinternaleventdialog.h
**作  者: wey       Version: 1.0       Date: 2016.11.08
**描  述: 状态内部事件设置
**Others:
**
**修改历史:
**
*************************************************/
#ifndef MYSTATEINTERNALEVENTDIALOG_H
#define MYSTATEINTERNALEVENTDIALOG_H

#include <QDialog>

namespace Ui {
class MyStateInternalEventDialog;
}

class MyStateInternalEventDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit MyStateInternalEventDialog(QWidget *parent = 0);
    ~MyStateInternalEventDialog();
    
private:
    Ui::MyStateInternalEventDialog *ui;
};

#endif // MYSTATEINTERNALEVENTDIALOG_H
