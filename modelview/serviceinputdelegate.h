/*************************************************
**版  权：RenGu Company
**文件名: serviceinputdelegate.h
**作  者: wey       Version: 1.0       Date: 2016.10.10
**描  述:
**Others:参数输入框类型输入代理
**
**修改历史:
**20161010:wey:添加QComboBox控件作为类型编辑代理
**20161031:wey:添加控件输入的引参【！！！】
*************************************************/
#ifndef SERVICEINPUTDELEGATE_H
#define SERVICEINPUTDELEGATE_H

#include <QItemDelegate>
#include <QStringList>

class ServiceInputDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    ServiceInputDelegate();

    void setColumnState(bool isComboBox = true);
    void updateStringList(const QStringList & list);

protected:
    QWidget * createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;

private:
    QStringList  stringList;             //代理下拉控件子item

    bool column2IsCombox;                //用于区分列表2代理类型
};

#endif // SERVICEINPUTDELEGATE_H
