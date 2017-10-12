#-------------------------------------------------
#
# Project created by QtCreator 2017-08-25T16:47:27
#
#-------------------------------------------------

QT       -= core gui

TARGET = libBusDriver
TEMPLATE = lib
CONFIG += shared

INCLUDEPATH += ../include \
               ../include/windrvr

win32 {
    LIBS += $$PWD/../lib/visa64.lib \
            $$PWD/../lib/windrvr/amd64/wdapi1020.lib

    DEFINES += _WINDOWS \
               _WIN64
}

CONFIG(debug,debug|release) {
    OBJECTS_DIR = $$PWD/x64/debug
    DESTDIR = ../Qt/x64/debug
} else {
    OBJECTS_DIR = $$PWD/x64/release
    DESTDIR = ../Qt/x64/release
}

DEFINES += LIBBUSDRIVER_LIBRARY

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
    viPCIDev.cpp \
    wdPCIDev.cpp \
    viMemIO.cpp \
    IGPIBDev.cpp \
    algorithm.cpp \
    FractionFloat.cpp \
    GeneralIniFile.cpp \
    GeneralInteger.cpp

HEADERS += \
    IPCIDev.h \
    viPCIDev.h \
    wdPCIDev.h \
    viMemIO.h \
    IGPIBDev.h \
    libBusDriver.h \
    algorithm.h \
    FractionFloat.h \
    GeneralIniFile.h \
    GeneralInteger.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
