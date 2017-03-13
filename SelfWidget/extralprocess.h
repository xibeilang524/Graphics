/*************************************************
**版  权：RenGu Company
**文件名: extralprocess.h
**作  者: wey       Version: 1.0       Date: 2017.03.13
**描  述:
**Others:外部程序启动窗口，支持路径选择，参数输入
**
**修改历史:
**20170313:wey:添加记录启动过程信息
*************************************************/
#ifndef EXTRALPROCESS_H
#define EXTRALPROCESS_H

#include <QDialog>
#include <QProcess>

namespace Ui {
class ExtralProcess;
}

class QProcess;

class ExtralProcess : public QDialog
{
    Q_OBJECT
    
public:
    explicit ExtralProcess(QWidget *parent = 0);

    static ExtralProcess * instance();
    ~ExtralProcess();

private slots:
    void openLocalFile();
    void startProcess();
    void stopProcess();
    void respStateChanged(QProcess::ProcessState state);
    
private:
    void addText(QString info);

    Ui::ExtralProcess *ui;

    static ExtralProcess * extralProcess;

    QProcess * process;
};

#endif // EXTRALPROCESS_H
