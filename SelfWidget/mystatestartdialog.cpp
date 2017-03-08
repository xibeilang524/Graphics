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

    chooseBar = new MyChooseBar(ui->widget_3);
    chooseBar->setParetWidget(this);

    QHBoxLayout * layout = new QHBoxLayout;
    layout->addWidget(chooseBar);
    layout->setContentsMargins(1,1,1,1);
    ui->widget_3->setLayout(layout);

    connect(chooseBar,SIGNAL(confirmPressed()),this,SLOT(updateInfo()));
}

void MyStateStartDialog::updateInfo()
{
    content = ui->plainTextEdit->toPlainText();
}

void MyStateStartDialog::setContent(QString content)
{
    ui->plainTextEdit->clear();
    ui->plainTextEdit->setPlainText(content);
    this->content = content;
}

MyStateStartDialog::~MyStateStartDialog()
{
    delete ui;
}
