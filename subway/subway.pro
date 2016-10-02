#-------------------------------------------------
#
# Project created by QtCreator 2016-09-21T18:30:04
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = subway
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    errorhandle.cpp \
    paintwidget.cpp \
    city.cpp

HEADERS  += mainwindow.h \
    errorhandle.h \
    paintwidget.h \
    city.h \
    line.h \
    station.h \
    city.h \
    line.h \
    station.h

FORMS    += \
    errorhandle.ui \
    mainwindow.ui

DISTFILES += \
    subway.pro.user

RESOURCES += \
    image.qrc
CONFIG += console
