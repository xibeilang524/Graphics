#include "myportoutputdialog.h"
#include "ui_myportoutputdialog.h"

#include "../Header.h"
#include "../global.h"
#include "mychoosebar.h"
#include "myportparameter.h"

#include <QTableWidgetItem>

MyPortOutputDialog::MyPortOutputDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MyPortOutputDialog)
{
    ui->setupUi(this);

    this->setGeometry((ScreenWidth-POP_SUB_DIALOG_WIDTH)/2,(ScreenHeight-POP_SUB_DIALOG_HEIGHT)/2
                      ,POP_SUB_DIALOG_WIDTH,POP_SUB_DIALOG_HEIGHT);
    chooseBar = new MyChooseBar(ui->widget_3);
    chooseBar->setParetWidget(this);

    QHBoxLayout * layout = new QHBoxLayout;
    layout->addWidget(chooseBar);
    layout->setContentsMargins(1,1,1,1);
    ui->widget_3->setLayout(layout);

    connect(chooseBar,SIGNAL(confirmPressed()),this,SLOT(updateProp()));
    connect(ui->addItem,SIGNAL(clicked()),this,SLOT(addNewItem()));
    connect(ui->removeSelectedItem,SIGNAL(clicked()),this,SLOT(removeSelectedItem()));
    connect(ui->removeAllItem,SIGNAL(clicked()),this,SLOT(removeAllItem()));

    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget->setAlternatingRowColors(true);
}

void MyPortOutputDialog::setInOutState(bool isInstate)
{
    ui->portTypeBox->clear();
    QStringList portType;
    if(isInstate)
    {
        setWindowTitle("输入端口设置");
        portType<<"事件类"<<"事件类1";
        ui->portTypeBox->addItems(portType);
    }
    else
    {
        setWindowTitle("输出端口设置");
        portType<<"属性类"<<"属性类2";
        ui->portTypeBox->addItems(portType);
    }
}

//将端口的属性设置到窗口
void MyPortOutputDialog::setProp(StateInOutProperty &prop)
{
    ui->portName->setText(prop.portName);

    this->sprop.portName = prop.portName;
    this->sprop.portType = prop.portType;

    for(int i = 0;i<prop.props.size();i++)
    {
        addRow(prop.props.at(i).portName,prop.props.at(i).portType);
        StatePortProperty pp = {prop.props.at(i).portName,prop.props.at(i).portType};
        this->sprop.props.append(pp);
    }
}

//更新属性
void MyPortOutputDialog::updateProp()
{
    sprop.portName = ui->portName->text();
    sprop.portType = ui->portTypeBox->currentText();
}

//添加新条目
void MyPortOutputDialog::addNewItem()
{
    MyPortParameter parameter;
    parameter.exec();
    QString pname,ptype;
    if(parameter.getResult(pname,ptype))
    {
        if(pname.size() > 0 && ptype.size() > 0)
        {
            StatePortProperty pp = {pname,ptype};
            addRow(pname,ptype);
            this->sprop.props.append(pp);
        }
    }
}

//添加一行
void MyPortOutputDialog::addRow(const QString  pname,const QString ptype)
{
    ui->tableWidget->setRowCount(ui->tableWidget->rowCount()+1);

    QTableWidgetItem * col1 = new QTableWidgetItem();
    col1->setText(pname);

    QTableWidgetItem * col2 = new QTableWidgetItem();
    col2->setText(ptype);

    ui->tableWidget->setItem(ui->tableWidget->rowCount()-1,0,col1);
    ui->tableWidget->setItem(ui->tableWidget->rowCount()-1,1,col2);
}

//移除选中的item
void MyPortOutputDialog::removeSelectedItem()
{
    QList<QTableWidgetItem*> items = ui->tableWidget->selectedItems();
    if(items.size() > 0)
    {
        QList<int> list;
        for(int i = 0; i < items.size(); i++)
        {
            if(!list.contains(items.at(i)->row()))
            {
                list.append(items.at(i)->row());
            }
        }

        if(list.size() > 0)
        {
            qSort(list.begin(),list.end());
            for(int i = list.size()-1; i >=0; i--)
            {
                ui->tableWidget->removeRow(list.at(i));
                this->sprop.props.removeAt(list.at(i));
            }
        }
    }
}

//移除所有item
void MyPortOutputDialog::removeAllItem()
{
    for(int i=ui->tableWidget->rowCount()-1;i>=0;i--)
    {
        ui->tableWidget->removeRow(i);
    }
    this->sprop.props.clear();
}

StateInOutProperty & MyPortOutputDialog::getProp()
{
    return this->sprop;
}

MyPortOutputDialog::~MyPortOutputDialog()
{
    delete ui;
}
