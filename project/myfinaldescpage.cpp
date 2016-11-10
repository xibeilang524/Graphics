#include "myfinaldescpage.h"
#include "ui_myfinaldescpage.h"

MyFinalDescPage::MyFinalDescPage(QWidget *parent) :
    QWizardPage(parent),
    ui(new Ui::MyFinalDescPage)
{
    ui->setupUi(this);

    setTitle("信息汇总");
}

//初始化界面
void MyFinalDescPage::initializePage()
{
    ui->textBrowser->setText(field("proPath").toString()+"\\"+field("proName").toString());
}

MyFinalDescPage::~MyFinalDescPage()
{
    delete ui;
}
