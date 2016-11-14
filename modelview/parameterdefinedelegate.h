/*************************************************
**版  权：RenGu Company
**文件名: parameterdefinedelegate.h
**作  者: wey       Version: 1.0       Date: 2016.11.11
**描  述: 参数定义代理
**Others:
**
**修改历史:
**
*************************************************/
#ifndef PARAMETERDEFINEDELEGATE_H
#define PARAMETERDEFINEDELEGATE_H

#include <QItemDelegate>
#include <QStringList>

#include "../Header.h"

class ParameterDefineDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    ParameterDefineDelegate(LoopPart loopPart);

    void updateVariList(QStringList newList);

protected:
    QWidget * createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index);

private:
    QStringList  stringList;             //代理下拉控件子item
    QStringList  currVariList;           //当前变量下拉显示列表

    LoopPart loopPart;
};

#endif // PARAMETERDEFINEDELEGATE_H
