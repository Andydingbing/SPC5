#-------------------------------------------------
#
# Project created by QtCreator 2017-08-28T10:40:36
#
#-------------------------------------------------

QT       -= core gui

TARGET = libLog
TEMPLATE = lib
CONFIG += shared

INCLUDEPATH += ../include

win32 {
    LIBS += $$PWD/../lib/libpthreadGC2.a
}

CONFIG(debug,debug|release) {
    DEFINES += _DEBUG
    OBJECTS_DIR = $$PWD/x64/debug
    DESTDIR = ../Qt/x64/debug
} else {
    OBJECTS_DIR = $$PWD/x64/release
    DESTDIR = ../Qt/x64/release
}

DEFINES += LIBLOG_LIBRARY

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Log.cpp

HEADERS += \
    Log.h \
    libLog.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
