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

    connect(ui->widget_3,SIGNAL(confirmPressed()),this,SLOT(updateInfo()));
}

void MyStateStartDialog::updateInfo()
{
    content = ui->plainTextEdit->toPlainText();
}

void MyStateStartDialog::setContent(QString content)
{
    ui->plainTextEdit->clear();
    ui->plainTextEdit->setPlainText(content);
}

MyStateStartDialog::~MyStateStartDialog()
{
    delete ui;
}
