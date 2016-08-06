QT += core
QT += gui widgets

QT += sql

CONFIG += c++11

TARGET = test
#CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    mainwindow.cpp

FORMS += \
    mainwindow.ui

HEADERS += \
    TGClientSDK.h \
    mainwindow.h

unix|win32: LIBS += -L$$PWD/./ -lNetClientSDK

INCLUDEPATH += $$PWD/.
DEPENDPATH += $$PWD/.
