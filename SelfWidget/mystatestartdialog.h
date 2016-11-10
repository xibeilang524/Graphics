/*************************************************
**版  权：RenGu Company
**文件名: XXXXX.h
**作  者: wey       Version: 1.0       Date: 2016.XX.XX
**描  述:初始块设置
**Others:
**
**修改历史:
**
*************************************************/
#ifndef MYSTATESTARTDIALOG_H
#define MYSTATESTARTDIALOG_H

#include <QDialog>

namespace Ui {
class MyStateStartDialog;
}

class MyStateStartDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit MyStateStartDialog(QWidget *parent = 0);
    ~MyStateStartDialog();

    void setContent(QString content);
    QString getContent(){return this->content;}

private slots:
    void updateInfo();
    
private:
    Ui::MyStateStartDialog *ui;

    QString content;
};

#endif // MYSTATESTARTDIALOG_H
