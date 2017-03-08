#include "mystatemodeldialog.h"
#include "ui_mystatemodeldialog.h"

#include "../Header.h"
#include "../global.h"
#include "mychoosebar.h"
#include "mystatestartdialog.h"
#include "mystateinternaleventdialog.h"

MyStateModelDialog::MyStateModelDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MyStateModelDialog)
{
    ui->setupUi(this);
    this->setWindowTitle("模型块状态设置");

    this->setGeometry((ScreenWidth-POP_SUB_DIALOG_WIDTH)/2,(ScreenHeight-POP_SUB_DIALOG_HEIGHT)/2
                      ,POP_SUB_DIALOG_WIDTH,POP_SUB_DIALOG_HEIGHT);

    chooseBar = new MyChooseBar(ui->widget_4);
    chooseBar->setParetWidget(this);

    QHBoxLayout * layout = new QHBoxLayout;
    layout->addWidget(chooseBar);
    layout->setContentsMargins(1,1,1,1);
    ui->widget_4->setLayout(layout);

    connect(ui->continueAction,SIGNAL(clicked()),this,SLOT(respContinueAction()));
    connect(ui->addItem,SIGNAL(clicked()),this,SLOT(respAddItem()));
    connect(ui->removeAll,SIGNAL(clicked()),this,SLOT(respRemoveAllItem()));
    connect(ui->removeSelectedItem,SIGNAL(clicked()),this,SLOT(respRemoveSelectedItem()));
    connect(chooseBar,SIGNAL(confirmPressed()),this,SLOT(updateInfo()));
    connect(ui->tableWidget,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(respShowCurrItem(QModelIndex)));

    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget->setAlternatingRowColors(true);
}

void MyStateModelDialog::setModelProp(StateModelProperty &property)
{
    prop.stateName = property.stateName;
    prop.continueContent = property.continueContent;
    continueContent = prop.continueContent;

    foreach(StatInnerProperty tmpProp,property.props)
    {
        prop.props.append(tmpProp);
        props.append(tmpProp);
        addRow(tmpProp);
    }

    ui->stateName->setText(prop.stateName);
}

//响应确定按钮，将当前props中的数据更新至prop集合中
void MyStateModelDialog::updateInfo()
{
    prop.stateName = ui->stateName->text();
    prop.continueContent = continueContent;

    prop.props.clear();
    foreach(StatInnerProperty tmpProp,props)
    {
        prop.props.append(tmpProp);
    }
}

//显示连续行为对话框
void MyStateModelDialog::respContinueAction()
{
    MyStateStartDialog dialog(this);
    dialog.setContent(continueContent);
    dialog.exec();
    continueContent = dialog.getContent();
}

//显示状态内部事件（守卫条件）设置
void MyStateModelDialog::respAddItem()
{
    MyStateInternalEventDialog dialog(this);
    dialog.exec();

    StatInnerProperty  pp;
    if(dialog.getResult(pp))
    {
        if(pp.propName.size()>0&&pp.propType.size()>0&&pp.propDesc.size()>0)
        {
            props.append(pp);
            addRow(pp);
        }
    }
}

void MyStateModelDialog::addRow(StatInnerProperty &pp)
{
    ui->tableWidget->setRowCount(ui->tableWidget->rowCount()+1);
    QTableWidgetItem * col1 = new QTableWidgetItem();
    col1->setText(pp.propName);

    ui->tableWidget->setItem(ui->tableWidget->rowCount()-1,0,col1);
}

//移除所有item
void MyStateModelDialog::respRemoveAllItem()
{
    for(int i=ui->tableWidget->rowCount()-1;i>=0;i--)
    {
        ui->tableWidget->removeRow(i);
    }
    props.clear();
}

//移除选中item
void MyStateModelDialog::respRemoveSelectedItem()
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
                props.removeAt(list.at(i));
            }
        }
    }
}

//双击显示选择的数据行
void MyStateModelDialog::respShowCurrItem(QModelIndex index)
{
    if(index.isValid() && index.row()>=0 && index.row() < ui->tableWidget->rowCount())
    {
        MyStateInternalEventDialog dialog(this);
        dialog.showDesc(props.at(index.row()));
        dialog.exec();

        StatInnerProperty  pp;
        if(dialog.getResult(pp))
        {
            if(pp.propName.size()>0&&pp.propType.size()>0&&pp.propDesc.size()>0)
            {
                props.replace(index.row(),pp);
                ui->tableWidget->item(index.row(),0)->setText(pp.propName);
            }
        }
    }
}

MyStateModelDialog::~MyStateModelDialog()
{
    delete ui;
}
