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
**20161104:wey:增加禁止从正在模拟的状态退出至建模状态
**20161107:wey:修复提取出的处理单元部分判断节点左右子节点为null，导致访问出错问题
**20161110:wey:增加服务推演
**             增加推演过程中引用前者的输出作为自己的输入
**20161114:wey:增加对循环、嵌套循环的支持(目前仅支持一个条件循环判断)
**20161115:wey:修复循环计算--的错误
**             添加自动和单步运行
**20161117:wey:增加循环依照引单计算
**             增加推演高亮提示(用于指示当前推演的进度)
**             增加对循环过程的记录，支持点击右侧记录查看当前循环的值
**20161123:wey:增加判断框条件的词法分析和逻辑运算
**             调整服务返回结果，将结果包装成map集合
**20161130:wey:增加对输入输出框的支持
*************************************************/
#ifndef SIMULATECONTROLPANEL_H
#define SIMULATECONTROLPANEL_H

#include <QWidget>
#include <QListWidgetItem>

#include "../global.h"

class QLabel;
class MyItem;
struct ProcessUnit;
struct ServiceProperty;

namespace Ui {
class SimulateControlPanel;
}

//循环记录
struct LoopRecord
{
    LoopRecord()
    {
        item = NULL;
        record = 0;
    }
    MyItem * item;
    int record;         //循环执行的次数
};

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

    void resetSimluateFlag();

signals:
    void resetSimluate();
    void sendSingleSimulate(ProcessUnit * unit);

private slots:
    void respStartSimulate();
    MyListWidgetItem * showSimulateOperate(ProcessUnit * unit);
    void respItemActivated(QListWidgetItem * current);
    void respItemChanged(QListWidgetItem * current, QListWidgetItem * previous);
    void respItemDoubleClicked(QListWidgetItem * current);
    void procLastUnitResult(bool hasFault, QMap<QString, QString> result);
    void stopCurrSimulate();
    void chooseRunMethod(bool flag);
    void stepByStep();
    
private:
    void setFlagState(QLabel * label,bool isSuccess);
    void setSimulateState(bool isSim);
    void submitUrl(MyItem *item, ServiceProperty * prop);
    void startProcUnit();
    bool countLoopValue(MyItem *item, SignalVariList & loopList);
    void clearLastSimluteRecord();
    QString getQuoteOutValue(MyItem * item,QString value);
    bool countJudgeValue(MyItem *item, QString express);
    QString switchQuoteParameter(MyItem *item, QString & express);
    QString findQuoteResult(MyItem * item, QString quoteServiceName, QString quoteParaName);
    void showCurrProcessResultPanel(bool isPanelEditable);

    Ui::SimulateControlPanel *ui;

    QList<ProcessUnit *> procUnits;

    bool isSimulateState;                   //是否为模拟状态
    bool isAutoRun;                         //是否为自动运行

    ProcessUnit * currProcUnit;             //当前处理单元
    ProcessUnit * beforeUnit;               //之前处理单元(推演结束置为NULL)

    QList<LoopRecord *>  loopRecords;       //记录每个循环的状态信息
};

#endif // SIMULATECONTROLPANEL_H
