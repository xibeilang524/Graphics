/*************************************************
**版  权：RenGu Company
**文件名: simulatecontrolpanel.h
**作  者: wey       Version: 1.0       Date: 2016.10.11
**描  述:
**Others:推演控制面板
**
**修改历史:
**
*************************************************/
#ifndef SIMULATECONTROLPANEL_H
#define SIMULATECONTROLPANEL_H

#include <QWidget>

namespace Ui {
class SimulateControlPanel;
}

class SimulateControlPanel : public QWidget
{
    Q_OBJECT
    
public:
    explicit SimulateControlPanel(QWidget *parent = 0);
    ~SimulateControlPanel();

private slots:
    void respStartSimulate();
    
private:
    Ui::SimulateControlPanel *ui;
};

#endif // SIMULATECONTROLPANEL_H
