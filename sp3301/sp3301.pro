#
# Copyright 2018 StarPoint Inc.,Ltd
#

#
# Qt project file of Library "sp3301".
#

include( ../include/boost.pri )

QT -= core gui
TARGET = sp3301
TEMPLATE = lib
CONFIG += shared

INCLUDEPATH += ../include       \
               ../include/win   \
               ../log           \
               ../bd            \
               ../sp1401        \
               ../sp2401

DEFINES += RD_DLL_EXPORTS

win32 {
    INCLUDEPATH += $$BOOST_INCLUDE
    LIBS += $$PWD/../lib/visa64.lib \
            $$PWD/../lib/windrvr/amd64/wdapi1020.lib
    LIBS += -lsp1401
    LIBS += -lsp2401
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
    version.cpp \
    dma_mgr.cpp \
    sp3301.cpp

HEADERS +=\
    libsp3301.h \
    dma_mgr.h \
    sp3301.h
