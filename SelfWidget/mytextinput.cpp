#include "mytextinput.h"
#include "ui_mytextinput.h"

MyTextInput::MyTextInput(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MyTextInput)
{
    ui->setupUi(this);

    setWindowTitle("±à¼­Ãû³Æ");

    connect(ui->confirm,SIGNAL(clicked()),this,SLOT(close()));
}

void MyTextInput::setTex(QString text)
{
    ui->content->clear();
    ui->content->setPlainText(text);
}

QString MyTextInput::getText()
{
    return ui->content->toPlainText();
}

MyTextInput::~MyTextInput()
{
    delete ui;
}
