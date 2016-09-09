#-------------------------------------------------
#
# Project created by QtCreator 2016-08-30T16:44:29
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Graphics
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    actionmanager.cpp \
    global.cpp \
    util.cpp \
    fileoperate.cpp

HEADERS  += mainwindow.h \
    actionmanager.h \
    Constants.h \
    Header.h \
    global.h \
    util.h \
    fileoperate.h

include($$PWD\SelfWidget\SelfWidget.pri)
include($$PWD\item\item.pri)


FORMS    += mainwindow.ui

RESOURCES += \
    res.qrc
