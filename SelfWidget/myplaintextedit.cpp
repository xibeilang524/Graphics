#include "myplaintextedit.h"

#include <QKeyEvent>

MyPlainTextEdit::MyPlainTextEdit(QWidget *parent):
    QPlainTextEdit(parent)
{

}

void MyPlainTextEdit::mouseReleaseEvent(QMouseEvent *e)
{
    searchQuoteParameter();
    QPlainTextEdit::mouseReleaseEvent(e);
}

void MyPlainTextEdit::keyPressEvent(QKeyEvent *e)
{
    if(e->key() == Qt::Key_Backspace)
    {
        searchQuoteParameter();
    }
    else if(e->key() == Qt::Key_Delete)
    {
        searchQuoteParameter(SEARCH_AFTER);
    }

    QPlainTextEdit::keyPressEvent(e);
}

int MyPlainTextEdit::findBeforePos(QString source,int curPos,QChar character)
{
    while(curPos > 0)
    {
        if(source.at(curPos-1) == character)
        {
            return curPos;
        }
        curPos--;
    }
    return -1;
}

int MyPlainTextEdit::findAfterPos(QString source,int curPos,QChar character)
{
    while(curPos < source.length())
    {
        if(source.at(curPos) == character)
        {
            return curPos;
        }
        curPos++;
    }
    return -1;
}

//查询引用
bool MyPlainTextEdit::searchQuoteParameter(SearchDirect direct)
{
    QString before = toPlainText();
    QTextCursor cursor = textCursor();
    //从此位置向前查找"["
    int pos = cursor.position();
    int beforePos = pos;
    int afterPos = pos;

    if(direct == SEARCH_BEFORE)
    {
        int firstLeft = findBeforePos(before,beforePos,'[');
        int firstRight = findBeforePos(before,beforePos,']');

        //光标在右括号之右(将光标紧邻']')
        if(firstRight >= firstLeft)
        {
            if(firstRight == pos)
            {
                cursor.setPosition(firstLeft-1,QTextCursor::MoveAnchor);
                cursor.setPosition(firstRight,QTextCursor::KeepAnchor);
                setTextCursor(cursor);
                return true;
            }
        }
        //光标在左右括号之间
        else
        {
            beforePos = firstLeft;

            bool findAfter = false;
            afterPos = findAfterPos(before,afterPos,']');
            if(afterPos > 0)
            {
                findAfter = true;
            }

            if(findAfter)
            {
               cursor.setPosition(beforePos-1,QTextCursor::MoveAnchor);
               cursor.setPosition(afterPos+1,QTextCursor::KeepAnchor);
               setTextCursor(cursor);
               return true;
            }
        }
    }
    else if(direct == SEARCH_AFTER)
    {
        int afterFirstLeft = findAfterPos(before,afterPos,'[');
        int afterFirstRight = findAfterPos(before,afterPos,']');

        int beforeFirstLeft = findBeforePos(before,afterPos,'[');
        int beforeFirstRight = findBeforePos(before,afterPos,']');

        if(afterFirstLeft == pos  || afterFirstRight == pos ||(afterFirstLeft < pos && pos <afterFirstRight))
        {
            if(afterFirstRight == pos && beforeFirstLeft >=0 && beforeFirstLeft < pos)
            {
                cursor.setPosition(beforeFirstLeft -1,QTextCursor::MoveAnchor);
                cursor.setPosition(afterFirstRight + 1,QTextCursor::KeepAnchor);
                setTextCursor(cursor);
            }
            else
            {
                cursor.setPosition(afterFirstLeft,QTextCursor::MoveAnchor);
                cursor.setPosition(afterFirstRight + 1,QTextCursor::KeepAnchor);
                setTextCursor(cursor);
            }

            return true;
        }

        if(beforeFirstLeft > beforeFirstRight && afterFirstLeft < afterFirstRight && afterFirstLeft < beforeFirstRight)
        {
            return false;
        }

        if(beforeFirstLeft <= pos && pos <= afterFirstRight && beforeFirstLeft > beforeFirstRight)
        {
            cursor.setPosition(beforeFirstLeft - 1 ,QTextCursor::MoveAnchor);
            cursor.setPosition(afterFirstRight + 1,QTextCursor::KeepAnchor);
            setTextCursor(cursor);
            return true;
        }
    }

    return false;
}
