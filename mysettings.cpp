#include "mysettings.h"

#include <QDebug>

MySettings::MySettings(const QString & fileName, Format format, QObject * parent):
    QSettings(fileName,format,parent)
{

}

void MySettings::readSplashSettings(QList<SplashDesc *> &splashDescs)
{
    beginGroup("SplashScreen");

    int count = beginReadArray("SplashDesc");

    for(int i = 0; i < count; i++)
    {
        setArrayIndex(i);

        SplashDesc  * desc = new SplashDesc;

        desc->splashContent = value("splashContent").toString();

        desc->scaleX = value("scaleX").toDouble();
        desc->scaleY = value("scaleY").toDouble();
        desc->scaleWidth = value("scaleW").toDouble();
        desc->scaleHeight = value("scaleH").toDouble();

        desc->fontSize = value("fontSize").toInt();
        desc->fontFamily = value("fontFamily").toString();
        desc->isBold = value("isBold").toBool();

        desc->fontColor = QColor(value("fontColor").toString());

        splashDescs.push_back(desc);
    }

    endArray();
    endGroup();
}

void MySettings::writeSplashSettings(QList<SplashDesc *> &splashDescs)
{
    beginGroup("SplashScreen");
    remove("SplashDesc");
    beginWriteArray("SplashDesc");
    int count = 0;
    foreach(SplashDesc * desc,splashDescs)
    {
        setArrayIndex(count++);
        setValue("splashContent",desc->splashContent);

        setValue("scaleX",desc->scaleX);
        setValue("scaleY",desc->scaleY);
        setValue("scaleW",desc->scaleWidth);
        setValue("scaleH",desc->scaleHeight);

        setValue("fontSize",desc->fontSize);
        setValue("fontFamily",desc->fontFamily);
        setValue("fontColor",desc->fontColor.name());
        setValue("isBold",desc->isBold);
    }
    endArray();
    endGroup();
}
