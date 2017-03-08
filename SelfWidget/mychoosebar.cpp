#include "mychoosebar.h"
#include "ui_mychoosebar.h"

#include "../Header.h"

MyChooseBar::MyChooseBar(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MyChooseBar)
{
    ui->setupUi(this);

    pWidget = NULL;

    connect(ui->confirm,SIGNAL(clicked()),this,SLOT(respConfirm()));
    connect(ui->cancel,SIGNAL(clicked()),this,SLOT(respCancel()));
}

//响应点击
void MyChooseBar::respConfirm()
{
    MY_ASSERT(pWidget)
    emit confirmPressed();
    pWidget->close();
}

//响应取消
void MyChooseBar::respCancel()
{
    MY_ASSERT(pWidget)
    emit cancelPressed();
    pWidget->close();
}

void MyChooseBar::setParetWidget(QWidget *parent)
{
    pWidget = parent;
}

MyChooseBar::~MyChooseBar()
{
    delete ui;
}
