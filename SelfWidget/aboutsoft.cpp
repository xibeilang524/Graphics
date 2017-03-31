#include "aboutsoft.h"

#include <QGridLayout>
#include <QDate>
#include <QSysInfo>
#include <QDialogButtonBox>
#include <QLabel>
#include <QPushButton>
#include <QTextBrowser>
#include <QApplication>

#include "../Header.h"

AboutSoft::AboutSoft(QWidget *parent):
    QDialog(parent)
{
    setWindowTitle("关于建模软件");

    initDialog();
}

void AboutSoft::initDialog()
{
    QGridLayout *layout = new QGridLayout(this);
    layout->setSizeConstraint(QLayout::SetFixedSize);

    const QString description = QString(
        "<h3>%1 %2</h3>"
        "Based on Qt %3 (%4 bit)<br/>"
        "<br/>"
        "Built on %5 at %6<br />"
        "<br/>"
        "<br/>"
        "Copyright 2014-%7 %8. All rights reserved.<br/>"
        "<br/>"
        "办公地址：江苏省南京市浦口区团结路99号孵鹰大厦A-403<br/>"
        "电话(传真)：025-58101129<br/>"
        "<br/>")
        .arg(qApp->applicationName())
        .arg(M_VERTION).
            arg(QLatin1String(QT_VERSION_STR)).arg(QString::number(QSysInfo::WordSize))
            .arg(QLatin1String(__DATE__)).arg(QLatin1String(__TIME__))
            .arg(QDate::currentDate().year()).arg(QString(M_AUTHOR));

    QLabel *copyRightLabel = new QLabel(description);
    copyRightLabel->setWordWrap(true);
    copyRightLabel->setOpenExternalLinks(true);
    copyRightLabel->setTextInteractionFlags(Qt::TextBrowserInteraction);

    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Close);
    QPushButton *closeButton = buttonBox->button(QDialogButtonBox::Close);
    buttonBox->addButton(closeButton, QDialogButtonBox::ButtonRole(QDialogButtonBox::RejectRole | QDialogButtonBox::AcceptRole));
    connect(buttonBox , SIGNAL(rejected()), this, SLOT(reject()));

    QLabel *logoLabel = new QLabel;
    logoLabel->setPixmap(QPixmap(":/images/softicon.png"));
    layout->addWidget(logoLabel , 0, 0, 1, 1);
    layout->addWidget(copyRightLabel, 0, 1, 4, 4);
    layout->addWidget(buttonBox, 4, 0, 1, 5);
}
