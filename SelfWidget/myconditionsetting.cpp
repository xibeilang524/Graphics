#include "myconditionsetting.h"
#include "ui_myconditionsetting.h"

#include "../global.h"
#include "../Header.h"

MyConditionSetting::MyConditionSetting(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MyConditionSetting)
{
    ui->setupUi(this);

    setWindowTitle("ÅÐ¶ÏÌõ¼þ±à¼­");

    setGeometry((ScreenWidth - POP_SIMULATE_DIALOG_WIDTH)/2,(ScreenHeight - POP_SIMULATE_DIALOG_HEIGHT)/2,POP_SIMULATE_DIALOG_WIDTH,POP_SIMULATE_DIALOG_HEIGHT);
}

MyConditionSetting::~MyConditionSetting()
{
    delete ui;
}
