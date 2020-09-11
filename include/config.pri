# Thrid party libraries
BOOST_VER = 1_67_0
FFTW_VER  = 3.3.5
QWT_VER   = 6.1.3

# Directory defination
UTILITIES_DIR = ../utilities
LIB_DIR = ../lib
BOOST_LIB   = $$LIB_DIR
FFTW_LIB    = $$LIB_DIR
QWT_LIB     = $$LIB_DIR
PTHREAD_LIB = $$LIB_DIR

win32 {
    INCLUDEPATH += \
        ../include/win \
        ../include/win/windrvr \
}

unix {
}

INCLUDEPATH += \
../include/boost/$$BOOST_VER \
../../include/boost/$$BOOST_VER \
../include/qwt/$$QWT_VER \
./chip \
../rd/chip \
./common \
../rd/common \
./common/analyzer \
../rd/common/analyzer \
./dt3308 \
../rd/dt3308 \
./sp9500 \
../rd/sp9500 \
./sp9500x \
../rd/sp9500x \
../include \
../../include \
../bd \
../log \
../../log \
../instr \
../utilities \
../utilities/ctd \
../../utilities \
../../utilities/ctd

contains(QT_ARCH,i386) {
    message("Arch : x86")

    CONFIG(debug,debug|release) {
        DESTDIR = ../x86/debug
        LIBS += -L../x86/debug
        OBJECTS_DIR = ./x86/debug
    } else {
        DESTDIR = ../x86/release
        LIBS += -L../x86/release
        OBJECTS_DIR = ./x86/release
    }

    LIB_DIR  = $$LIB_DIR/x86
    win32 { LIBS += -lvisa32 -lrsnrpz_32 }
    unix  { }
} else {
    message("Arch : x64")

    CONFIG(debug,debug|release) {
        LIBS += -L../x64/debug
        OBJECTS_DIR = ./x64/debug
        DESTDIR = ../x64/debug
    } else {
        LIBS += -L../x64/release
        OBJECTS_DIR = ./x64/release
        DESTDIR = ../x64/release
    }

    LIB_DIR = $$LIB_DIR/x64
    win32 { LIBS += -lvisa64 -lrsnrpz_64 }
    unix  { LIBS += -L/usr/lib64 -lvisa }
}

LIBS += -L$$LIB_DIR
FFTW_LIB = $$LIB_DIR/fftw/$$FFTW_VER

win32-g++ {
    message("win32-g++")

    LIB_DIR = $$LIB_DIR/win32-g++

    LIBS += \
    -lboost_filesystem \
    -lboost_system \
    -lboost_timer \
    -lboost_chrono
}
win32-msvc {
    message("win32-msvc")

    # Multi-Processor
    QMAKE_CXXFLAGS += /MP

    INCLUDEPATH += ../include/win/pthread

    LIB_DIR = $$LIB_DIR/win32-msvc

    LIBS += -lpthreadVC2
}
linux-g++ {
    message("linux-g++")

    LIB_DIR = $$LIB_DIR/linux-g++

    LIBS += \
    -lboost_filesystem \
    -lboost_system \
    -lboost_timer \
    -lboost_chrono
}

BOOST_LIB = $$LIB_DIR/boost/$$BOOST_VER
QWT_LIB   = $$LIB_DIR/qwt/$$QWT_VER

CONFIG(debug,debug|release) {
    DEFINES += _DEBUG
}

message($$LIB_DIR)
message($$BOOST_LIB)
message($$FFTW_LIB)
message($$QWT_LIB)
LIBS += \
-L$$LIB_DIR \
-L../$$BOOST_LIB \
-L$$BOOST_LIB \
-L$$FFTW_LIB \
-L$$PTHREAD_LIB \
-L$$QWT_LIB
