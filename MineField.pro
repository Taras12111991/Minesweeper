#-------------------------------------------------
#
# Project created by QtCreator 2017-01-29T14:51:30
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MineField
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    minebutton.cpp \
    minefield.cpp

HEADERS  += mainwindow.h \
    minebutton.h \
    minefield.h

FORMS    += mainwindow.ui

RESOURCES += \
    Resources.qrc
