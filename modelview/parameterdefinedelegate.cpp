#include "parameterdefinedelegate.h"

#include <QComboBox>
#include <QLineEdit>
#include <QListView>
#include <QDebug>

ParameterDefineDelegate::ParameterDefineDelegate(LoopPart loopPart):
    loopPart(loopPart)
{
    switch(loopPart)
    {
        case LOOP_VARI:
                        stringList<<QString("short")<<QString("int")<<QString("long")
                                 <<QString(COMBOX_LOOP_QUOTE);
                        break;
        case LOOP_EXPRESS:
                        stringList<<">"<<">="<<"<"<<"<="<<"==";
                        break;
        case LOOP_FINAL:
                        stringList<<"++"<<"--"<<"+="<<"-=";
                        break;
    }
}

//创建代理的控件
QWidget * ParameterDefineDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(option)
    if(loopPart == LOOP_VARI)
    {
        if(index.column() == 0)
        {
                QComboBox * box = new QComboBox(parent);
                box->setView(new QListView);
                box->setStyleSheet("QComboBox QAbstractItemView::item{height:23px;}QComboBox QAbstractItemView::item:hover{background-color:#567dbc;}");
//                box->setEditable(true);
                box->addItems(stringList);
                return box;
        }
    }
    else if(loopPart == LOOP_EXPRESS || loopPart == LOOP_FINAL)
    {
        if(index.column() == 1 || index.column() == 0)
        {
                QComboBox * box = new QComboBox(parent);
                box->setView(new QListView);
                box->setStyleSheet("QComboBox QAbstractItemView::item{height:23px;}QComboBox QAbstractItemView::item:hover{background-color:#567dbc;}");
//                box->setEditable(true);
                if(index.column() == 0)
                {
                    box->addItems(currVariList);
                }
                else
                {
                    box->addItems(stringList);
                }

                return box;
        }
    }
    QLineEdit * edit = new QLineEdit(parent);
    return edit;
}

//设置代理控件的值,如果列表不存在当前值，则将当前值插入至候选列表，防止因未选择值导致当前值被覆盖。
void ParameterDefineDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QString value = index.model()->data(index,Qt::DisplayRole).toString();

    if(loopPart == LOOP_VARI)
    {
        if(index.column() == 0 )
        {
            QComboBox * box = dynamic_cast<QComboBox *>(editor);
            if(box)
            {
//                if(!stringList.contains(value))
//                {
//                    box->insertItem(0,value);
//                    box->setCurrentIndex(0);
//                }
//                else
//                {
                    box->setCurrentIndex(stringList.indexOf(value));
//                }
            }
        }
        else
        {
            QLineEdit * edit = dynamic_cast<QLineEdit *>(editor);
            if(edit)
            {
                edit->setText(value);
            }
        }
    }
    else if(loopPart == LOOP_EXPRESS || loopPart == LOOP_FINAL)
    {
        if(index.column() == 1 || index.column() == 0)
        {
            QComboBox * box = dynamic_cast<QComboBox *>(editor);
            if(box)
            {
                if(index.column() == 0)
                {
                    box->setCurrentIndex(currVariList.indexOf(value));
                }
                else
                {
                    box->setCurrentIndex(stringList.indexOf(value));
                }
            }
        }
        else
        {
            QLineEdit * edit = dynamic_cast<QLineEdit *>(editor);
            if(edit)
            {
                edit->setText(value);
            }
        }
    }
}

//更新代理控件的位置尺寸
void ParameterDefineDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(option)
    Q_UNUSED(index)
    editor->setGeometry(option.rect);
}

//更新模型数据
void ParameterDefineDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index)
{
    QString value;

    if(loopPart == LOOP_VARI)
    {
        if(index.column() == 0)
        {
            QComboBox * box = dynamic_cast<QComboBox *>(editor);
            if(box)
            {
                value = box->currentText();
            }
        }
        else
        {
            QLineEdit * edit = dynamic_cast<QLineEdit *>(editor);
            if(edit)
            {
                value = edit->text();
            }
        }
    }
    else if(loopPart == LOOP_EXPRESS || loopPart == LOOP_FINAL)
    {
        if(index.column() == 1 || index.column() == 0)
        {
            QComboBox * box = dynamic_cast<QComboBox *>(editor);
            if(box)
            {
                value = box->currentText();
            }
        }
        else
        {
            QLineEdit * edit = dynamic_cast<QLineEdit *>(editor);
            if(edit)
            {
                value = edit->text();
            }
        }
    }

    model->setData(index,value,Qt::EditRole);
}

//更新添加变量名的下拉提示列表，只可以从当前变量列表中选择
void ParameterDefineDelegate::updateVariList(QStringList newList)
{
    currVariList = newList;
}
