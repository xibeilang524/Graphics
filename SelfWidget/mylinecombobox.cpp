#include "mylinecombobox.h"

#include <QListView>

MyLineComboBox::MyLineComboBox()
{

    setView(new QListView());
    setStyleSheet("QComboBox{ height:23px;\
                  }\
                  QComboBox QAbstractItemView::item{\
                  height:30px;selection-background-color: lightgray;\
                 }");
    setSizeAdjustPolicy(QComboBox::AdjustToContents);
}
