#include "myportoutputdialog.h"
#include "ui_myportoutputdialog.h"

#include "../Header.h"
#include "../global.h"
#include "mychoosebar.h"

MyPortOutputDialog::MyPortOutputDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MyPortOutputDialog)
{
    ui->setupUi(this);
    this->setWindowTitle("Êä³ö¶Ë¿ÚÉèÖÃ");
    this->setGeometry((ScreenWidth-POP_SUB_DIALOG_WIDTH)/2,(ScreenHeight-POP_SUB_DIALOG_HEIGHT)/2
                      ,POP_SUB_DIALOG_WIDTH,POP_SUB_DIALOG_HEIGHT);
    ui->widget_3->setParetWidget(this);
}

MyPortOutputDialog::~MyPortOutputDialog()
{
    delete ui;
}
