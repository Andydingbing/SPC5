#
# Copyright 2018 StarPoint Inc.,Ltd
#

#
# Qt project file of Library "sp2401".
#

include( ../include/boost.pri )

QT -= core gui
TARGET = sp2401
TEMPLATE = lib
CONFIG += shared

INCLUDEPATH += ../include       \
               ../include/win   \
               ../log           \
               ../bd            \
               ../sp1401

DEFINES += RD_DLL_EXPORTS

win32 {
    LIBS += $$PWD/../lib/visa64.lib \
            $$PWD/../lib/windrvr/amd64/wdapi1020.lib
    LIBS += -lbd
    LIBS += -llog
}

unix {
    target.path = /usr/lib
    INSTALLS += target
}

CONFIG(debug,debug|release) {
    DEFINES += _DEBUG
    LIBS += -L$$PWD/../Qt/x64/debug/
    OBJECTS_DIR = $$PWD/x64/debug
    DESTDIR = ../Qt/x64/debug
} else {
    LIBS += -L$$PWD/../Qt/x64/release/
    OBJECTS_DIR = $$PWD/x64/release
    DESTDIR = ../Qt/x64/release
}

SOURCES += \
    sp2401_r1a.cpp

HEADERS += \
    sp2401_r1a.h \
    libsp2401.h
