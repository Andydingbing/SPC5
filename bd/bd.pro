include( ../include/boost.pri )

CONFIG += shared
CONFIG -= qt
TEMPLATE = lib

INCLUDEPATH += \
    ../include \
    ../include/win \
    ../include/win/windrvr

DEFINES += RD_DLL_EXPORTS

win32 {
    LIBS += \
        ../lib/visa64.lib \
        ../lib/windrvr/amd64/wdapi1020.lib
}

unix {
    target.path = /usr/lib
    INSTALLS += target

    DEFINES += UNIX
    DEFINES += LINUX
    DEFINES += KERNEL_64BIT
}

CONFIG(debug,debug|release) {
    DESTDIR = ../Qt/x64/debug
} else {
    DESTDIR = ../Qt/x64/release
}

SOURCES += \
    vi_pci_dev.cpp \
    wd_pci_dev.cpp \
    vi_mem_io.cpp \
    gpib_dev.cpp

HEADERS += \
    pci_dev.h \
    vi_pci_dev.h \
    wd_pci_dev.h \
    vi_mem_io.h \
    gpib_dev.h \
    libbd.h
