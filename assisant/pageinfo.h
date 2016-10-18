/*************************************************
**版  权：RenGu Company
**文件名: pageinfo.h
**作  者: wey       Version: 1.0       Date: 2016.07.05
**描  述: 页面记录
**Others:
**      1：用于根据当前记录的条数来封装上一页、下一页等操作。
**      2：本文档设定当前页从0开始，如果有10条记录，每页显示4条，那么totalPages==3，可以访问的页为0,1,2

**修改历史:
**20160706:wey:修复了初始无数据而导致获取下一页数据出错问题
**20160707:wey:添加手动设定总页数功能
**             添加手动设定当前显示的页数功能
*************************************************/
#ifndef PAGEINFO_H
#define PAGEINFO_H

#include <QString>

class PageInfo
{
public:
    //页面切换的方向
    enum SwitchDirect
    {
        LEFT2RIGHT,            //从左至右
        RIGHT2LEFT,            //从右至左
        TOP2BOTTOM,            //从上至下
        BOTTOM2TOP             //从下至上
    };

    PageInfo();

    void addNum();
    void reset();
    void setPerPageShow(const int num){this->perPageShow =num;}
    void setTotalNum(const int num);
    void setCurrPage(const int num);
    const QString setFormat(const QString & format ="/");
    const int getTotalPages(){return this->totalPages;}
    const int getTotalNums(){return this->totalNums;}
    const int getCurrPage(){return this->currPage;}
    const int getPerPageShow(){return this->perPageShow;}
    const int getPrePage();
    const int getNextPage();
    const int getFirstPage();
    const int getLastPage();

private:
    void countTotalPage();
    void fixCurrPage();

    int perPageShow;           //每页显示的条数
    int totalNums;             //总条数
    int totalPages;            //总页数
    int currPage;              //当前页数
};

#endif // PAGEINFO_H
