/*************************************************
**版  权：RenGu Company
**文件名: myplaintextedit.h
**作  者: wey       Version: 1.0       Date: 2016.11.18
**描  述: 自定义文本编辑框
**Others:实现对引用参数进行点选，以及支持backspace和delete控件对引用参数的动态判断。
**
**修改历史:
**20161121:wey:增加对backspace和delete的支持
*************************************************/
#ifndef MYPLAINTEXTEDIT_H
#define MYPLAINTEXTEDIT_H

#include <QPlainTextEdit>

class MyPlainTextEdit : public QPlainTextEdit
{
    Q_OBJECT
public:
    MyPlainTextEdit(QWidget * parent = 0);

protected:
    void keyPressEvent(QKeyEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);

private:
    //搜寻方向
    enum SearchDirect
    {
        SEARCH_BEFORE,          //向前搜寻
        SEARCH_AFTER            //向后搜寻
    };


    int findBeforePos(QString source,int curPos,QChar character);
    int findAfterPos(QString source,int curPos,QChar character);
    bool searchQuoteParameter(SearchDirect direct = SEARCH_BEFORE);
};

#endif // MYPLAINTEXTEDIT_H
