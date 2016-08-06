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
    mainwindow.h \
    PlayMediaSDK.h

unix|win32: LIBS += -L$$PWD/./ -lNetClientSDK

INCLUDEPATH += $$PWD/.
DEPENDPATH += $$PWD/.

unix|win32: LIBS += -L$$PWD/./ -lPlayMediaSDK

INCLUDEPATH += $$PWD/.
DEPENDPATH += $$PWD/.
