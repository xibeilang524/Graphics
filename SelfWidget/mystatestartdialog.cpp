#include "mystatestartdialog.h"
#include "ui_mystatestartdialog.h"

#include "../Header.h"
#include "../global.h"
#include "mychoosebar.h"

MyStateStartDialog::MyStateStartDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MyStateStartDialog)
{
    ui->setupUi(this);
    this->setWindowTitle("³õÊ¼¿éÉèÖÃ");

    this->setGeometry((ScreenWidth-POP_SUB_DIALOG_WIDTH)/2,(ScreenHeight-POP_SUB_DIALOG_HEIGHT)/2
                      ,POP_SUB_DIALOG_WIDTH,POP_SUB_DIALOG_HEIGHT);

    ui->widget_3->setParetWidget(this);

}

MyStateStartDialog::~MyStateStartDialog()
{
    delete ui;
}
