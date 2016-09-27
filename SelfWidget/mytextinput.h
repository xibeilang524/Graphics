/*************************************************
**版  权：RenGu Company
**文件名: mytextinput.h
**作  者: wey       Version: 1.0       Date: 2016.09.20
**描  述:
**Others: 文本编辑框
**
**修改历史:
**20160927:wey:调整lineEdit成plainTextEdit
*************************************************/
#ifndef MYTEXTINPUT_H
#define MYTEXTINPUT_H

#include <QDialog>

namespace Ui {
class MyTextInput;
}

class MyTextInput : public QDialog
{
    Q_OBJECT
    
public:
    explicit MyTextInput(QWidget *parent = 0);
    ~MyTextInput();

    void setTex(QString text);
    QString getText();
    
private:
    Ui::MyTextInput *ui;
};

#endif // MYTEXTINPUT_H
