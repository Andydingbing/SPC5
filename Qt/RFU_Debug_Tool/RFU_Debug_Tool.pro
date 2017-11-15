#-------------------------------------------------
#
# Project created by QtCreator 2017-09-06T13:11:26
#
#-------------------------------------------------

QT += core gui
QT += charts
QMAKE_CXXFLAGS += -std=c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = RFU_Debug_Tool
TEMPLATE = app

INCLUDEPATH += ../../include \
               ../../libLog \
               ../../libBusDriver \
               ../../libInstr \
               ../../libSP1401 \
               ../../libSP2401 \
               ../../libSP3301 \
               ../../libSP3501 \
               ../../lib/qwt/qwt-6.1.3/src

CONFIG(debug,debug|release) {
    DEFINES += _DEBUG
    LIBS += -L$$PWD/../x64/debug/
    OBJECTS_DIR = $$PWD/x64/debug
    DESTDIR = ../x64/debug
    win32 {
        LIBS += $$PWD/../../lib/qwt/qwt-6.1.3/lib/libqwtd.dll.a
    }
    unix {
        LIBS += $$PWD/../../lib/qwt/qwt-6.1.3/lib/libqwt.so
    }
} else {
    LIBS += -L$$PWD/../x64/release/

    OBJECTS_DIR = $$PWD/x64/release
    DESTDIR = ../x64/release
    win32 {
        LIBS += $$PWD/../../lib/qwt/qwt-6.1.3/lib/libqwt.dll.a
    }
    unix {
        LIBS += $$PWD/../../lib/qwt/qwt-6.1.3/lib/libqwt.so
    }
}

win32 {
    LIBS += $$PWD/../../lib/visa64.lib \
            $$PWD/../../lib/windrvr/amd64/wdapi1020.lib \
            $$PWD/../../lib/libpthreadGC2.a
    LIBS += -llibSP3301.dll \
            -llibSP1401.dll \
            -llibSP2401.dll \
            -llibSP3501.dll \
            -llibInstr.dll  \
            -llibBusDriver.dll \
            -llibLog.dll
}

unix {
    LIBS += /usr/local/lib64/libvisa.so
    LIBS += -lrsnrpz
    LIBS += -llibSP3301 \
            -llibSP1401 \
            -llibSP2401 \
            -llibSP3501 \
            -llibInstr  \
            -llibBusDriver  \
            -llibLog    \
            -lfftw3
}


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
        main.cpp \
        mainwindow.cpp \
    qrfr1cdlg.cpp \
    qrfr1cadvdlg.cpp \
    qwinthread.cpp \
    qdeviceinitthread.cpp \
    qcalr1ctxloleakdlg.cpp \
    qfpgadlg.cpp \
    qattachthreaddlg.cpp \
    qr1ctxloleakmodel.cpp \
    qcalr1ctxloleakthread.cpp \
    qrdttableview.cpp \
    qmsglogmodel.cpp \
    qreglogmodel.cpp \
    qcalbasemodel.cpp \
    qiqcapdlg.cpp \
    qiqcapthread.cpp

HEADERS += \
        mainwindow.h \
    qrfr1cdlg.h \
    qrfr1cadvdlg.h \
    qwinthread.h \
    qdeviceinitthread.h \
    qcalr1ctxloleakdlg.h \
    qfpgadlg.h \
    qattachthreaddlg.h \
    qr1ctxloleakmodel.h \
    qcalr1ctxloleakthread.h \
    qrdttableview.h \
    qmsglogmodel.h \
    qreglogmodel.h \
    qcalbasemodel.h \
    qiqcapdlg.h \
    qiqcapthread.h \
    qcalbasedlg.h

FORMS += \
        mainwindow.ui \
    qrfr1cdlg.ui \
    qrfr1cadvdlg.ui \
    qcalr1ctxloleakdlg.ui \
    qfpgadlg.ui \
    qiqcapdlg.ui
