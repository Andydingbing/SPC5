win32 {
    BOOST_INCLUDE = C:/Boost/include/boost-1_67
    BOOST_LIB = C:/Boost/lib

    INCLUDEPATH += $$BOOST_INCLUDE
}

CONFIG(debug,debug|release) {
    win32 {
        LIBS += $$BOOST_LIB/libboost_date_time-vc141-mt-gd-x64-1_67.lib
        LIBS += $$BOOST_LIB/libboost_timer-vc141-mt-gd-x64-1_67.lib
        LIBS += $$BOOST_LIB/libboost_chrono-vc141-mt-gd-x64-1_67.lib
        LIBS += $$BOOST_LIB/libboost_system-vc141-mt-gd-x64-1_67.lib
        LIBS += $$BOOST_LIB/libboost_filesystem-vc141-mt-gd-x64-1_67.lib
    }
} else {
    win32 {
        LIBS += $$BOOST_LIB/libboost_date_time-vc141-mt-x64-1_67.lib
        LIBS += $$BOOST_LIB/libboost_timer-vc141-mt-x64-1_67.lib
        LIBS += $$BOOST_LIB/libboost_chrono-vc141-mt-x64-1_67.lib
        LIBS += $$BOOST_LIB/libboost_system-vc141-mt-x64-1_67.lib
        LIBS += $$BOOST_LIB/libboost_filesystem-vc141-mt-x64-1_67.lib
    }
}
