#include "serviceinputdelegate.h"

#include <QComboBox>

ServiceInputDelegate::ServiceInputDelegate()
{
    stringList<<QString("bool")<<QString("char")<<QString("short")<<QString("int")<<QString("float")<<QString("double");
}

//创建代理的控件
QWidget * ServiceInputDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if(index.column() == 1)
    {
        QComboBox * box = new QComboBox(parent);
        box->addItems(stringList);
        return box;
    }
    return NULL;
}

//设置代理控件的值
void ServiceInputDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QString value = index.model()->data(index,Qt::DisplayRole).toString();
    if(index.column() == 1)
    {
        QComboBox * box = dynamic_cast<QComboBox *>(editor);
        if(box)
        {
            box->setCurrentIndex(stringList.indexOf(value));
        }
    }
}

//更新代理控件的位置尺寸
void ServiceInputDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    editor->setGeometry(option.rect);
}

//更新模型数据
void ServiceInputDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QString value;
    if(index.column() == 1)
    {
        QComboBox * box = dynamic_cast<QComboBox *>(editor);
        if(box)
        {
            value = box->currentText();
        }
    }
    model->setData(index,value,Qt::EditRole);
}
