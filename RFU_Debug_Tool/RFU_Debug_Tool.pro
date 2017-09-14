#-------------------------------------------------
#
# Project created by QtCreator 2017-09-06T13:11:26
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = RFU_Debug_Tool
TEMPLATE = app

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

LIBS += -lBusDriver \
        -lLog \
        -lInstr \
        -lSP1401 \
        -lSP2401 \
        -lSP3301 \
        -lSP3501

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += ../include \
               ../libLog \
               ../libBusDriver \
               ../libInstr \
               ../libSP1401 \
               ../libSP2401 \
               ../libSP3301 \
               ../libSP3501

win32 {
    DEFINES += _WINDOWS \
               _WIN64
}

SOURCES += \
        main.cpp \
        mainwindow.cpp \
    qrfr1cdlg.cpp \
    qrfr1cadvdlg.cpp \
    qwinthread.cpp \
    qdeviceinitthread.cpp \
    qcalr1ctxloleakdlg.cpp \
    qfpgadlg.cpp

HEADERS += \
        mainwindow.h \
    qrfr1cdlg.h \
    qrfr1cadvdlg.h \
    qwinthread.h \
    qdeviceinitthread.h \
    qcalr1ctxloleakdlg.h \
    qfpgadlg.h

FORMS += \
        mainwindow.ui \
    qrfr1cdlg.ui \
    qrfr1cadvdlg.ui \
    qcalr1ctxloleakdlg.ui \
    qfpgadlg.ui
