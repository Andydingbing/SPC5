#
# Copyright 2018 StarPoint Inc.,Ltd
#

#
# Qt project file of Library "log".
#

include( ../include/boost.pri )

QT -= core gui
TARGET = log
TEMPLATE = lib
CONFIG += shared

INCLUDEPATH += ../include

DEFINES += RD_DLL_EXPORTS

win32 {
    INCLUDEPATH += ../include/win/pthread
    LIBS += $$PWD/../lib/pthread/pthreadVC2.lib
}

unix {
    target.path = /usr/lib
    INSTALLS += target
}

CONFIG(debug,debug|release) {
    DEFINES += _DEBUG
    OBJECTS_DIR = $$PWD/x64/debug
    DESTDIR = ../Qt/x64/debug
} else {
    OBJECTS_DIR = $$PWD/x64/release
    DESTDIR = ../Qt/x64/release
}

SOURCES += \
    log.cpp

HEADERS += \
    liblog.h \
    log.h
