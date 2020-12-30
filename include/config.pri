# Thrid party libraries
QWT_VER   = 6.1.3
FFTW_VER  = 3.3.8
BOOST_VER = 1_75_0

# Directory defination
UTILITIES_DIR = ../utilities
LIB_DIR = ../lib
LIB_DIR_QWT   = $$LIB_DIR
LIB_DIR_FFTW  = $$LIB_DIR
LIB_DIR_BOOST = $$LIB_DIR

win32 {
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
./sp9500pro \
../rd/sp9500pro \
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
        OBJECTS_DIR = ./x86/debug
    } else {
        DESTDIR = ../x86/release
        OBJECTS_DIR = ./x86/release
    }

    LIB_DIR  = $$LIB_DIR/x86
    win32 { LIBS += -lvisa32 -lrsnrpz_32 }
    unix  { }
} else {
    message("Arch : x64")

    CONFIG(debug,debug|release) {
        OBJECTS_DIR = ./x64/debug
        DESTDIR = ../x64/debug
    } else {
        OBJECTS_DIR = ./x64/release
        DESTDIR = ../x64/release
    }

    LIB_DIR = $$LIB_DIR/x64
    win32 { LIBS += -lvisa64 -lrsnrpz_64 }
    unix  { LIBS += -L/usr/lib64 -lvisa }
}

win32-g++ { make_spec = win32-g++
    LIBS += \
    -lboost_timer \
    -lboost_chrono \
    -lboost_system \
    -lboost_filesystem
}
win32-msvc { make_spec = win32-msvc
    # Multi-Processor
    QMAKE_CFLAGS += /MP
    QMAKE_CFLAGS_DEBUG += /MP
    QMAKE_CXXFLAGS += /MP
    QMAKE_CXXFLAGS_DEBUG += /MP

#    QMAKE_LFLAGS_DEBUG -= /DEBUG
}
linux-g++ { make_spec = linux-g++
    LIBS += \
    -lboost_timer \
    -lboost_chrono \
    -lboost_system \
    -lboost_filesystem
}

LIBS += -L$$LIB_DIR
DESTDIR = $$DESTDIR/$$make_spec
OBJECTS_DIR = $$OBJECTS_DIR/$$make_spec

LIB_DIR       = $$LIB_DIR/$$make_spec
LIB_DIR_QWT   = $$LIB_DIR/qwt/$$QWT_VER
LIB_DIR_FFTW  = $$LIB_DIR/fftw/$$FFTW_VER
LIB_DIR_BOOST = $$LIB_DIR/boost/$$BOOST_VER

CONFIG(debug,debug|release) {
    DEFINES += _DEBUG
}

message($$make_spec)
message($$LIB_DIR)
message($$LIB_DIR_QWT)
message($$LIB_DIR_FFTW)
message($$LIB_DIR_BOOST)
message($$DESTDIR)
message($$OBJECTS_DIR)

LIBS += \
-L$$DESTDIR \
-L$$LIB_DIR \
-L$$LIB_DIR_QWT \
-L$$LIB_DIR_FFTW \
-L$$LIB_DIR_BOOST \
-L../$$LIB_DIR_BOOST
