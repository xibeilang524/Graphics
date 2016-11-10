#include "myportinitialdialog.h"
#include "ui_myportinitialdialog.h"

#include "../Header.h"
#include "../global.h"
#include "mychoosebar.h"

#include <QDebug>

MyPortInitialDialog::MyPortInitialDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MyPortInitialDialog)
{
    ui->setupUi(this);

    this->setWindowTitle("初始化端口设置");

    this->setGeometry((ScreenWidth-POP_SUB_DIALOG_WIDTH)/2,(ScreenHeight-POP_SUB_DIALOG_HEIGHT)/2
                      ,POP_SUB_DIALOG_WIDTH,POP_SUB_DIALOG_HEIGHT);
    ui->widget_3->setParetWidget(this);
    connect(ui->widget_3,SIGNAL(confirmPressed()),this,SLOT(updateProp()));
}

void MyPortInitialDialog::updateProp()
{
    sprop.portName = ui->portName->text();
    sprop.portType = ui->portType->text();
}

void MyPortInitialDialog::setProp(StatePortProperty &prop)
{
    sprop.portName = prop.portName;
    sprop.portType = prop.portType;
    ui->portName->setText(sprop.portName);
    ui->portType->setText(sprop.portType);
}

StatePortProperty & MyPortInitialDialog::getProp()
{
    return this->sprop;
}

MyPortInitialDialog::~MyPortInitialDialog()
{
    delete ui;
}
