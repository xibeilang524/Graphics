#include "myportparameter.h"
#include "ui_myportparameter.h"

#include <QDialogButtonBox>
#include <QAbstractButton>
#include <QPushButton>
#include <QDebug>

MyPortParameter::MyPortParameter(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MyPortParameter)
{
    ui->setupUi(this);

    connect(ui->buttonBox,SIGNAL(clicked(QAbstractButton*)),this,SLOT(getButton(QAbstractButton*)));
}

void MyPortParameter::getButton(QAbstractButton *butt)
{
    if(ui->buttonBox->button(QDialogButtonBox::Ok)==butt)
    {
        pname = ui->portName->text();
        ptype = ui->portType->text();
        isOkPressed = true;
    }
    else if(ui->buttonBox->button(QDialogButtonBox::Cancel)==butt)
    {
        isOkPressed = false;
    }
}

//获取结果
bool MyPortParameter::getResult(QString &pname, QString &ptype)
{
    pname = this->pname;
    ptype = this->ptype;
    return isOkPressed;
}

//显示相信信息
void MyPortParameter::showDesc(StatePortProperty prop)
{
    ui->portName->setText(prop.portName);
    ui->portType->setText(prop.portType);
}

MyPortParameter::~MyPortParameter()
{
    delete ui;
}
