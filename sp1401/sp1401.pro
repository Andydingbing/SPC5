#
# Copyright 2018 StarPoint Inc.,Ltd
#

#
# Qt project file of Library "sp1401".
#

include( ../include/boost.pri )

QT -= core gui
TARGET = sp1401
TEMPLATE = lib
CONFIG += shared

INCLUDEPATH += ../include               \
               ../include/win           \
               ../include/win/pthread   \
               ../log                   \
               ../bd                    \
               ../lib/fftw-3.3.4/fftw-3.3.4-dll64

DEFINES += RD_DLL_EXPORTS

win32 {
    LIBS += $$PWD/../lib/visa64.lib \
            $$PWD/../lib/windrvr/amd64/wdapi1020.lib \
            $$PWD/../lib/pthread/pthreadVC2.lib
    LIBS += -lbd
    LIBS += -llog
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

SOURCES += \
    arb_reader.cpp \
    cal_file.cpp \
    cal_file_r1ab.cpp \
    tx_lol_table.cpp \
    tx_sb_table.cpp \
    tx_att_table.cpp \
    tx_pwr_table.cpp \
    rx_ref_table.cpp \
    rx_att_table.cpp \
    tx_filter_table.cpp \
    tx_filter_offset_table.cpp \
    rx_filter_table.cpp \
    rx_filter_offset_table.cpp \
    cfv_ctrl.cpp \
    cal_file_r1cd.cpp \
    dsp_buf.cpp \
    sp1401.cpp \
    sp1401_r1a.cpp \
    sp1401_r1c.cpp

HEADERS += \
    arb_reader.h \
    cal_table.h \
    cal_file_r1ab.h \
    cal_file_r1cd.h \
    cfv_ctrl.h \
    cal_file.h \
    tx_lol_table.h \
    tx_sb_table.h \
    tx_att_table.h \
    tx_pwr_table.h \
    rx_ref_table.h \
    rx_att_table.h \
    tx_filter_table.h \
    tx_filter_offset_table.h \
    rx_filter_table.h \
    rx_filter_offset_table.h \
    cfv_ctrl_0000_0001.hpp \
    cfv_ctrl_0001_0002.hpp \
    cfv_ctrl_0002_0003.hpp \
    cfv_ctrl_0003_0004.hpp \
    cfv_ctrl_0004_0005.hpp \
    cfv_ctrl_0005_0006.hpp \
    dsp_buf.h \
    sp1401.h \
    sp1401_r1a.h \
    sp1401_r1b.h \
    sp1401_r1c.h \
    sp1401_r1d.h \
    libsp1401.h
