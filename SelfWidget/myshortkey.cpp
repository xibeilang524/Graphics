#include "myshortkey.h"
#include "ui_myshortkey.h"

#include <QMap>
#include <QTableWidgetItem>

#include "../util.h"
#include "../global.h"

MyShortKey::MyShortKey(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MyShortKey)
{
    ui->setupUi(this);
    setWindowTitle("快捷键列表");

    ui->tableWidget->verticalHeader()->hide();
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget->setFocusPolicy(Qt::NoFocus);

    //设置表头宽度
    QHeaderView * view = new QHeaderView(Qt::Horizontal,this);
    view->setResizeMode(QHeaderView::Custom);
    ui->tableWidget->setHorizontalHeader(view);

    view->setStretchLastSection(true);
    view->resizeSection(0,180);

    ui->tableWidget->horizontalHeader()->setStyleSheet("QHeaderView::section{color:white;border:none;height:23px;margin-left:1px; \
                                                     background-color:#567dbc;}");

    int height = POP_SIMULATE_DIALOG_HEIGHT + 30;
    setGeometry((ScreenWidth - POP_SIMULATE_DIALOG_WIDTH)/2,(ScreenHeight - height)/2,POP_SIMULATE_DIALOG_WIDTH,height);

    init();
}

void MyShortKey::init()
{
    QMapIterator<QString,QString> iterator(GlobalKeyMap);

    while(iterator.hasNext())
    {
        ui->tableWidget->setRowCount(ui->tableWidget->rowCount() + 1);
        iterator.next();
        QTableWidgetItem * keyItem = new QTableWidgetItem;
        keyItem->setText(iterator.key());

        QTableWidgetItem * valItem = new QTableWidgetItem;
        valItem->setText(iterator.value());

        int row = ui->tableWidget->rowCount();

        keyItem->setTextColor(Qt::black);
        keyItem->setTextAlignment(Qt::AlignCenter);
        valItem->setTextColor(Qt::black);
        valItem->setTextAlignment(Qt::AlignCenter);

        if(row%2==1)
        {
            keyItem->setBackgroundColor(QColor(100,100,100,15));
            valItem->setBackgroundColor(QColor(100,100,100,15));
        }

        ui->tableWidget->setItem(row - 1,0,keyItem);
        ui->tableWidget->setItem(row - 1,1,valItem);
    }
}

MyShortKey::~MyShortKey()
{
    delete ui;
}
