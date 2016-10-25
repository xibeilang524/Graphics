/*************************************************
**版  权：RenGu Company
**文件名: simulatecontrolpanel.h
**作  者: wey       Version: 1.0       Date: 2016.10.11
**描  述:
**Others:推演控制面板
**
**修改历史:
**20161017:wey:调整模拟界面，增加模拟前对控件状态的恢复
**20161020:wey:增加验证提示，以及流程运行过程
**20161021:wey:增加从推演队列选择对应的处理单元
**20161025:wey:增加从过程列表直接双击显示处理单元属性编辑信息
*************************************************/
#ifndef SIMULATECONTROLPANEL_H
#define SIMULATECONTROLPANEL_H

#include <QWidget>
#include <QListWidgetItem>

class QLabel;
struct ProcessUnit;

namespace Ui {
class SimulateControlPanel;
}

class MyListWidgetItem : public QListWidgetItem
{
public:
    MyListWidgetItem(QListWidget * parent = 0, int type = Type);
    MyListWidgetItem(const QString & text, QListWidget * parent = 0, int type = Type);

    void bindProscessUnit(ProcessUnit *unit);
    ProcessUnit * getUnit(){return this->punit;}

private:
    ProcessUnit * punit;
};

class SimulateControlPanel : public QWidget
{
    Q_OBJECT
    
public:
    explicit SimulateControlPanel(QWidget *parent = 0);
    ~SimulateControlPanel();

signals:
    void resetSimluate();
    void sendSingleSimulate(ProcessUnit * unit);

private slots:
    void respStartSimulate();
    void showSimulateOperate(ProcessUnit * unit);
    void respItemChanged(QListWidgetItem * current, QListWidgetItem * previous);
    void respItemDoubleClicked(QListWidgetItem * current);
    
private:

    void setFlagState(QLabel * label,bool isSuccess);
    void resetSimluateFlag();

    Ui::SimulateControlPanel *ui;
};

#endif // SIMULATECONTROLPANEL_H
