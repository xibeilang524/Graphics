/*************************************************
**��  Ȩ��RenGu Company
**�ļ���: serviceinputdelegate.h
**��  ��: wey       Version: 1.0       Date: 2016.10.10
**��  ��:
**Others:��������������������
**
**�޸���ʷ:
**20161010:wey:����QComboBox�ؼ���Ϊ���ͱ༭����
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

protected:
    QWidget * createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;

private:
    QStringList  stringList;            //���������ؼ���item

};

#endif // SERVICEINPUTDELEGATE_H