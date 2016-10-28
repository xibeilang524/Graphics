#include "myconditionsetting.h"
#include "ui_myconditionsetting.h"

#include "../global.h"

MyConditionSetting::MyConditionSetting(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MyConditionSetting)
{
    ui->setupUi(this);

    setWindowTitle("ÅÐ¶ÏÌõ¼þ±à¼­");

    int width = 550;
    int height = 600;

    setGeometry((ScreenWidth - width)/2,(ScreenHeight - height)/2,width,height);
}

MyConditionSetting::~MyConditionSetting()
{
    delete ui;
}
