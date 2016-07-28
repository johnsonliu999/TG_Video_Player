QT += core
QT += gui widgets

QT += sql serialport network

CONFIG += c++11

TARGET = test
#CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    task.cpp \
    mainwindow.cpp \
    a.cpp \
    b.cpp \
    iplineedit.cpp

INCLUDEPATH += C:/Qwt-6.1.2/include

INCLUDEPATH += D:/Download/OpenCV/opencv/build/include \
D:/Download/OpenCV/opencv/build/include/opencv \
D:/Download/OpenCV/opencv/build/include/opencv2

#LIBS += D:\Download\OpenCV\opencv\build\x64\vc14\lib\opencv_world310d.lib \
#D:\Download\OpenCV\opencv\build\x64\vc14\lib\opencv_world310.lib

#win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../Download/OpenCV/opencv/build/x64/vc14/lib/ -lopencv_world310
#else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../Download/OpenCV/opencv/build/x64/vc14/lib/ -lopencv_world310d
#else:unix: LIBS += -L$$PWD/../../../Download/OpenCV/opencv/build/x64/vc14/lib/ -lopencv_world310

INCLUDEPATH += $$PWD/../../../Download/OpenCV/opencv/build/x64/vc14
DEPENDPATH += $$PWD/../../../Download/OpenCV/opencv/build/x64/vc14

FORMS += \
    form.ui \
    mainwindow.ui

HEADERS += \
    test.h \
    task.h \
    TGClientSDK.h \
    mainwindow.h \
    a.h \
    b.h \
    iplineedit.h

#LIBS += C:/Qwt-6.1.2/lib/qwtd.lib
#C:/Qwt-6.1.2/lib/qwt.lib \

unix|win32: LIBS += -L$$PWD/./ -lNetClientSDK

INCLUDEPATH += $$PWD/.
DEPENDPATH += $$PWD/.
