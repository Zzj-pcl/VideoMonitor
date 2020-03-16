#-------------------------------------------------
#
# Project created by QtCreator 2016-10-16T13:57:42
#
#-------------------------------------------------

QT += core network gui

CONFIG += c++11
DEFINES += QT_DEPRECATED_WARNINGS

CONFIG += console
CONFIG -= app_bundle
INCLUDEPATH += /usr/local/include \
                /usr/local/include/opencv \
                /usr/local/include/opencv2

LIBS += /usr/local/lib/lib*


QT       += core gui network multimedia

QT += widgets

TARGET = smart_home
TEMPLATE = app


SOURCES += main.cpp\
        smart_home.cpp \
    handle_video.cpp \
    mainconsole.cpp \
    EqualizerDialog.cpp \
    capturescreen.cpp \
    cwskcontrolwin.cpp \
    splitscreen.cpp \
    detection.cpp

HEADERS  += smart_home.h \
    handle_video.h \
    mainconsole.h \
    EqualizerDialog.h \
    capturescreen.h \
    cwskcontrolwin.h \
    splitscreen.h \
    detection.h

FORMS    += smart_home.ui \
    EqualizerDialog.ui \
    cwskcontrolwin.ui

RESOURCES += \
    smart_home.qrc

RC_FILE = myapp.rc


#vlc
LIBS       += -lVLCQtCore -lVLCQtWidgets
LIBS       += -L/usr/local/libvlc-qt/lib -lVLCQtCore -lVLCQtWidgets
INCLUDEPATH += /usr/local/libvlc-qt/include
