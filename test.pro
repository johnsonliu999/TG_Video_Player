QT += core
QT += gui widgets

QT += sql

CONFIG += c++11

TARGET = test
#CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    mainwindow.cpp \
    abstracttgfile.cpp \
    tgfile.cpp \
    drawthread.cpp

FORMS += \
    mainwindow.ui

HEADERS += \
    mainwindow.h \
    PlayMediaSDK.h \
    abstracttgfile.h \
    tgfile.h \
    tgfiledef.h \
    drawthread.h

INCLUDEPATH += $$PWD/.
DEPENDPATH += $$PWD/.

unix|win32: LIBS += -L$$PWD/./ -lPlayMediaSDK
