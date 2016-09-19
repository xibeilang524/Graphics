#ifndef MYLISTWIDGET_H
#define MYLISTWIDGET_H

#include <QListWidget>

class MyListWidget : public QListWidget
{
    Q_OBJECT
public:
    MyListWidget(QWidget * parent = 0);


protected:
    void startDrag(Qt::DropActions supportedActions);

private:

};

#endif // MYLISTWIDGET_H
