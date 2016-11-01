#include "serviceinputdelegate.h"

#include <QComboBox>
#include <QListView>
#include <QLineEdit>

#include <QDebug>

ServiceInputDelegate::ServiceInputDelegate()
{
    column2IsCombox = true;
//    stringList<<QString("bool")<<QString("char")<<QString("short")<<QString("int")<<QString("float")<<QString("double");
}

//更新列表的信息
void ServiceInputDelegate::updateStringList(const QStringList &list)
{
    stringList.clear();
    stringList = list;
}

//创建代理的控件
QWidget * ServiceInputDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(option)
    if(index.column() == 2)
    {
        if(column2IsCombox)
        {
            QComboBox * box = new QComboBox(parent);
            box->setView(new QListView);
            box->setStyleSheet("QComboBox QAbstractItemView::item{height:23px;}QComboBox QAbstractItemView::item:hover{background-color:#567dbc;}");
            box->setEditable(true);
            box->addItems(stringList);
            return box;
        }
        else
        {
            QLineEdit * edit = new QLineEdit(parent);
            return edit;
        }

    }
    return NULL;
}

//设置代理控件的值,如果列表不存在当前值，则将当前值插入至候选列表，防止因未选择值导致当前值被覆盖。
void ServiceInputDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QString value = index.model()->data(index,Qt::DisplayRole).toString();

    if(index.column() == 2)
    {
        if(column2IsCombox)
        {
            QComboBox * box = dynamic_cast<QComboBox *>(editor);
            if(box)
            {
                if(!stringList.contains(value))
                {
                    box->insertItem(0,value);
                    box->setCurrentIndex(0);
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
void ServiceInputDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(option)
    Q_UNUSED(index)
    editor->setGeometry(option.rect);
}

//更新模型数据
void ServiceInputDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QString value;
    if(index.column() == 2)
    {
        if(column2IsCombox)
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

//设置第二列控件代理类型
void ServiceInputDelegate::setColumnState(bool isComboBox)
{
    column2IsCombox = isComboBox;
}
