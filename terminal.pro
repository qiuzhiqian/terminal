#-------------------------------------------------
#
# Project created by QtCreator 2016-12-04T10:16:10
#
#-------------------------------------------------

QT       += core gui xml serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = terminal
TEMPLATE = app


SOURCES += main.cpp\
        terminal.cpp \
    sendbutton.cpp \
    treeview.cpp \
    mycombobox.cpp \
    mylineedit.cpp \
    ftphandle.cpp \
    isp_widget.cpp

HEADERS  += terminal.h \
    sendbutton.h \
    treeview.h \
    mycombobox.h \
    mylineedit.h \
    ftphandle.h \
    isp_widget.h

FORMS    += terminal.ui
