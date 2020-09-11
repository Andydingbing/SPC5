include( ../include/config.pri )

CONFIG += shared
CONFIG -= qt
TEMPLATE = lib
DEFINES += RD_DLL_EXPORTS

unix {
    target.path = /usr/lib
    INSTALLS += target
}

SOURCES += \
    log.cpp

HEADERS += \
    log.h \
    liblog.h
