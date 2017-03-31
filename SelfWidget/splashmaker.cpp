#include "splashmaker.h"
#include "ui_splashmaker.h"

#include <QDebug>
#include <QInputDialog>

#include "global.h"
#include "mainwindow.h"
#include "mysettings.h"

SplashMaker * SplashMaker::splashMaker = NULL;

SplashMaker * SplashMaker::instance()
{
    if(!splashMaker)
    {
        splashMaker = new SplashMaker(GlobalMainWindow);
    }
    return splashMaker;
}

SplashMaker::SplashMaker(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SplashMaker)
{
    ui->setupUi(this);

    setWindowTitle("启动信息设置");
    setFixedWidth(ScreenWidth * 0.25);

    settings = GlobalMainWindow->globalSettings();

    ui->listWidget->setSelectionMode(QAbstractItemView::SingleSelection);

    connect(ui->addContent,SIGNAL(clicked()),this,SLOT(addContent()));
    connect(ui->removeContent,SIGNAL(clicked()),this,SLOT(removeContent()));
    connect(ui->listWidget,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(modifyCurrText(QModelIndex)));
}

void SplashMaker::execMe()
{
    readSettings();
    exec();
}

void SplashMaker::readSettings()
{
    settings->readSplashSettings(splashlist);
    for(int i = 0; i < splashlist.size(); i++)
    {
        ui->listWidget->addItem(splashlist.at(i)->splashContent);
    }
}

void SplashMaker::writeSettings()
{
    settings->writeSplashSettings(splashlist);
}

void SplashMaker::addContent()
{
    bool ok;
    QString text = QInputDialog::getText(this, "添加提示信息",
                                              "内  容:", QLineEdit::Normal,
                                                "", &ok);
   if (ok && !text.isEmpty())
   {
       SplashDesc * desc = new SplashDesc;
       desc->splashContent = text;
       splashlist.append(desc);
       ui->listWidget->addItem(text);
       writeSettings();
   }
}

void SplashMaker::removeContent()
{
    QList<QListWidgetItem *> items = ui->listWidget->selectedItems();
    for(int i = items.size() - 1; i>=0; i--)
    {
        int row = ui->listWidget->row(items.at(i));
        ui->listWidget->takeItem(row);
        delete splashlist.at(row);
        splashlist.removeAt(row);
    }
    writeSettings();
}

void SplashMaker::modifyCurrText(QModelIndex index)
{
    if(!index.isValid())
        return;
    bool ok;
    QString text = QInputDialog::getText(this, "修改信息",
                                               "内  容:", QLineEdit::Normal,
                                                index.data().toString(), &ok);
    if (ok && !text.isEmpty())
    {
        ui->listWidget->item(index.row())->setData(Qt::DisplayRole,text);
        splashlist.at(index.row())->splashContent = text;
        writeSettings();
    }
}

SplashMaker::~SplashMaker()
{
    foreach(SplashDesc * desc,splashlist)
    {
        delete desc;
    }
    splashlist.clear();

    delete ui;
}
