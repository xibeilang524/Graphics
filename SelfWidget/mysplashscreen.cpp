#include "mysplashscreen.h"

#include <QLabel>
#include <QDebug>

#include "Header.h"
#include "global.h"
#include "mysettings.h"

enum {
    DEBUG_ABLE = 1
};

MySplashScreen::MySplashScreen(const QPixmap &pixmap, Qt::WindowFlags f):
    QSplashScreen(pixmap,f)
{
    settings = NULL;
}

void MySplashScreen::setSettings(MySettings *settings)
{
    this->settings = settings;
    settings->readSplashSettings(splashDescs);

    if(!IsConfigIniExisted)
    {
//        settings->writeSplashSettings(splashDescs);
    }
    setValue();
}


void MySplashScreen::setValue()
{
    foreach(SplashDesc * desc,splashDescs)
    {
        QLabel * label = new QLabel(this);

        label->setText(desc->splashContent);
        QFont font;
        font.setPointSize(desc->fontSize);
        font.setFamily(desc->fontFamily);
        font.setBold(desc->isBold);
        label->setFont(font);
        label->setStyleSheet("color:"+desc->fontColor.name()+";");
        label->setGeometry(desc->scaleX * width(),desc->scaleY * height(),desc->scaleWidth * width(),desc->scaleHeight * height());
    }
}

MySplashScreen::~MySplashScreen()
{
//    foreach(SplashDesc * desc,splashDescs)
//    {
//        delete desc;
//    }
    splashDescs.clear();
}
