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

    ui->widget_4->setParetWidget(this);

    connect(ui->continueAction,SIGNAL(clicked()),this,SLOT(respContinueAction()));
    connect(ui->addItem,SIGNAL(clicked()),this,SLOT(respAddItem()));
}

//显示连续行为对话框
void MyStateModelDialog::respContinueAction()
{
    MyStateStartDialog dialog(this);
    dialog.exec();
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
