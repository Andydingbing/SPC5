include( ../include/boost.pri )

CONFIG += shared
CONFIG -= qt
TEMPLATE = lib

INCLUDEPATH += \
    ../include \
    ../include/win \
    ../bd \
    ../utilities

DEFINES += RD_DLL_EXPORTS

LIBS += -lbd

win32 {
    LIBS += \
        ../lib/visa64.lib \
        ../lib/rsnrp/rsnrpz_64.lib
}

unix {
    target.path = /usr/lib
    INSTALLS += target
}

CONFIG(debug,debug|release) {
    LIBS += -L../Qt/x64/debug/
    DESTDIR = ../Qt/x64/debug
} else {
    LIBS += -L../Qt/x64/release/
    DESTDIR = ../Qt/x64/release
}

SOURCES += \
    rs_fsup.cpp \
    instr.cpp \
    sa.cpp \
    rs_nrp.cpp \
    ks_sg.cpp \
    ks_sa.cpp

HEADERS += \
    instr.h \
    pm.h \
    sa.h \
    sg.h \
    libinstr.h \
    rs_fsup.h \
    rs_nrp.h \
    rs_smf100a.h \
    ks_sg.h \
    ks_sa.h
