#
# Copyright 2018 StarPoint Inc.,Ltd
#

#
# Qt project file of Library "instr".
#

include( ../include/boost.pri )

QT -= core gui
TARGET = instr
TEMPLATE = lib
CONFIG += shared

INCLUDEPATH += ../include       \
               ../include/win   \
               ../bd

DEFINES += RD_DLL_EXPORTS

win32 {
    LIBS += -lbd
    LIBS += $$PWD/../lib/visa64.lib
    LIBS += $$PWD/../lib/rsnrp/rsnrpz_64.lib
}

unix {
    target.path = /usr/lib
    INSTALLS += target
}

CONFIG(debug,debug|release) {
    LIBS += -L$$PWD/../Qt/x64/debug/
    OBJECTS_DIR = $$PWD/x64/debug
    DESTDIR = ../Qt/x64/debug
} else {
    LIBS += -L$$PWD/../Qt/x64/release/
    OBJECTS_DIR = $$PWD/x64/release
    DESTDIR = ../Qt/x64/release
}

SOURCES += \
    ks_e4438c.cpp \
    ks_e4440a.cpp \
    ks_e4443a.cpp \
    ks_e4445a.cpp \
    ks_e8267d.cpp \
    rs_fsup.cpp \
    instr.cpp \
    sa.cpp \
    ks_n5182a.cpp \
    ks_n5182b.cpp \
    ks_n9010a.cpp \
    ks_n9020a.cpp \
    ks_n9030a.cpp \
    rs_nrp.cpp \
    rs_smf100a.cpp

HEADERS += \
    instr.h \
    pm.h \
    sa.h \
    sg.h \
    libinstr.h \
    ks_e4438c.h \
    ks_e4440a.h \
    ks_e4443a.h \
    ks_e4445a.h \
    ks_e8267d.h \
    rs_fsup.h \
    ks_n5182a.h \
    ks_n5182b.h \
    ks_n9010a.h \
    ks_n9020a.h \
    ks_n9030a.h \
    rs_nrp.h \
    rs_smf100a.h
