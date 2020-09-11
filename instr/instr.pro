include( ../include/config.pri )

CONFIG += shared
CONFIG -= qt
TEMPLATE = lib
DEFINES += RD_DLL_EXPORTS
LIBS += -lbd

unix {
    target.path = /usr/lib
    INSTALLS += target
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
