#-------------------------------------------------
#
# Project created by QtCreator 2017-09-04T10:56:55
#
#-------------------------------------------------

QT       -= core gui

TARGET = libSP3501
TEMPLATE = lib
CONFIG += shared

INCLUDEPATH += ../include \
               ../libLog \
               ../libBusDriver

CONFIG(debug,debug|release) {
    win32 {
        DEFINES += _DEBUG
        LIBS += -L$$PWD/../x64/debug/
    }

    OBJECTS_DIR = $$PWD/x64/debug
    DESTDIR = ../x64/debug
} else {
    win32 {
        LIBS += -L$$PWD/../x64/release/
    }

    OBJECTS_DIR = $$PWD/x64/release
    DESTDIR = ../x64/release
}

LIBS += -llibBusDriver.dll
LIBS += -llibLog.dll

win32 {
    LIBS += $$PWD/../lib/visa64.lib \
            $$PWD/../lib/windrvr/amd64/wdapi1020.lib \
            $$PWD/../lib/libpthreadGC2.a
    DEFINES += _WINDOWS \
               _WIN64
}

DEFINES += LIBSP3501_LIBRARY

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
    SP3501.cpp

HEADERS += \
    SP3501.h \
    libSP3501.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
