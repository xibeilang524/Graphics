#include "mytextinput.h"
#include "ui_mytextinput.h"

MyTextInput::MyTextInput(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MyTextInput)
{
    ui->setupUi(this);

    setWindowTitle("±à¼­ÎÄ±¾");

    connect(ui->confirm,SIGNAL(clicked()),this,SLOT(close()));
}

void MyTextInput::setTex(QString text)
{
    ui->content->setText(text);
}

QString MyTextInput::getText()
{
    return ui->content->text();
}

MyTextInput::~MyTextInput()
{
    delete ui;
}
