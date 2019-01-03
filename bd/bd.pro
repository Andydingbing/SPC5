#
# Copyright 2018 StarPoint Inc.,Ltd
#

#
# Qt project file of library "bd".
#

QT -= core gui
TARGET = bd
TEMPLATE = lib
CONFIG += shared

INCLUDEPATH += ../include       \
               ../include/win   \
               ../include/win/windrvr

DEFINES += RD_DLL_EXPORTS

win32 {
    LIBS += $$PWD/../lib/visa64.lib \
            $$PWD/../lib/windrvr/amd64/wdapi1020.lib
}

unix {
    target.path = /usr/lib
    INSTALLS += target

    DEFINES += UNIX
    DEFINES += LINUX
    DEFINES += KERNEL_64BIT
}

CONFIG(debug,debug|release) {
    OBJECTS_DIR = $$PWD/x64/debug
    DESTDIR = ../Qt/x64/debug
} else {
    OBJECTS_DIR = $$PWD/x64/release
    DESTDIR = ../Qt/x64/release
}

SOURCES += \
    vi_pci_dev.cpp \
    wd_pci_dev.cpp \
    vi_mem_io.cpp \
    gpib_dev.cpp \
    algorithm.cpp \
    fraction_float.cpp \
    gen_ini_file.cpp \
    gen_int.cpp

HEADERS += \
    pci_dev.h \
    vi_pci_dev.h \
    wd_pci_dev.h \
    vi_mem_io.h \
    gpib_dev.h \
    libbd.h \
    algorithm.h \
    fraction_float.h \
    gen_ini_file.h \
    gen_int.h
