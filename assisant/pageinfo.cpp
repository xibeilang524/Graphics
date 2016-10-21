#include "pageinfo.h"
#include "math.h"


PageInfo::PageInfo()
{
    reset();
}

void PageInfo::reset()
{
    perPageShow = 0;
    totalNums = 0;
    totalPages = 0;
    currPage = 0;
}

const QString PageInfo::setFormat(const QString &format)
{
    return QString::number(this->currPage+1)+format+QString::number(this->totalPages);
}

//手动的设置总数量
void PageInfo::setTotalNum(const int num)
{
    this->totalNums = num;
    countTotalPage();
}

//手动设置当前显示的页面
void PageInfo::setCurrPage(const int num)
{
    if(num<0 || num >= totalPages)
    {
        this->currPage = 0;
    }
    else
    {
        this->currPage = num;
    }
}

//增加一条记录
void PageInfo::addNum()
{
    totalNums++;
    countTotalPage();
}

void PageInfo::countTotalPage()
{
    totalPages = ceil((float)totalNums/perPageShow);
}

//上一页
int PageInfo::getPrePage()
{
    currPage = (currPage-1<=0)?0:currPage-1;

    return this->currPage;
}

//下一页
int PageInfo::getNextPage()
{
    currPage = (currPage+1>=totalPages)?totalPages-1:currPage+1;

    fixCurrPage();

    return this->currPage;
}

//第一页
int PageInfo::getFirstPage()
{
    this->currPage = 0;
    return this->currPage;
}

//最后一页
int PageInfo::getLastPage()
{
    currPage = totalPages -1;

    fixCurrPage();

    return this->currPage;
}

//验证性修复当前页面为负的情况
void PageInfo::fixCurrPage()
{
    currPage = (currPage<=0)?0:currPage;
}
