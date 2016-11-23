/*************************************************
**版  权：RenGu Company
**文件名: mylinecombobox.h
**作  者: wey       Version: 1.0       Date: 2016.09.27
**描  述:
**Others: 自定义线条类型下拉框
**
**修改历史:
**
*************************************************/
#ifndef MYLINECOMBOBOX_H
#define MYLINECOMBOBOX_H

#include <QComboBox>
#include <QMap>

#include "../Header.h"
#include "id.h"

class MyLineComboBox : public QComboBox
{
    Q_OBJECT
public:
    MyLineComboBox();
    void updateIndex(int index);

signals:
    void indexChanged(int);

private slots:
    void respIndexChanged(int index);

private:
    bool isAutoChanged;
};

//下拉框管理器
class ComboBoxManager
{
public:
    static ComboBoxManager * instance();
    MyLineComboBox *addItem(const char * name);

    bool contains(const char * name);

    MyLineComboBox * item(const char * name);

private:
    ComboBoxManager();
    static ComboBoxManager * manager;

    QMap<Id,MyLineComboBox*> maps;
};

#endif // MYLINECOMBOBOX_H
