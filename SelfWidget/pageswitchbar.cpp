#include "pageswitchbar.h"
#include "ui_pageswitchbar.h"

#include "../assisant/pageinfo.h"

PageSwitchBar::PageSwitchBar(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PageSwitchBar)
{
    ui->setupUi(this);

    pageInfo = new PageInfo;
    setPageShowNum();

    initConnection();
}

void PageSwitchBar::initConnection()
{
    connect(ui->firstPage,SIGNAL(clicked()),this,SLOT(respSwitchPage()));
    connect(ui->prePage,SIGNAL(clicked()),this,SLOT(respSwitchPage()));
    connect(ui->nextPage,SIGNAL(clicked()),this,SLOT(respSwitchPage()));
    connect(ui->lastPage,SIGNAL(clicked()),this,SLOT(respSwitchPage()));

    connect(ui->search,SIGNAL(clicked()),this,SLOT(respSearch()));
//    connect(ui->filterItem,SIGNAL(clicked()),this,SLOT(setFilterCondition()));
    connect(ui->comboBox,SIGNAL(currentIndexChanged(int)),this,SLOT(respPerPageChange(int)));
//    connect(ui->insertData,SIGNAL(clicked()),this,SIGNAL(signalInserData()));

//    connect(ui->close,SIGNAL(clicked()),this,SIGNAL(signalCloseDialog()));
}

//综合响应页面切换
void PageSwitchBar::respSwitchPage()
{
    QString objName = QObject::sender()->objectName();

    if(objName == "firstPage")
    {
        pageInfo->getFirstPage();
    }
    else if(objName == "prePage")
    {
        pageInfo->getPrePage();
    }
    else if(objName == "nextPage")
    {
        pageInfo->getNextPage();
    }
    else if(objName == "lastPage")
    {
        pageInfo->getLastPage();
    }

    respSearch();
}

//查询
void PageSwitchBar::respSearch()
{
    showTipInfo();
    emit signalBeginSearch();
}

//显示当前页/总页数
void PageSwitchBar::showTipInfo()
{
    ui->currPage->setText(pageInfo->setFormat());
}

//响应每页数据改变后，重新从第一页显示。
void PageSwitchBar::respPerPageChange(int)
{
    pageInfo->reset();
    setPageShowNum();
    emit signalBeginSearch();
}

//设置每页显示的数量
void PageSwitchBar::setPageShowNum()
{
    QString currText = ui->comboBox->currentText();
    pageInfo->setPerPageShow(currText.left(currText.size()-1).toInt());
}

//设置每页显示的条数
void PageSwitchBar::setPerPageShow(const int num)
{
    pageInfo->setPerPageShow(num);
}

//设置总条数
void PageSwitchBar::setTotalNum(const int num)
{
    pageInfo->setTotalNum(num);
}

//设置当前页
void PageSwitchBar::setCurrPage(const int num)
{
    pageInfo->setCurrPage(num);
}

//获取标准的输出
QString PageSwitchBar::getFormatOutput(const QString &format)
{
   return pageInfo->setFormat(format);
}

//获取总的条数
const int PageSwitchBar::getTotalNums()
{
    return pageInfo->getTotalNums();
}

//获取当前的页面数
const int PageSwitchBar::getCurrPage()
{
    return pageInfo->getCurrPage();
}

//获取每页显示的条数
const int PageSwitchBar::getPerPageShow()
{
    return pageInfo->getPerPageShow();
}

//获取总的页数
const int PageSwitchBar::getTotalPage()
{
    return pageInfo->getTotalPages();
}

PageSwitchBar::~PageSwitchBar()
{
    if(pageInfo)
    {
        delete pageInfo;
        pageInfo = NULL;
    }
    delete ui;
}
