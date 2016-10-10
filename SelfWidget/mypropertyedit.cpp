#include "mypropertyedit.h"
#include "ui_mypropertyedit.h"

#include "../global.h"
#include "../Header.h"
#include "../modelview/serviceinputtableview.h"
#include "../modelview/serviceinputmodel.h"

#include <QHBoxLayout>
#include <QDebug>

MyPropertyEdit::MyPropertyEdit(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MyPropertyEdit)
{
    ui->setupUi(this);

    setWindowTitle("服务编辑");

    int width = 550;
    int height = 600;

    setGeometry((ScreenWidth - width)/2,(ScreenHeight - height)/2,width,height);

    inputTableView = new ServiceInputTableView(0,4);

    outputTableView = new ServiceInputTableView(0,4);

    QHBoxLayout * inputLayout = new QHBoxLayout;
    inputLayout->addWidget(inputTableView);
    inputLayout->setContentsMargins(2,2,2,2);
    ui->inputWidget->setLayout(inputLayout);

    QHBoxLayout * outputtLayout = new QHBoxLayout;
    outputtLayout->addWidget(outputTableView);
    outputtLayout->setContentsMargins(2,2,2,2);
    ui->outputWidget->setLayout(outputtLayout);

    currItemProp = NULL;

    connect(ui->confirm,SIGNAL(clicked()),this,SLOT(confirmPropety()));
    connect(ui->cancel,SIGNAL(clicked()),this,SLOT(cancelProperty()));
}

//初始显示信息
void MyPropertyEdit::initProp(ServiceProperty *prop)
{
    currItemProp = prop;
    inputTableView->model()->setPara(prop->inputParas);
    outputTableView->model()->setPara(prop->outputParas);
    ui->serviceName->setText(prop->serviceName);
    ui->servicePath->setText(prop->servicePath);
}

//将数据保存至当前Item属性
void MyPropertyEdit::confirmPropety()
{
    MY_ASSERT(currItemProp)
    currItemProp->serviceName = ui->serviceName->text();
    currItemProp->servicePath = ui->servicePath->text();
    currItemProp->inputParas = inputTableView->model()->getPara();
    currItemProp->outputParas = outputTableView->model()->getPara();

    cancelProperty();
}

//取消
void MyPropertyEdit::cancelProperty()
{
    ui->serviceName->setText("");
    ui->servicePath->setText("");
    this->close();
}

MyPropertyEdit::~MyPropertyEdit()
{
    delete ui;
}
