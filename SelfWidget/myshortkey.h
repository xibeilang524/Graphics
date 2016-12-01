/*************************************************
**版  权：RenGu Company
**文件名: myshortkey.h
**作  者: wey       Version: 1.0       Date: 2016.12.01
**描  述: 显示快捷键
**Others:
**
**修改历史:
**
*************************************************/
#ifndef MYSHORTKEY_H
#define MYSHORTKEY_H

#include <QDialog>

namespace Ui {
class MyShortKey;
}

class MyShortKey : public QDialog
{
    Q_OBJECT
    
public:
    explicit MyShortKey(QWidget *parent = 0);
    ~MyShortKey();
    
private:
    void init();

    Ui::MyShortKey *ui;
};

#endif // MYSHORTKEY_H
