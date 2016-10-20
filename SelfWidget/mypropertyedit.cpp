#include "mypropertyedit.h"
#include "ui_mypropertyedit.h"

#include "../global.h"
#include "../modelview/serviceinputtableview.h"
#include "../modelview/serviceinputmodel.h"
#include "../sql/serviceinfoprocess.h"

#include <QHBoxLayout>
#include <QListView>
#include <QDebug>

MyPropertyEdit::MyPropertyEdit(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MyPropertyEdit)
{
    ui->setupUi(this);

    setWindowTitle("服务编辑");

    int width = 550;
    int height = 600;
    isComboxAutoChanged = false;

    setGeometry((ScreenWidth - width)/2,(ScreenHeight - height)/2,width,height);

    inputTableView = new ServiceInputTableView(0,4);

    outputTableView = new ServiceInputTableView(0,4);

    ui->serviceName->setView(new QListView());

    QHBoxLayout * inputLayout = new QHBoxLayout;
    inputLayout->addWidget(inputTableView);
    inputLayout->setContentsMargins(2,2,2,2);
    ui->inputWidget->setLayout(inputLayout);

    QHBoxLayout * outputtLayout = new QHBoxLayout;
    outputtLayout->addWidget(outputTableView);
    outputtLayout->setContentsMargins(2,2,2,2);
    ui->outputWidget->setLayout(outputtLayout);

    currItemProp = NULL;

    connect(ui->serviceName,SIGNAL(currentIndexChanged(int)),this,SLOT(switchServiceInfo(int)));
    connect(ui->confirm,SIGNAL(clicked()),this,SLOT(confirmPropety()));
    connect(ui->cancel,SIGNAL(clicked()),this,SLOT(cancelProperty()));
}

//初始显示信息
void MyPropertyEdit::initProp(ServiceProperty *prop)
{
    currItemProp = prop;

    if(prop->hasSettInfo)
    {
        inputTableView->model()->setPara(prop->inputParas);
        outputTableView->model()->setPara(prop->outputParas);

        isComboxAutoChanged = true;
        int index = -1;
        for(int i = 0;i<GlobalServiceProperties.size();i++)
        {
            ui->serviceName->addItem(GlobalServiceProperties.at(i).serviceName);
            if(GlobalServiceProperties.at(i).serviceName == prop->serviceName)
            {
                index = i;
            }
        }

        ui->serviceName->setCurrentIndex(index);

        isComboxAutoChanged = false;

        ui->serviceState->setText(prop->status);
        ui->serviceDesc->setText(prop->descirption);
        ui->servicePath->setText(prop->servicePath);
        ui->servicePort->setText(prop->method);
        ui->outputAsNextInput->setChecked(prop->isAsNextInput);
    }
    else
    {
        initServiceData();
    }
}

//从数据库加载数据
void MyPropertyEdit::initServiceData()
{
    if(GlobalServiceProperties.size() == 0)
    {
        ServiceInfoProcess::instance()->getServiceProperty(GlobalServiceProperties);
    }

    foreach(ServiceProperty prop,GlobalServiceProperties)
    {
        ui->serviceName->addItem(prop.serviceName);
    }
}

//切换显示某个属性的详细信息
void MyPropertyEdit::switchServiceInfo(int index)
{
    if(!isComboxAutoChanged && index >= 0 && index < GlobalServiceProperties.size())
    {
        ServiceProperty  prop = GlobalServiceProperties.at(index);
        ui->serviceState->setText(prop.status);
        ui->serviceDesc->setText(prop.descirption);
        ui->servicePath->setText(prop.servicePath);
        ui->servicePort->setText(prop.method);
    }
}

//将数据保存至当前Item属性
void MyPropertyEdit::confirmPropety()
{
    MY_ASSERT(currItemProp)

    currItemProp->hasSettInfo = true;
    currItemProp->serviceName = ui->serviceName->currentText();
    currItemProp->status = ui->serviceState->text();
    currItemProp->descirption = ui->serviceDesc->text();
    currItemProp->servicePath = ui->servicePath->text();
    currItemProp->method = ui->servicePort->text();
    currItemProp->inputParas = inputTableView->model()->getPara();
    currItemProp->outputParas = outputTableView->model()->getPara();

    cancelProperty();
}

//取消
void MyPropertyEdit::cancelProperty()
{
    this->close();
}

MyPropertyEdit::~MyPropertyEdit()
{
    delete ui;
}
