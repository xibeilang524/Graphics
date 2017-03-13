#include "extralprocess.h"
#include "ui_extralprocess.h"

#include "global.h"
#include "mainwindow.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QDateTime>
#include <QDebug>

ExtralProcess * ExtralProcess::extralProcess = NULL;

ExtralProcess * ExtralProcess::instance()
{
    if(!extralProcess)
    {
        extralProcess = new ExtralProcess(GlobalMainWindow);
    }
    return extralProcess;
}

ExtralProcess::ExtralProcess(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ExtralProcess)
{
    ui->setupUi(this);
    this->setWindowTitle("启动外部程序");

    process = NULL;

    ui->start->setEnabled(false);
    ui->stop->setEnabled(false);

    ui->argList->setPlaceholderText(",分隔多个参数");

    connect(ui->localFile,SIGNAL(clicked()),this,SLOT(openLocalFile()));
    connect(ui->start,SIGNAL(clicked()),this,SLOT(startProcess()));
    connect(ui->stop,SIGNAL(clicked()),this,SLOT(stopProcess()));
    connect(ui->clearArg,SIGNAL(clicked()),ui->argList,SLOT(clear()));
}

void ExtralProcess::openLocalFile()
{
    QString fileName = QFileDialog::getOpenFileName(this,"选择可执行程序","/home", "exe (*.exe)");
    if(!fileName.isEmpty())
    {
        ui->filePath->setText(fileName);
        ui->start->setEnabled(true);
        ui->stop->setEnabled(false);
    }
}

void ExtralProcess::startProcess()
{
    if(!process)
    {
        process = new QProcess;
        connect(process,SIGNAL(stateChanged(QProcess::ProcessState)),this,SLOT(respStateChanged(QProcess::ProcessState)));
    }

    QString program = ui->filePath->text();
    QStringList argList = ui->argList->text().split(",");

    process->start(program,argList);
}

//响应不同状态
void ExtralProcess::respStateChanged(QProcess::ProcessState state)
{
    QString fileName = ui->filePath->text();
    fileName = fileName.mid(fileName.lastIndexOf('/')+1);

    switch(state)
    {
        case QProcess::Starting:
                                {
                                    ui->start->setEnabled(false);
                                    ui->stop->setEnabled(false);
                                    addText(fileName +"启动中");
                                    break;
                                }
        case QProcess::Running:
                                {
                                    ui->start->setEnabled(false);
                                    ui->stop->setEnabled(true);
                                    addText(fileName +"启动成功");
                                    break;
                                }
        case QProcess::NotRunning:
                                {
                                    ui->start->setEnabled(true);
                                    ui->stop->setEnabled(false);
                                    addText(fileName +"关闭");
                                    break;
                                }
    }
}

void ExtralProcess::addText(QString info)
{
   QString curTime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
   ui->processInfo->appendPlainText(curTime+":"+info);
}

void ExtralProcess::stopProcess()
{
    if(process)
    {
        process->close();
        delete process;
        process = NULL;
    }
}

ExtralProcess::~ExtralProcess()
{
    delete ui;
}
