include( ../include/config.pri )

SRC_DIR = ../rd/sp9500

CONFIG += shared
CONFIG -= qt
TEMPLATE = lib
INCLUDEPATH += $$SRC_DIR
DEFINES += RD_DLL_EXPORTS
LIBS += -lbd -llog

win32 { LIBS += -lws2_32 -llibfftw3-3 }
unix  { LIBS += -lfftw3 }

HEADERS += \
$$UTILITIES_DIR/algorithm.h \
$$UTILITIES_DIR/spec.h \
$$SRC_DIR/../common/ftplib.h \
$$SRC_DIR/../common/arb_reader.h \
$$SRC_DIR/cal_table.h \
$$SRC_DIR/cal_file.h \
$$SRC_DIR/cfv_ctrl.h \
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
$$SRC_DIR/tx_tc_table.h \
$$SRC_DIR/dma_mgr.h \
$$SRC_DIR/complex_sequence.h \
$$SRC_DIR/ftp.h \
$$SRC_DIR/../frontend.h \
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
rd_rfu_sp9500.h

SOURCES += \
$$UTILITIES_DIR/algorithm.cpp \
$$UTILITIES_DIR/spec.cpp \
$$SRC_DIR/../common/ftplib.c \
$$SRC_DIR/../common/arb_reader.cpp \
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
$$SRC_DIR/tx_tc_table.cpp \
$$SRC_DIR/dma_mgr.cpp \
$$SRC_DIR/complex_sequence.cpp \
$$SRC_DIR/../frontend.cpp \
$$SRC_DIR/sp1401.cpp \
$$SRC_DIR/sp1401_r1a.cpp \
$$SRC_DIR/sp1401_r1c.cpp \
$$SRC_DIR/sp1401_r1e.cpp \
$$SRC_DIR/sp1401_r1f.cpp \
$$SRC_DIR/sp2401_r1a.cpp \
$$SRC_DIR/sp3301.cpp \
$$SRC_DIR/sp3501.cpp \
rd_rfu_sp9500.cpp
