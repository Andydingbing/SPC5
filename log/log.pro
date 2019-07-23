include( ../include/boost.pri )

CONFIG += shared
CONFIG -= qt
TEMPLATE = lib

INCLUDEPATH += ../include

DEFINES += RD_DLL_EXPORTS

win32 {
    INCLUDEPATH += ../include/win/pthread
    LIBS += ../lib/pthread/pthreadVC2.lib
}

unix {
    target.path = /usr/lib
    INSTALLS += target
}

CONFIG(debug,debug|release) {
    DEFINES += _DEBUG
    DESTDIR = ../Qt/x64/debug
} else {
    DESTDIR = ../Qt/x64/release
}

SOURCES += \
    log.cpp

HEADERS += \
    liblog.h \
    log.h
