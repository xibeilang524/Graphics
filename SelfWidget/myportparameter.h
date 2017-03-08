/*************************************************
**版  权：RenGu Company
**文件名: myportparameter.h
**作  者: wey       Version: 1.0       Date: 2017.03.08
**描  述: 输入端口参数输入
**Others:
**
**修改历史:
**
*************************************************/
#ifndef MYPORTPARAMETER_H
#define MYPORTPARAMETER_H

#include <QDialog>

class QAbstractButton;

namespace Ui {
class MyPortParameter;
}

class MyPortParameter : public QDialog
{
    Q_OBJECT
    
public:
    explicit MyPortParameter(QWidget *parent = 0);
    bool getResult(QString &pname,QString &ptype);

    ~MyPortParameter();

private slots:
    void getButton(QAbstractButton* butt);
    
private:
    Ui::MyPortParameter *ui;

    QString pname,ptype;
    bool isOkPressed;
};

#endif // MYPORTPARAMETER_H
