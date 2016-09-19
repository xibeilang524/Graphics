/*************************************************
**版  权：RenGu Company
**文件名: nodeeditdialog.h
**作  者: wey       Version: 1.0       Date: 2016.09.19
**描  述: 用于编辑某个端口
**Others:
**
**修改历史:
**
*************************************************/
#ifndef NODEEDITDIALOG_H
#define NODEEDITDIALOG_H

#include <QDialog>

class MyNodePort;

namespace Ui {
class NodeEditDialog;
}

class NodeEditDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit NodeEditDialog(QWidget *parent = 0);
    void initNodeInfo(MyNodePort * node);
    ~NodeEditDialog();
    
private:
    Ui::NodeEditDialog *ui;

    MyNodePort * nodePort;

};

#endif // NODEEDITDIALOG_H
