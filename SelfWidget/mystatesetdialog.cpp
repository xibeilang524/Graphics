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
    this->setWindowTitle("×´Ì¬ÉèÖÃ");

    this->setGeometry((ScreenWidth-POP_SUB_DIALOG_WIDTH)/2,(ScreenHeight-POP_SUB_DIALOG_HEIGHT)/2
                      ,POP_SUB_DIALOG_WIDTH,POP_SUB_DIALOG_HEIGHT);

    chooseBar = new MyChooseBar(ui->widget_4);
    chooseBar->setParetWidget(this);

    QHBoxLayout * layout = new QHBoxLayout;
    layout->addWidget(chooseBar);
    layout->setContentsMargins(1,1,1,1);
    ui->widget_4->setLayout(layout);
}

MyStateSetDialog::~MyStateSetDialog()
{
    delete ui;
}
