/*************************************************
**版  权：RenGu Company
**文件名: aboutsoft.h
**作  者: wey       Version: 1.0       Date: 2016.11.10
**描  述: 关于软件信息
**Others:
**
**修改历史:
**
*************************************************/
#ifndef ABOUTSOFT_H
#define ABOUTSOFT_H

#include <QDialog>

class AboutSoft : public QDialog
{
public:
    explicit AboutSoft(QWidget * parent = 0);

private:
    void initDialog();
};

#endif // ABOUTSOFT_H
