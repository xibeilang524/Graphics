#include "mystatesetdialog.h"
#include "ui_mystatesetdialog.h"

#include "../Header.h"
#include "../global.h"
#include "mychoosebar.h"

MyStateSetDialog::MyStateSetDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MyStateSetDialog)
{
    ui->setupUi(this);
    this->setWindowTitle("状态设置");

    this->setGeometry((ScreenWidth-POP_SUB_DIALOG_WIDTH)/2,(ScreenHeight-POP_SUB_DIALOG_HEIGHT)/2
                      ,POP_SUB_DIALOG_WIDTH,POP_SUB_DIALOG_HEIGHT);

    chooseBar = new MyChooseBar(ui->widget_4);
    chooseBar->setParetWidget(this);

    QHBoxLayout * layout = new QHBoxLayout;
    layout->addWidget(chooseBar);
    layout->setContentsMargins(1,1,1,1);
    ui->widget_4->setLayout(layout);

    connect(ui->internalEvent,SIGNAL(clicked(bool)),this,SLOT(respSetInterEventList(bool)));
    connect(ui->outerEvent,SIGNAL(clicked(bool)),this,SLOT(respSetOuterEventList(bool)));
    connect(chooseBar,SIGNAL(confirmPressed()),this,SLOT(respConfirm()));
}

void MyStateSetDialog::respConfirm()
{
    if(ui->internalEvent->isChecked())
    {
        this->prop.triggerMethod = ui->internalEvent->objectName();
    }
    else if(ui->outerEvent->isChecked())
    {
        this->prop.triggerMethod = ui->outerEvent->objectName();
    }

    this->prop.triggerType = ui->comboBox->currentText();
    this->prop.desc = ui->desc->toPlainText();
    this->prop.hasSettled = true;
}

//内部事件列表
void MyStateSetDialog::respSetInterEventList(bool flag)
{
    if(flag)
    {
        ui->comboBox->clear();
        ui->comboBox->addItems(innerList);
    }
}

//外部事件列表
void MyStateSetDialog::respSetOuterEventList(bool flag)
{
    if(flag)
    {
        ui->comboBox->clear();
        ui->comboBox->addItems(outerList);
    }
}

void MyStateSetDialog::initComboxList(QStringList & innerList,QStringList & outerList)
{
    this->innerList = innerList;
    this->outerList = outerList;
}

void MyStateSetDialog::setProp(LinkedStateProperty &linkProp)
{
    this->prop.hasSettled = linkProp.hasSettled;
    this->prop.triggerMethod = linkProp.triggerMethod;
    this->prop.triggerType = linkProp.triggerType;
    this->prop.desc = linkProp.desc;

    if(linkProp.hasSettled)
    {
        //设置状态
        if(linkProp.triggerMethod == ui->internalEvent->objectName())
        {
            ui->internalEvent->setChecked(true);
            ui->outerEvent->setChecked(false);
            respSetInterEventList(true);
        }
        else if(linkProp.triggerMethod == ui->outerEvent->objectName())
        {
            ui->internalEvent->setChecked(false);
            ui->outerEvent->setChecked(true);
            respSetOuterEventList(true);
        }
    }

    for(int i=0;i<ui->comboBox->count();i++)
    {
        if(ui->comboBox->itemText(i)==this->prop.triggerType)
        {
            ui->comboBox->setCurrentIndex(i);
            break;
        }
    }

    ui->desc->setPlainText(linkProp.desc);
}

MyStateSetDialog::~MyStateSetDialog()
{
    delete ui;
}
