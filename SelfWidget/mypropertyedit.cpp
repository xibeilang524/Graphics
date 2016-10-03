#include "mypropertyedit.h"
#include "ui_mypropertyedit.h"

MyPropertyEdit::MyPropertyEdit(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MyPropertyEdit)
{
    ui->setupUi(this);

    setWindowTitle(" Ù–‘±‡º≠");

}

MyPropertyEdit::~MyPropertyEdit()
{
    delete ui;
}
