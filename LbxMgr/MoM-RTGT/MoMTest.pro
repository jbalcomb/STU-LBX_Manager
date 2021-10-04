#-------------------------------------------------
#
# Project created by QtCreator 2012-03-18T20:14:15
#
#-------------------------------------------------

unix:PLATF = Linux
win32-g++:PLATF = Windows
win32-msvc2008:PLATF = Windows
message(PLATF: $$PLATF)

unix:QMAKE_CXXFLAGS += -g -fshort-enums -Wno-unused-parameter -Wno-sign-compare
win32-g++:QMAKE_CXXFLAGS += -g -fshort-enums -Wno-unused-parameter -Wno-sign-compare
win32-msvc2008:QMAKE_CXXFLAGS +=
message(QMAKE_CXXFLAGS: $$QMAKE_CXXFLAGS)

unix:QMAKE_LIBS +=
win32-g++:QMAKE_LIBS += -luser32
win32-msvc2008:QMAKE_LIBS += user32.lib
message(QMAKE_LIBS: $$QMAKE_LIBS)

QT       += testlib widgets

TARGET = MoMTest

DEFINES += VERSION=\\\"Test\\\"

CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

INCLUDEPATH += \
    Applications/MoMTweaker 

include(MoMTweaker.pri)

# Additional platform specific sources
SOURCES += \
    Applications/MoMTest/Test/tst_momcombat.cpp \
    Applications/MoMTest/Test/test_utility.cpp

# Test sources
SOURCES += \
    Applications/MoMTest/Test/test_main.cpp \
    Applications/MoMTest/Test/tst_momgamebase.cpp \
    Applications/MoMTest/Test/tst_momtweaker_mainwindow.cpp

HEADERS += \
    Applications/MoMTest/Test/test_utility.h
