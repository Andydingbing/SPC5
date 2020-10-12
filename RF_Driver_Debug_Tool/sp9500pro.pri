INCLUDEPATH += \
sp9500pro/cal

HEADERS += \
sp9500pro/sp9500pro_sp1403_r1b_widget.h \
sp9500pro/sp9500pro_child_widget_helper.hpp \
sp9500pro/cal/sp9500pro_cal_widget.h \
sp9500pro/cal/sp9500pro_cal_tx_filter_thread.h \
sp9500pro/cal/sp9500pro_cal_tx_filter_childwidgets.h

SOURCES += \
sp9500pro/sp9500pro_sp1403_r1b_widget.cpp \
sp9500pro/cal/sp9500pro_cal_widget.cpp \
sp9500pro/cal/sp9500pro_cal_tx_filter_thread.cpp \
sp9500pro/cal/sp9500pro_cal_tx_filter_childwidgets.cpp

FORMS += \
sp9500pro/sp9500pro_sp1403_r1b.ui
