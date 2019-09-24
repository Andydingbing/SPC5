include( ../../include/boost.pri )

QT += core gui network
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

DEFINES += RD_EXCEPTION
DEFINES += QWT_DLL
DEFINES += QT_DEPRECATED_WARNINGS

UTILITIES_DIR = ../../utilities

# Set stable.h as the PCH
PRECOMPILED_HEADER = stable.h

# Multi-Processor
QMAKE_CXXFLAGS += /MP

TARGET = SPC5_RFU_Debug_Tool
TEMPLATE = app
RC_ICONS = RFU_Debug_Tool.ico

INCLUDEPATH += \
    Cal \
    Test \
    ../../include/win \
    ../../include \
    ../../log \
    ../../bd \
    ../../instr \
    ../../utilities \
    ../../src/spc5_sp9500 \
    ../../lib/qwt/qwt-6.1.3/src

CONFIG(debug,debug|release) {
    DEFINES += _DEBUG
    LIBS += -L../x64/debug/
    UI_DIR = ./ui
    DESTDIR = ../x64/debug
} else {
    LIBS += -L../x64/release/
    UI_DIR = ./ui
    DESTDIR = ../x64/release
}

win32 {
    LIBS += -L../../lib/qwt/qwt-6.1.3/lib/
    LIBS += \
        ../../lib/visa64.lib \
        ../../lib/windrvr/amd64/wdapi1020.lib \
        ../../lib/pthread/pthreadVC2.lib

    LIBS += -linstr -lbd -llog -lrd_rfu_sp9500
}

unix {
    LIBS += \
        /usr/local/lib64/libvisa.so
        ../../lib/qwt/qwt-6.1.3/lib/libqwt.so

    LIBS += -lrsnrpz -linstr -lbd -llog -lfftw3
}

SOURCES += \
    Test/q_test_temp_pwr_thread.cpp \
        main.cpp \
        mainwindow.cpp \
    q_log_model.cpp \
    q_temp_ctrl_dlg.cpp \
    q_rf_r1a_dlg.cpp \
    q_rf_r1a_adv_dlg.cpp \
    q_rf_r1c_dlg.cpp \
    q_rf_r1c_adv_dlg.cpp \
    q_rf_r1f_dlg.cpp \
    q_rf_r1f_adv_dlg.cpp \
    q_adf5355_dlg.cpp \
    q_bb_dlg.cpp \
    q_arb_dlg.cpp \
    q_iq_cap_dlg.cpp \
    q_iq_cap_thread.cpp \
    q_fpga_dlg.cpp \
    q_attach_thread_dlg.cpp \
    q_device_init_thread.cpp \
    q_frame_slot_trig_widget.cpp \
    q_r1c_temp_ctrl_model.cpp \
    q_r1c_temp_ctrl_thread.cpp \
    q_rdt_tableview.cpp \
    q_winthread.cpp \
    q_led.cpp \
    q_cal_tx_filter_offset_tabwidget.cpp \
    q_cal_rx_filter_offset_tabwidget.cpp \
    $$UTILITIES_DIR/freq_string.cpp \
    Test/q_test_freq_res_widget.cpp \
    q_plot.cpp \
    Test/q_test_tx_io_sw_msg_dlg.cpp

SOURCES += \
    Cal/q_cal_dlg.cpp \
    Cal/q_cal_r1c_dlg.cpp \
    Cal/q_cal_r1c_thread.cpp \
    Cal/q_cal_r1c_tx_loleak_thread.cpp \
    Cal/q_cal_r1c_tx_sb_thread.cpp \
    Cal/q_cal_r1c_tx_filter_thread.cpp \
    Cal/q_cal_r1c_tx_pwr_thread.cpp \
    Cal/q_cal_r1c_tx_att_thread.cpp \
    Cal/q_cal_r1c_tx_filter_offset_thread.cpp \
    Cal/q_cal_r1c_rx_filter_thread.cpp \
    Cal/q_cal_r1c_rx_ref_thread.cpp \
    Cal/q_cal_r1c_rx_att_thread.cpp \
    Cal/q_cal_r1c_rx_filter_offset_thread.cpp \
    Cal/q_cal_tx_filter_tabwidget.cpp \
    Cal/q_cal_tx_pwr_widget.cpp \
    Cal/q_cal_tx_att_tabwidget.cpp \
    Cal/q_cal_rx_filter_tabwidget.cpp \
    Cal/q_cal_rx_ref_widget.cpp \
    Cal/q_cal_rx_att_tabwidget.cpp \
    Cal/q_model_tx_loleak.cpp \
    Cal/q_model_tx_sb.cpp \
    Cal/q_model_tx_filter.cpp \
    Cal/q_model_tx_att.cpp \
    Cal/q_model_rx_ref.cpp \
    Cal/q_model_rx_att.cpp \
    Cal/q_model_rx_filter_offset.cpp \
    Test/q_test_dlg.cpp \
    Test/q_test_freq_res_thread.cpp \
    Test/q_test_rf_thread.cpp

HEADERS += \
    $$UTILITIES_DIR/exception.hpp \
    $$UTILITIES_DIR/freq_string.hpp \
    Cal/q_cal_dlg.h \
    Cal/q_model_filter_offset.h \
    Cal/q_model_tx_filter.h \
    Cal/q_model_tx_loleak.h \
    Cal/q_model_tx_pwr.h \
    Cal/q_model_tx_sb.h \
    Test/q_model_temp_pwr.h \
    Test/q_test_temp_pwr_thread.h \
    q_cal_tx_filter_offset_tabwidget.h \
    Cal/q_model_rx_filter_offset.h \
    q_cal_rx_filter_offset_tabwidget.h \
    Cal/q_cal_base_dlg.h \
    q_log_model.h \
    q_rf_r1f_adv_dlg.h \
    Cal/q_model_rx_filter.h \
    Cal/q_model_tx_att.h \
    Cal/q_cal_tx_att_tabwidget.h \
    Cal/q_cal_rx_att_tabwidget.h \
    Cal/q_model_rx_att.h \
    Cal/q_cal_tx_pwr_widget.h \
    Cal/q_cal_rx_ref_widget.h \
    Cal/q_model_rx_ref.h \
    Cal/q_base_model.h \
    Test/q_test_freq_res_thread.h \
    Test/q_model_freq_res.h \
    Test/q_test_freq_res_widget.h \
    Test/q_test_rf_thread.h \
    q_plot.h \
    Test/q_test_tx_io_sw_msg_dlg.h

HEADERS += \
        mainwindow.h \
    define.h \
    q_adf5355_dlg.h \
    q_arb_dlg.h \
    q_attach_thread_dlg.h \
    q_bb_dlg.h \
    q_device_init_thread.h \
    q_fpga_dlg.h \
    q_frame_slot_trig_widget.h \
    q_iq_cap_dlg.h \
    q_iq_cap_thread.h \
    q_r1c_temp_ctrl_model.h \
    q_r1c_temp_ctrl_thread.h \
    q_rdt_tableview.h \
    q_rf_dlg.h \
    q_rf_r1a_adv_dlg.h \
    q_rf_r1a_dlg.h \
    q_rf_r1c_adv_dlg.h \
    q_rf_r1c_dlg.h \
    q_rf_r1f_dlg.h \
    q_temp_ctrl_dlg.h \
    q_winthread.h \
    Cal/q_cal_tx_filter_tabwidget.h \
    Cal/q_cal_rx_filter_tabwidget.h \
    Cal/q_cal_r1c_rx_att_thread.h \
    Cal/q_cal_r1c_rx_filter_offset_thread.h \
    Cal/q_cal_r1c_rx_filter_thread.h \
    Cal/q_cal_r1c_rx_ref_thread.h \
    Cal/q_cal_r1c_tx_att_thread.h \
    Cal/q_cal_r1c_tx_filter_offset_thread.h \
    Cal/q_cal_r1c_tx_filter_thread.h \
    Cal/q_cal_r1c_tx_loleak_thread.h \
    Cal/q_cal_r1c_tx_pwr_thread.h \
    Cal/q_cal_r1c_tx_sb_thread.h \
    Cal/q_cal_r1c_dlg.h \
    Cal/q_cal_r1c_thread.h \
    q_led.h \
    Cal/q_trouble_shoot_model.hpp \
    q_rf_container_dlg.h \
    q_scrolledit.h

FORMS += \
    Test/q_test_temp_pwr_dlg.ui \
        mainwindow.ui \
    q_temp_ctrl_dlg.ui \
    q_rf_r1a_dlg.ui \
    q_rf_r1a_adv_dlg.ui \
    q_rf_r1c_dlg.ui \
    q_rf_r1c_adv_dlg.ui \
    q_rf_r1f_dlg.ui \
    q_rf_r1f_adv_dlg.ui \
    q_adf5355_dlg.ui \
    q_arb_dlg.ui \
    q_bb_dlg.ui \
    q_fpga_dlg.ui \
    q_iq_cap_dlg.ui \
    q_frame_slot_trig_widget.ui \
    Test/q_test_rf_dlg.ui \
    Test/q_test_tx_io_sw_msg_dlg.ui

FORMS += \
    Cal/q_cal_r1c_dlg.ui \
    Cal/q_cal_tx_loleak_dlg.ui \
    Cal/q_cal_tx_sb_dlg.ui \
    Cal/q_cal_tx_filter_dlg.ui \
    Cal/q_cal_tx_pwr_dlg.ui \
    Cal/q_cal_tx_att_dlg.ui \
    Cal/q_cal_tx_filter_offset_dlg.ui \
    Cal/q_cal_rx_filter_dlg.ui \
    Cal/q_cal_rx_ref_dlg.ui \
    Cal/q_cal_rx_att_dlg.ui \
    Cal/q_cal_rx_filter_offset_dlg.ui \
    Test/q_test_freq_res_dlg.ui
