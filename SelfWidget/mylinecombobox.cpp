#include "mylinecombobox.h"

#include <QListView>

MyLineComboBox::MyLineComboBox()
{

    setView(new QListView());
    setStyleSheet("QComboBox{ height:23px;\
                  }\
                  QComboBox QAbstractItemView::item{\
                  width:150px;height:30px;selection-background-color: lightgray;\
                 }");
    setSizeAdjustPolicy(QComboBox::AdjustToContents);
}
