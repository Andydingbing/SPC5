include(../include/config.pri)

QT += core gui network svg
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

SUBDIRS = \
sp9500 \
dt3308 \
sp9500x \
sp9500pro

DEFINES += RD_EXCEPTION

TARGET = RDDT
TEMPLATE = app
RC_ICONS = RDDT.ico

INCLUDEPATH += \
chip \
cal \
test \
../rd

UI_DIR = ./ui

LIBS += \
-llog \
-lbd \
-linstr \
-lrd

win32 { LIBS += -lqwtd }
unix  { LIBS += -lrsnrpz -lfftw3 -lqwt}

HEADERS += \
global.h \
cal_widget.h \
cal/q_cal_dlg.h \
cal/q_model_filter_offset.h \
cal/q_model_tx_filter.h \
cal/q_model_tx_loleak.h \
cal/q_model_tx_pwr.h \
cal/q_model_tx_sb.h \
test/q_test_temp_pwr_thread.h \
q_cal_tx_filter_offset_tabwidget.h \
cal/q_model_rx_filter_offset.h \
q_cal_rx_filter_offset_tabwidget.h \
cal/q_cal_base_dlg.h \
cal/q_model_rx_filter.h \
cal/q_model_tx_att.h \
cal/q_cal_tx_att_tabwidget.h \
cal/q_cal_rx_att_tabwidget.h \
cal/q_model_rx_att.h \
cal/q_cal_tx_pwr_widget.h \
cal/q_cal_rx_ref_widget.h \
cal/q_model_rx_ref.h \
cal/q_base_model.h \
test/q_test_freq_res_thread.h \
test/q_model_freq_res.h \
test/q_test_freq_res_widget.h \
test/q_test_rf_thread.h \
test/q_test_tx_io_sw_msg_dlg.h \
cal/q_cal_r1c_tx_temp_comp_thread.h \
cal/q_model_trouble_shoot.hpp

SOURCES += \
global.cpp \
test/q_test_temp_pwr_thread.cpp \
main.cpp \
mainwindow.cpp \
child_widget_helper.cpp \
q_temp_ctrl_dlg.cpp \
q_iq_cap_thread.cpp \
q_device_init_thread.cpp \
q_r1c_temp_ctrl_thread.cpp \
q_winthread.cpp \
q_cal_tx_filter_offset_tabwidget.cpp \
q_cal_rx_filter_offset_tabwidget.cpp \
test/q_test_freq_res_widget.cpp \
test/q_test_tx_io_sw_msg_dlg.cpp

SOURCES += \
cal/q_cal_dlg.cpp \
cal/q_cal_r1c_dlg.cpp \
cal/q_cal_r1c_thread.cpp \
cal/q_cal_r1c_tx_loleak_thread.cpp \
cal/q_cal_r1c_tx_sb_thread.cpp \
cal/q_cal_r1c_tx_filter_thread.cpp \
cal/q_cal_r1c_tx_pwr_thread.cpp \
cal/q_cal_r1c_tx_att_thread.cpp \
cal/q_cal_r1c_tx_filter_offset_thread.cpp \
cal/q_cal_r1c_rx_filter_thread.cpp \
cal/q_cal_r1c_rx_ref_thread.cpp \
cal/q_cal_r1c_rx_att_thread.cpp \
cal/q_cal_r1c_rx_filter_offset_thread.cpp \
cal/q_cal_r1c_tx_temp_comp_thread.cpp \
cal/q_cal_tx_filter_tabwidget.cpp \
cal/q_cal_tx_pwr_widget.cpp \
cal/q_cal_tx_att_tabwidget.cpp \
cal/q_cal_rx_filter_tabwidget.cpp \
cal/q_cal_rx_ref_widget.cpp \
cal/q_cal_rx_att_tabwidget.cpp \
cal/q_model_tx_loleak.cpp \
cal/q_model_tx_sb.cpp \
cal/q_model_tx_filter.cpp \
cal/q_model_tx_att.cpp \
cal/q_model_rx_ref.cpp \
cal/q_model_rx_att.cpp \
cal/q_model_rx_filter_offset.cpp \
test/q_test_dlg.cpp \
test/q_test_freq_res_thread.cpp \
test/q_test_rf_thread.cpp


HEADERS += \
mainwindow.h \
q_log_model.hpp \
child_widget_helper.h \
q_device_init_thread.h \
q_iq_cap_thread.h \
q_r1c_temp_ctrl_model.h \
q_r1c_temp_ctrl_thread.h \
q_temp_ctrl_dlg.h \
q_winthread.h \
cal/q_cal_tx_filter_tabwidget.h \
cal/q_cal_rx_filter_tabwidget.h \
cal/q_cal_r1c_rx_att_thread.h \
cal/q_cal_r1c_rx_filter_offset_thread.h \
cal/q_cal_r1c_rx_filter_thread.h \
cal/q_cal_r1c_rx_ref_thread.h \
cal/q_cal_r1c_tx_att_thread.h \
cal/q_cal_r1c_tx_filter_offset_thread.h \
cal/q_cal_r1c_tx_filter_thread.h \
cal/q_cal_r1c_tx_loleak_thread.h \
cal/q_cal_r1c_tx_pwr_thread.h \
cal/q_cal_r1c_tx_sb_thread.h \
cal/q_cal_r1c_dlg.h \
cal/q_cal_r1c_thread.h

FORMS += \
cal/cal_filter.ui \
test/q_test_temp_pwr_dlg.ui \
mainwindow.ui \
q_temp_ctrl_dlg.ui \
test/q_test_rf_dlg.ui \
test/q_test_tx_io_sw_msg_dlg.ui \
cal/q_cal_rx_temp_comp_dlg.ui

# Common
FORMS += \
common/sp1401_r1a.ui \
common/sp1401_r1a_adv.ui \
common/sp1401_r1c.ui \
common/sp1401_r1c_adv.ui \
common/sp1401_r1f.ui \
common/sp1401_r1f_adv.ui \
common/sp2401_r1a.ui \
common/sp1403_r1a.ui \
common/sp1403_r1b.ui

# SP9500
HEADERS += \
sp9500/sp1401_r1a_widget.h \
sp9500/sp1401_r1a_adv_widget.h \
sp9500/sp1401_r1c_widget.h \
sp9500/sp1401_r1c_adv_widget.h \
sp9500/sp1401_r1f_widget.h \
sp9500/sp1401_r1f_adv_widget.h \
sp9500/sp2401_r1a_widget.h \
sp9500/sp9500_child_widget_helper.hpp \
sp9500/sp9500_arb_config_widget.h \
sp9500/sp9500_iq_cap_config_widget.h \
sp9500/sp9500_fpga_widget.h

SOURCES += \
sp9500/sp1401_r1a_widget.cpp \
sp9500/sp1401_r1a_adv_widget.cpp \
sp9500/sp1401_r1c_widget.cpp \
sp9500/sp1401_r1c_adv_widget.cpp \
sp9500/sp1401_r1f_widget.cpp \
sp9500/sp1401_r1f_adv_widget.cpp \
sp9500/sp2401_r1a_widget.cpp \
sp9500/sp9500_arb_config_widget.cpp \
sp9500/sp9500_iq_cap_config_widget.cpp \
sp9500/sp9500_fpga_widget.cpp

FORMS += \
sp9500/sp9500_fpga.ui

# DT3308
HEADERS += \
dt3308/dt3308_rf_f_widget.h \
dt3308/dt3308_bb_f_widget.h \
dt3308/dt3308_child_widget_helper.hpp \
dt3308/dt3308_iq_cap_config_widget.h

SOURCES += \
dt3308/dt3308_rf_f_widget.cpp \
dt3308/dt3308_bb_f_widget.cpp \
dt3308/dt3308_iq_cap_config_widget.cpp

FORMS += \
dt3308/dt3308_rf_f.ui \
dt3308/dt3308_bb_f.ui

# SP9500X
HEADERS += \
sp9500x/sp1403_r1a_widget.h \
sp9500x/sp1403_r1b_widget.h \
sp9500x/sp9500x_rrh_widget.h \
sp9500x/sp9500x_sp2406_widget.h \
sp9500x/sp9500x_child_widget_helper.hpp \
sp9500x/sp9500x_arb_config_widget.h \
sp9500x/sp9500x_iq_cap_config_widget.h \
sp9500x/sp9500x_fpga_widget.h \
sp9500x/sp9500x_fan_dlg.h \
sp9500x/sp9500x_cal_tx_filter_thread.h \
sp9500x/sp9500x_cal_tx_pwr_thread.h

SOURCES += \
sp9500x/sp1403_r1a_widget.cpp \
sp9500x/sp1403_r1b_widget.cpp \
sp9500x/sp9500x_rrh_widget.cpp \
sp9500x/sp9500x_sp2406_widget.cpp \
sp9500x/sp9500x_arb_config_widget.cpp \
sp9500x/sp9500x_iq_cap_config_widget.cpp \
sp9500x/sp9500x_fpga_widget.cpp \
sp9500x/sp9500x_fan_dlg.cpp \
sp9500x/sp9500x_cal_widget.cpp \
sp9500x/sp9500x_cal_tx_filter_thread.cpp \
sp9500x/sp9500x_cal_tx_pwr_thread.cpp

FORMS += \
sp9500x/sp9500x_rrh.ui \
sp9500x/sp9500x_sp2406.ui \
sp9500x/sp9500x_fpga.ui \
sp9500x/sp9500x_fan.ui \
sp9500x/sp9500x_cal_tx_pwr.ui

# SP9500Pro
INCLUDEPATH += \
sp9500pro/cal

HEADERS += \
sp9500pro/sp9500pro_child_widget_helper.hpp

SOURCES += \

# ARB
# IQ Capture
# Signal Analyzer
HEADERS += \
arb_widget.h \
arb_config_widget.h \
iq_cap_config_widget.h \
signal_analyzer_freq_widget.h \
signal_analyzer_widget.h

SOURCES += \
arb_widget.cpp \
arb_config_widget.cpp \
iq_cap_config_widget.cpp \
signal_analyzer_freq_widget.cpp \
signal_analyzer_widget.cpp

FORMS += \
arb.ui \
signal_analyzer.ui

# Chip
HEADERS += \
chip/hmc832_dlg.h \
chip/adf5355_dlg.h \
chip/lmx2594_dlg.h
SOURCES += \
chip/hmc832_dlg.cpp \
chip/adf5355_dlg.cpp \
chip/lmx2594_dlg.cpp
FORMS += \
chip/hmc832.ui \
chip/adf5355.ui \
chip/lmx2594.ui

# Test
HEADERS += \
test/sp9500x_test_thread.h

SOURCES += \
test/sp9500x_test_thread.cpp

FORMS += \
test/test_sp9500x_baseband.ui

FORMS += \
cal/q_cal_r1c_dlg.ui \
cal/q_cal_tx_loleak_dlg.ui \
cal/q_cal_tx_sb_dlg.ui \
cal/q_cal_tx_filter_dlg.ui \
cal/q_cal_tx_pwr_dlg.ui \
cal/q_cal_tx_att_dlg.ui \
cal/q_cal_tx_filter_offset_dlg.ui \
cal/q_cal_rx_filter_dlg.ui \
cal/q_cal_rx_ref_dlg.ui \
cal/q_cal_rx_att_dlg.ui \
cal/q_cal_rx_filter_offset_dlg.ui \
cal/q_cal_tx_temp_comp_dlg.ui \
test/q_test_freq_res_dlg.ui
