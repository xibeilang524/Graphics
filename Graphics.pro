#-------------------------------------------------
#
# Project created by QtCreator 2016-08-30T16:44:29
#
#-------------------------------------------------

QT       += core gui
QT       += sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Graphics
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    global.cpp \
    util.cpp \
    fileoperate.cpp \
    myapplication.cpp

HEADERS  += mainwindow.h \
    Constants.h \
    Header.h \
    global.h \
    util.h \
    fileoperate.h \
    myapplication.h

include($$PWD/SelfWidget/SelfWidget.pri)
include($$PWD/item/item.pri)
include($$PWD/manager/manager.pri)
include($$PWD/modelview/modelview.pri)
include($$PWD/simulate/simulate.pri)
include($$PWD/sql/sql.pri)
include($$PWD/assisant/assisant.pri)


RC_FILE = images/log.rc


FORMS    += mainwindow.ui

RESOURCES += \
    res.qrc

OTHER_FILES +=
