#include "myportoutputdialog.h"
#include "ui_myportoutputdialog.h"

#include "../Header.h"
#include "../global.h"
#include "mychoosebar.h"

MyPortOutputDialog::MyPortOutputDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MyPortOutputDialog)
{
    ui->setupUi(this);

    this->setGeometry((ScreenWidth-POP_SUB_DIALOG_WIDTH)/2,(ScreenHeight-POP_SUB_DIALOG_HEIGHT)/2
                      ,POP_SUB_DIALOG_WIDTH,POP_SUB_DIALOG_HEIGHT);
    ui->widget_3->setParetWidget(this);

    connect(ui->widget_3,SIGNAL(confirmPressed()),this,SLOT(updateProp()));
}

void MyPortOutputDialog::setInOutState(bool isInstate)
{
    ui->portTypeBox->clear();
    QStringList portType;
    if(isInstate)
    {
        setWindowTitle("输入端口设置");
        portType<<"事件类"<<"事件类1";
        ui->portTypeBox->addItems(portType);
    }
    else
    {
        setWindowTitle("输出端口设置");
        portType<<"属性类"<<"属性类2";
        ui->portTypeBox->addItems(portType);
    }
}

void MyPortOutputDialog::setProp(StateInOutProperty &prop)
{
    ui->portName->setText(prop.portName);
}

//更新属性
void MyPortOutputDialog::updateProp()
{
    sprop.portName = ui->portName->text();
    sprop.portType = ui->portTypeBox->currentText();
}

StateInOutProperty & MyPortOutputDialog::getProp()
{
    return this->sprop;
}

MyPortOutputDialog::~MyPortOutputDialog()
{
    delete ui;
}
