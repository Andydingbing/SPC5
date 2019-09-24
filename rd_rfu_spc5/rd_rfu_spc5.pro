include( ../include/boost.pri )

CONFIG += shared
CONFIG -= qt
TEMPLATE = lib

# Multi-Processor
QMAKE_CXXFLAGS += /MP

LIB_DIR = ../lib
SRC_DIR = ../src/spc5_sp9500
UTILITIES_DIR = ../utilities

INCLUDEPATH += \
    $$SRC_DIR \
    ../include \
    ../include/win \
    ../include/win/pthread \
    ../log \
    ../bd \
    ../utilities \
    $$LIB_DIR/fftw-3.3.4/fftw-3.3.4-dll64 \
    $$LIB_DIR/ftplib

DEFINES += RD_DLL_EXPORTS

LIBS += -lbd -llog

win32 {
    LIBS += \
        Ws2_32.lib \
        ../lib/visa64.lib \
        ../lib/windrvr/amd64/wdapi1020.lib \
        ../lib/pthread/pthreadVC2.lib \
        ../lib/fftw-3.3.4/fftw-3.3.4-dll64/libfftw3-3.lib
}

unix {
    target.path = /usr/lib
    INSTALLS += target
}

CONFIG(debug,debug|release) {
    DEFINES += _DEBUG
    LIBS += -L../Qt/x64/debug/
    DESTDIR = ../Qt/x64/debug
} else {
    LIBS += -L../Qt/x64/release/
    DESTDIR = ../Qt/x64/release
}

HEADERS += \
    $$UTILITIES_DIR/algorithm.h \
    $$UTILITIES_DIR/gen_int.h \
    $$UTILITIES_DIR/gen_ini_file.h \
    $$UTILITIES_DIR/test_data.hpp \
    $$UTILITIES_DIR/test_report.hpp \
    $$UTILITIES_DIR/algo_math.h \
    $$UTILITIES_DIR/algo_chip.h \
    $$UTILITIES_DIR/algo_fit.hpp \
    $$UTILITIES_DIR/spec.h \
    $$LIB_DIR/ftplib/ftplib.h \
    $$SRC_DIR/arb_reader.h \
    $$SRC_DIR/cal_table.h \
    $$SRC_DIR/cal_file.h \
    $$SRC_DIR/cfv_ctrl.h \
    $$SRC_DIR/cfv_ctrl_0000_0001.hpp \
    $$SRC_DIR/cfv_ctrl_0001_0002.hpp \
    $$SRC_DIR/cfv_ctrl_0002_0003.hpp \
    $$SRC_DIR/cfv_ctrl_0003_0004.hpp \
    $$SRC_DIR/cfv_ctrl_0004_0005.hpp \
    $$SRC_DIR/cfv_ctrl_0005_0006.hpp \
    $$SRC_DIR/cal_file_r1ab.h \
    $$SRC_DIR/cal_file_r1cd.h \
    $$SRC_DIR/tx_lol_table.h \
    $$SRC_DIR/tx_sb_table.h \
    $$SRC_DIR/tx_filter_table.h \
    $$SRC_DIR/tx_pwr_table.h \
    $$SRC_DIR/tx_att_table.h \
    $$SRC_DIR/tx_filter_offset_table.h \
    $$SRC_DIR/rx_filter_table.h \
    $$SRC_DIR/rx_ref_table.h \
    $$SRC_DIR/rx_att_table.h \
    $$SRC_DIR/rx_filter_offset_table.h \
    $$SRC_DIR/dma_mgr.h \
    $$SRC_DIR/dsp_buf.h \
    $$SRC_DIR/ftp.h \
    $$SRC_DIR/sp1401.h \
    $$SRC_DIR/sp1401_r1a.h \
    $$SRC_DIR/sp1401_r1b.h \
    $$SRC_DIR/sp1401_r1c.h \
    $$SRC_DIR/sp1401_r1d.h \
    $$SRC_DIR/sp1401_r1e.h \
    $$SRC_DIR/sp1401_r1f.h \
    $$SRC_DIR/sp2401_r1a.h \
    $$SRC_DIR/sp3301.h \
    $$SRC_DIR/sp3501.h \
    rd_rfu_spc5.h

SOURCES += \
    $$UTILITIES_DIR/algorithm.cpp \
    $$UTILITIES_DIR/gen_int.cpp \
    $$UTILITIES_DIR/gen_ini_file.cpp \
    $$UTILITIES_DIR/spec.cpp \
    $$UTILITIES_DIR/algo_math.cpp \
    $$UTILITIES_DIR/algo_chip.cpp \
    $$LIB_DIR/ftplib/ftplib.c \
    $$SRC_DIR/arb_reader.cpp \
    $$SRC_DIR/cal_file.cpp \
    $$SRC_DIR/cfv_ctrl.cpp \
    $$SRC_DIR/cal_file_r1ab.cpp \
    $$SRC_DIR/cal_file_r1cd.cpp \
    $$SRC_DIR/tx_lol_table.cpp \
    $$SRC_DIR/tx_sb_table.cpp \
    $$SRC_DIR/tx_filter_table.cpp \
    $$SRC_DIR/tx_pwr_table.cpp \
    $$SRC_DIR/tx_att_table.cpp \
    $$SRC_DIR/tx_filter_offset_table.cpp \
    $$SRC_DIR/rx_filter_table.cpp \
    $$SRC_DIR/rx_ref_table.cpp \
    $$SRC_DIR/rx_att_table.cpp \
    $$SRC_DIR/rx_filter_offset_table.cpp \
    $$SRC_DIR/dma_mgr.cpp \
    $$SRC_DIR/dsp_buf.cpp \
    $$SRC_DIR/ftp.cpp \
    $$SRC_DIR/sp1401.cpp \
    $$SRC_DIR/sp1401_r1a.cpp \
    $$SRC_DIR/sp1401_r1c.cpp \
    $$SRC_DIR/sp1401_r1e.cpp \
    $$SRC_DIR/sp1401_r1f.cpp \
    $$SRC_DIR/sp2401_r1a.cpp \
    $$SRC_DIR/sp3301.cpp \
    $$SRC_DIR/sp3501.cpp \
    $$SRC_DIR/version_.cpp \
    rd_rfu_spc5.cpp
