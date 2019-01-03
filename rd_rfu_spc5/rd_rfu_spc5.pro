#
# Copyright 2018 StarPoint Inc.,Ltd
#

#
# Qt project file of Library "rd_rfu_spc5".
#

include( ../include/boost.pri )

QT -= core gui
TARGET = rd_rfu_spc5
TEMPLATE = lib
CONFIG += shared

INCLUDEPATH += ../include               \
               ../include/win           \
               ../include/win/pthread   \
               ../log                   \
               ../bd                    \
               ../sp1401                \
               ../sp2401                \
               ../sp3301                \
               ../sp3501                \
               ../lib/fftw-3.3.4/fftw-3.3.4-dll64

DEFINES += RD_DLL_EXPORTS

win32 {
    LIBS += $$PWD/../lib/visa64.lib \
            $$PWD/../lib/windrvr/amd64/wdapi1020.lib \
            $$PWD/../lib/pthread/pthreadVC2.lib
    LIBS += -lbd
    LIBS += -llog
    LIBS += -lsp1401
    LIBS += -lsp2401
    LIBS += -lsp3301
    LIBS += -lsp3501
    LIBS += $$PWD/../lib/fftw-3.3.4/fftw-3.3.4-dll64/libfftw3-3.lib
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

HEADERS += \
    rd_rfu_spc5.h

SOURCES += \
    rd_rfu_spc5.cpp
