/*************************************************
**版  权：RenGu Company
**文件名: mystatemodeldialog.h
**作  者: wey       Version: 1.0       Date: 2016.11.08
**描  述:模型块状态设置
**Others:
**
**修改历史:
**
*************************************************/
#ifndef MYSTATEMODELDIALOG_H
#define MYSTATEMODELDIALOG_H

#include <QDialog>

namespace Ui {
class MyStateModelDialog;
}

class MyStateModelDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit MyStateModelDialog(QWidget *parent = 0);
    ~MyStateModelDialog();
    
private slots:
    void respContinueAction();
    void respAddItem();

private:
    Ui::MyStateModelDialog *ui;
};

#endif // MYSTATEMODELDIALOG_H
