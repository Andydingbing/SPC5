include( ../include/config.pri )

CONFIG += shared
CONFIG -= qt
TEMPLATE = lib
DEFINES += RD_DLL_EXPORTS
LIBS += -lbd -llog -lfftw3

win32 { LIBS += -lws2_32 }

# common
HEADERS += \
$$UTILITIES_DIR/spec.h \
rf_driver.h \
frontend.h \
baseband.h \
./chip/sp1403.h \
./common/ftplib.h \
./common/arb_reader.h \
./common/cal_file_memory.h \
./common/common_cal_file.h

SOURCES += \
$$UTILITIES_DIR/spec.cpp \
rf_driver.cpp \
frontend.cpp \
baseband.cpp \
./chip/sp1403.cpp  \
./common/ftplib.c \
./common/arb_reader.cpp \
./common/cal_file_memory.cpp

# spc5,sp9500
HEADERS += \
./sp9500/cal_table.h \
./sp9500/cal_file.h \
./sp9500/cfv_ctrl.h \
./sp9500/cal_file_r1ab.h \
./sp9500/cal_file_r1cd.h \
./sp9500/tx_lol_table.h \
./sp9500/tx_sb_table.h \
./sp9500/tx_filter_table.h \
./sp9500/tx_pwr_table.h \
./sp9500/tx_att_table.h \
./sp9500/tx_filter_offset_table.h \
./sp9500/rx_filter_table.h \
./sp9500/rx_ref_table.h \
./sp9500/rx_att_table.h \
./sp9500/rx_filter_offset_table.h \
./sp9500/tx_tc_table.h \
./sp9500/dma_mgr.h \
./sp9500/complex_sequence.h \
./sp9500/ftp.h \
./sp9500/sp1401.h \
./sp9500/sp1401_r1a.h \
./sp9500/sp1401_r1b.h \
./sp9500/sp1401_r1c.h \
./sp9500/sp1401_r1d.h \
./sp9500/sp1401_r1e.h \
./sp9500/sp1401_r1f.h \
./sp9500/sp2401_r1a.h \
./sp9500/sp3301.h \
./sp9500/sp3501.h \
./sp9500/self_cal_helper.h \
rd_spc5.h \
rd_sp9500.h

SOURCES += \
./sp9500/cal_file.cpp \
./sp9500/cfv_ctrl.cpp \
./sp9500/cal_file_r1ab.cpp \
./sp9500/cal_file_r1cd.cpp \
./sp9500/tx_lol_table.cpp \
./sp9500/tx_sb_table.cpp \
./sp9500/tx_filter_table.cpp \
./sp9500/tx_pwr_table.cpp \
./sp9500/tx_att_table.cpp \
./sp9500/tx_filter_offset_table.cpp \
./sp9500/rx_filter_table.cpp \
./sp9500/rx_ref_table.cpp \
./sp9500/rx_att_table.cpp \
./sp9500/rx_filter_offset_table.cpp \
./sp9500/tx_tc_table.cpp \
./sp9500/dma_mgr.cpp \
./sp9500/complex_sequence.cpp \
./sp9500/sp1401.cpp \
./sp9500/sp1401_r1a.cpp \
./sp9500/sp1401_r1c.cpp \
./sp9500/sp1401_r1e.cpp \
./sp9500/sp1401_r1f.cpp \
./sp9500/sp2401_r1a.cpp \
./sp9500/sp3301.cpp \
./sp9500/sp3501.cpp \
./sp9500/self_cal_helper.cpp \
rd_spc5.cpp \
rd_sp9500.cpp


# dt3308
HEADERS += \
./dt3308/rf_3308_f.h \
./dt3308/bb_3308_f.h \
rd_dt3308.h

SOURCES += \
./dt3308/rf_3308_f.cpp \
./dt3308/bb_3308_f.cpp \
rd_dt3308.cpp


# sp9500x
HEADERS += \
./sp9500x/cal_table_tx_lol.h \
./sp9500x/cal_table_filter.h \
./sp9500x/cal_table_tx_pwr.h \
./sp9500x/cal_table_tx_att.h \
./sp9500x/cal_file_sp9500x.h \
./sp9500x/sp9500x_sp1403.h \
./sp9500x/sp9500x_sp1403_r1a.h \
./sp9500x/sp9500x_sp1403_r1b.h \
./sp9500x/sp9500x_sp3103.h \
./sp9500x/sp3103.h \
./sp9500x/rrh.h \
./sp9500x/sp2406.h \
rd_sp9500x.h

SOURCES += \
./sp9500x/cal_table_tx_lol.cpp \
./sp9500x/cal_table_filter.cpp \
./sp9500x/cal_table_tx_pwr.cpp \
./sp9500x/cal_table_tx_att.cpp \
./sp9500x/cal_file_sp9500x.cpp \
./sp9500x/sp9500x_sp1403.cpp \
./sp9500x/sp9500x_sp1403_r1a.cpp \
./sp9500x/sp9500x_sp1403_r1b.cpp \
./sp9500x/sp9500x_sp3103.cpp \
./sp9500x/sp3103.cpp \
./sp9500x/rrh.cpp \
./sp9500x/sp2406.cpp \
rd_sp9500x.cpp


# sp9500pro
HEADERS += \
./sp9500pro/sp9500pro_sp1403.h \
./sp9500pro/sp9500pro_sp1403_r1a.h \
./sp9500pro/sp9500pro_sp1403_r1b.h

SOURCES += \
./sp9500pro/sp9500pro_sp1403.cpp
