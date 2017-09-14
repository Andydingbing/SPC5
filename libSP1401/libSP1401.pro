#-------------------------------------------------
#
# Project created by QtCreator 2017-08-28T10:29:32
#
#-------------------------------------------------

QT       -= core gui

TARGET = libSP1401
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

DEFINES += LIBSP1401_LIBRARY

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
    ArbReader.cpp \
    CalFileR1A.cpp \
    CalFileR1C.cpp \
    CfvCtrl.cpp \
    ICalFile.cpp \
    RxAttTable.cpp \
    RxFilterOffTable.cpp \
    RxFilterTable.cpp \
    RxRefTable.cpp \
    TxAttTable.cpp \
    TxFilterOffTable.cpp \
    TxFilterTable.cpp \
    TxLoLeakageTable.cpp \
    TxPowerTable.cpp \
    TxSidebandTable.cpp \
    ISP1401.cpp \
    DMAMgr.cpp \
    IQBufMgr.cpp \
    SP1401R1A.cpp \
    SP1401R1C.cpp

HEADERS += \
    ArbReader.h \
    CalFileR1A.h \
    CalFileR1C.h \
    CalTable.h \
    CfvCtrl.h \
    ICalFile.h \
    RxAttTable.h \
    RxFilterOffTable.h \
    RxFilterTable.h \
    RxRefTable.h \
    TxAttTable.h \
    TxFilterOffTable.h \
    TxFilterTable.h \
    TxLoLeakageTable.h \
    TxPowerTable.h \
    TxSidebandTable.h \
    ISP1401.h \
    DMAMgr.h \
    IQBufMgr.h \
    SP1401R1A.h \
    SP1401R1B.h \
    SP1401R1C.h \
    SP1401R1D.h \
    libSP1401.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
