#
# Copyright 2018 StarPoint Inc.,Ltd
#

#
# Qt project file of "SPC5_RFU_Debug_Tool".
#

include( ../../include/boost.pri )

QT += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SPC5_RFU_Debug_Tool
TEMPLATE = app

INCLUDEPATH += ./                   \
               Cal                  \
               ../../include/win    \
               ../../include        \
               ../../log            \
               ../../bd             \
               ../../instr          \
               ../../sp1401         \
               ../../sp2401         \
               ../../sp3301         \
               ../../sp3501         \
               ../../lib/qwt/qwt-6.1.3/src

DEFINES += QWT_DLL

CONFIG(debug,debug|release) {
    DEFINES += _DEBUG
    LIBS += -L$$PWD/../x64/debug/
    OBJECTS_DIR = $$PWD/x64/debug
    UI_DIR = $$PWD/x64/ui
    MOC_DIR = $$PWD/x64/debug
    DESTDIR = ../x64/debug

    unix {
        LIBS += $$PWD/../../lib/qwt/qwt-6.1.3/lib/libqwt.so
    }
} else {
    LIBS += -L$$PWD/../x64/release/
    OBJECTS_DIR = $$PWD/x64/release
    UI_DIR = $$PWD/x64/ui
    MOC_DIR = $$PWD/x64/release
    DESTDIR = ../x64/release

    unix {
        LIBS += $$PWD/../../lib/qwt/qwt-6.1.3/lib/libqwt.so
    }
}

win32 {
    LIBS += -L$$PWD/../../lib/qwt/qwt-6.1.3/lib/
    LIBS += $$PWD/../../lib/visa64.lib \
            $$PWD/../../lib/windrvr/amd64/wdapi1020.lib \
            $$PWD/../../lib/pthread/libpthreadGC2.a
    LIBS += -lsp3301    \
            -lsp1401    \
            -lsp2401    \
            -lsp3501    \
            -linstr     \
            -lbd        \
            -llog
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
    qfpgadlg.cpp \
    qattachthreaddlg.cpp \
    qrdttableview.cpp \
    qmsglogmodel.cpp \
    qreglogmodel.cpp \
    qiqcapdlg.cpp \
    qiqcapthread.cpp \
    qtempctrldlg.cpp \
    qrfr1adlg.cpp \
    qrfr1acontainerdlg.cpp \
    qrfr1ccontainerdlg.cpp \
    qrfr1aadvdlg.cpp \
    qadf5355dlg.cpp \
    qarbdlg.cpp \
    qbbdlg.cpp \
    qiqphasescrolledit.cpp \
    qiqampscrolledit.cpp \
    qdcoffsetscrolledit.cpp \
    Cal/qcalbasemodel.cpp \
    Cal/qr1ctxloleakmodel.cpp \
    Cal/qcalr1ctxloleakdlg.cpp \
    Cal/qcalr1ctxloleakthread.cpp \
    Cal/qcalr1ctxsbdlg.cpp \
    Cal/qr1ctxsbmodel.cpp \
    Cal/qcalr1ctxsbthread.cpp \
    Cal/qcalr1ctxfilterdlg.cpp \
    Cal/qcalr1ctxfilterthread.cpp \
    qcalplot.cpp \
    qcalr1ctxfiltertabwidget.cpp \
    Cal/qr1ctxfiltermodel.cpp \
    Cal/qcalr1ctxpwrdlg.cpp \
    Cal/qr1ctxpwrmodel.cpp \
    Cal/qcalr1ctxpwrthread.cpp \
    Cal/qcalbasedlg.cpp \
    Cal/qcalr1ctxattdlg.cpp \
    Cal/qcalr1ctxattthread.cpp \
    Cal/qr1ctxattmodel.cpp \
    qcalr1ctxatttabwidget.cpp \
    qrfdlg.cpp \
    qframeslottrigwidget.cpp \
    Cal/qcalr1ctxfilteroffsetdlg.cpp \
    qcalr1ctxfilteroffsettabwidget.cpp \
    Cal/qr1ctxfilteroffsetmodel.cpp \
    Cal/qcalr1ctxfilteroffsetthread.cpp \
    Cal/qcalr1crxfilterdlg.cpp \
    Cal/qr1crxfiltermodel.cpp \
    qcalr1crxfiltertabwidget.cpp \
    Cal/qcalr1crxfilterthread.cpp \
    Cal/qr1crxrefmodel.cpp \
    Cal/qcalr1crxrefdlg.cpp \
    Cal/qcalr1crxrefthread.cpp \
    Cal/qr1crxattmodel.cpp \
    qcalr1crxatttabwidget.cpp \
    Cal/qcalr1crxattdlg.cpp \
    Cal/qcalr1crxattthread.cpp \
    Cal/qr1crxfilteroffsetmodel.cpp \
    qcalr1crxfilteroffsettabwidget.cpp \
    Cal/qcalr1crxfilteroffsetthread.cpp \
    Cal/qcalr1crxfilteroffsetdlg.cpp

HEADERS += \
        mainwindow.h \
    qrfr1cdlg.h \
    qrfr1cadvdlg.h \
    qwinthread.h \
    qdeviceinitthread.h \
    qfpgadlg.h \
    qattachthreaddlg.h \
    qrdttableview.h \
    qmsglogmodel.h \
    qreglogmodel.h \
    qiqcapdlg.h \
    qiqcapthread.h \
    define.h \
    qtempctrldlg.h \
    qrfr1adlg.h \
    qrfr1acontainerdlg.h \
    qrfr1ccontainerdlg.h \
    qrfr1aadvdlg.h \
    qadf5355dlg.h \
    qbbdlg.h \
    qiqphasescrolledit.h \
    qarbdlg.h \
    qiqampscrolledit.h \
    qdcoffsetscrolledit.h \
    Cal/qcalbasemodel.h \
    Cal/qcalbasedlg.h \
    Cal/qr1ctxloleakmodel.h \
    Cal/qcalr1ctxloleakdlg.h \
    Cal/qcalr1ctxloleakthread.h \
    Cal/qcalr1ctxsbdlg.h \
    Cal/qr1ctxsbmodel.h \
    Cal/qcalr1ctxsbthread.h \
    Cal/qcalr1ctxfilterdlg.h \
    Cal/qcalr1ctxfilterthread.h \
    qcalplot.h \
    qcalr1ctxfiltertabwidget.h \
    Cal/qr1ctxfiltermodel.h \
    Cal/qcalr1ctxpwrdlg.h \
    Cal/qr1ctxpwrmodel.h \
    Cal/qcalr1ctxpwrthread.h \
    Cal/qcalr1ctxattdlg.h \
    Cal/qcalr1ctxattthread.h \
    Cal/qr1ctxattmodel.h \
    qcalr1ctxatttabwidget.h \
    qrfdlg.h \
    qframeslottrigwidget.h \
    Cal/qcalr1ctxfilteroffsetdlg.h \
    qcalr1ctxfilteroffsettabwidget.h \
    Cal/qr1ctxfilteroffsetmodel.h \
    Cal/qcalr1ctxfilteroffsetthread.h \
    Cal/qcalr1crxfilterdlg.h \
    Cal/qr1crxfiltermodel.h \
    qcalr1crxfiltertabwidget.h \
    Cal/qcalr1crxfilterthread.h \
    Cal/qr1crxrefmodel.h \
    Cal/qcalr1crxrefdlg.h \
    Cal/qcalr1crxrefthread.h \
    Cal/qr1crxattmodel.h \
    qcalr1crxatttabwidget.h \
    Cal/qcalr1crxattdlg.h \
    Cal/qcalr1crxattthread.h \
    Cal/qr1crxfilteroffsetmodel.h \
    qcalr1crxfilteroffsettabwidget.h \
    Cal/qcalr1crxfilteroffsetthread.h \
    Cal/qcalr1crxfilteroffsetdlg.h

FORMS += \
        mainwindow.ui \
    qrfr1cdlg.ui \
    qrfr1cadvdlg.ui \
    qfpgadlg.ui \
    qiqcapdlg.ui \
    qtempctrldlg.ui \
    qrfr1adlg.ui \
    qrfr1aadvdlg.ui \
    qadf5355dlg.ui \
    qarbdlg.ui \
    qbbdlg.ui \
    Cal/qcalr1ctxloleakdlg.ui \
    Cal/qcalr1ctxsbdlg.ui \
    Cal/qcalr1ctxfilterdlg.ui \
    Cal/qcalr1ctxpwrdlg.ui \
    Cal/qcalr1ctxattdlg.ui \
    qframeslottrigwidget.ui \
    Cal/qcalr1ctxfilteroffsetdlg.ui \
    Cal/qcalr1crxfilterdlg.ui \
    Cal/qcalr1crxrefdlg.ui \
    Cal/qcalr1crxattdlg.ui \
    Cal/qcalr1crxfilteroffsetdlg.ui
