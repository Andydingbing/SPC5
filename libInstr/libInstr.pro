#-------------------------------------------------
#
# Project created by QtCreator 2017-08-26T17:01:44
#
#-------------------------------------------------

QT       -= core gui

TARGET = libInstr
TEMPLATE = lib
CONFIG += shared

INCLUDEPATH += ../include \
               ../libBusDriver

LIBS += -llibBusDriver.dll

win32 {
    LIBS += $$PWD/../lib/visa64.lib
    LIBS += $$PWD/../lib/rsnrpz_64.lib
    DEFINES += _WINDOWS \
               _WIN64
}

CONFIG(debug,debug|release) {
    win32 {
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

DEFINES += LIBINSTR_LIBRARY

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
    ISA.cpp \
    FSUP.cpp \
    N9010A.cpp \
    N9020A.cpp \
    N9030A.cpp \
    SMF100A.cpp \
    E4438C.cpp \
    E4440A.cpp \
    E4443A.cpp \
    E4445A.cpp \
    E8267D.cpp \
    N5182A.cpp \
    N5182B.cpp \
    RSNRP.cpp \
    Instrument.cpp

HEADERS += \
    ISA.h \
    FSUP.h \
    N9010A.h \
    N9020A.h \
    N9030A.h \
    SMF100A.h \
    E4438C.h \
    E4440A.h \
    E4443A.h \
    E4445A.h \
    E8267D.h \
    IPM.h \
    ISG.h \
    N5182A.h \
    N5182B.h \
    RSNRP.h \
    Instrument.h \
    libInstr.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
