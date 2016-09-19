#include "nodeeditdialog.h"
#include "ui_nodeeditdialog.h"

#include "../item/mynodeport.h"

NodeEditDialog::NodeEditDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NodeEditDialog)
{
    ui->setupUi(this);

    setWindowTitle("端口属性编辑");

}

//初始化节点信息，为编辑做准备
void NodeEditDialog::initNodeInfo(MyNodePort *node)
{
    this->nodePort = node;
}

NodeEditDialog::~NodeEditDialog()
{
    delete ui;
}
