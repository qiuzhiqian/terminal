#-------------------------------------------------
#
# Project created by QtCreator 2016-12-04T10:16:10
#
#-------------------------------------------------

QT       += core gui xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = terminal
TEMPLATE = app


SOURCES += main.cpp\
        terminal.cpp \
    sendbutton.cpp \
    treeview.cpp

HEADERS  += terminal.h \
    sendbutton.h \
    treeview.h

FORMS    += terminal.ui
