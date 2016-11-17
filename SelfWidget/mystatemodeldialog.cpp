#include "mystatemodeldialog.h"
#include "ui_mystatemodeldialog.h"

#include "../Header.h"
#include "../global.h"
#include "mychoosebar.h"
#include "mystatestartdialog.h"
#include "mystateinternaleventdialog.h"

MyStateModelDialog::MyStateModelDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MyStateModelDialog)
{
    ui->setupUi(this);
    this->setWindowTitle("模型块状态设置");

    this->setGeometry((ScreenWidth-POP_SUB_DIALOG_WIDTH)/2,(ScreenHeight-POP_SUB_DIALOG_HEIGHT)/2
                      ,POP_SUB_DIALOG_WIDTH,POP_SUB_DIALOG_HEIGHT);

    chooseBar = new MyChooseBar(ui->widget_4);
    chooseBar->setParetWidget(this);

    QHBoxLayout * layout = new QHBoxLayout;
    layout->addWidget(chooseBar);
    layout->setContentsMargins(1,1,1,1);
    ui->widget_4->setLayout(layout);

    connect(ui->continueAction,SIGNAL(clicked()),this,SLOT(respContinueAction()));
    connect(ui->addItem,SIGNAL(clicked()),this,SLOT(respAddItem()));
    connect(chooseBar,SIGNAL(confirmPressed()),this,SLOT(updateInfo()));
}

void MyStateModelDialog::setModelProp(StateModelProperty &property)
{
    prop.stateName = property.stateName;
    prop.continueContent = property.continueContent;
    continueContent = prop.continueContent;
    prop.props.clear();
    foreach(StatInnerProperty tmpProp,property.props)
    {
        prop.props.append(tmpProp);
    }

    ui->stateName->setText(prop.stateName);
}

void MyStateModelDialog::updateInfo()
{
    prop.stateName = ui->stateName->text();
    prop.continueContent = continueContent;
    prop.props.clear();
    foreach(StatInnerProperty tmpProp,props)
    {
        prop.props.append(tmpProp);
    }
}

//显示连续行为对话框
void MyStateModelDialog::respContinueAction()
{
    MyStateStartDialog dialog(this);
    dialog.setContent(continueContent);
    dialog.exec();
    continueContent = dialog.getContent();
}

//显示状态内部事件（守卫条件）设置
void MyStateModelDialog::respAddItem()
{
    MyStateInternalEventDialog dialog(this);
    dialog.exec();
}

MyStateModelDialog::~MyStateModelDialog()
{
    delete ui;
}
