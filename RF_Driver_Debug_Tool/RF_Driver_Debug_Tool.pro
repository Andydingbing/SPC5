include(../include/config.pri)
include(sp9500.pri)
include(dt3308.pri)
include(sp9500x.pri)

QT += core gui network svg
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

DEFINES += RD_EXCEPTION

TARGET = RF_Driver_Debug_Tool
TEMPLATE = app
RC_ICONS = RF_Driver_Debug_Tool.ico

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
q_rdt_tableview.h \
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
test/q_test_temp_pwr_dlg.ui \
mainwindow.ui \
q_temp_ctrl_dlg.ui \
test/q_test_rf_dlg.ui \
test/q_test_tx_io_sw_msg_dlg.ui \
cal/q_cal_rx_temp_comp_dlg.ui


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
