#include "mypropertyedit.h"
#include "ui_mypropertyedit.h"

#include "../global.h"

MyPropertyEdit::MyPropertyEdit(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MyPropertyEdit)
{
    ui->setupUi(this);

    setWindowTitle("服务编辑");

    int width = 550;
    int height = 600;

    setGeometry((ScreenWidth - width)/2,(ScreenHeight - height)/2,width,height);

    connect(ui->confirm,SIGNAL(clicked()),this,SLOT(confirmPropety()));
    connect(ui->cancel,SIGNAL(clicked()),this,SLOT(cancelProperty()));
}

//确定
void MyPropertyEdit::confirmPropety()
{

}

//取消
void MyPropertyEdit::cancelProperty()
{

}

MyPropertyEdit::~MyPropertyEdit()
{
    delete ui;
}
