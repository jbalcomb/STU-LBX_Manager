#-------------------------------------------------
#
# Project created by QtCreator 2012-08-31T19:39:56
#
#-------------------------------------------------

unix:PLATF = Linux
win32-g++:PLATF = Windows
win32-msvc2008:PLATF = Windows

unix:QMAKE_CXXFLAGS += -g -fshort-enums -Wno-unused-parameter
win32-g++:QMAKE_CXXFLAGS += -g -fshort-enums -Wno-unused-parameter
win32-msvc2008:QMAKE_CXXFLAGS +=

unix:QMAKE_LIBS +=
win32-g++:QMAKE_LIBS += -luser32
win32-msvc2008:QMAKE_LIBS += user32.lib

# Python
unix:INCLUDEPATH += /usr/include/python3.1
unix:QMAKE_LIBS += -lpython3.1
win32-g++:INCLUDEPATH += C:/Python32/include
win32-g++:QMAKE_LIBS += C:/Python32/libs/python32.lib
win32-msvc2008:INCLUDEPATH += C:/Python32/include
win32-msvc2008:QMAKE_LIBS += C:/Python32/libs/python32.lib

QT       -= core gui

TARGET = MoMCore
TEMPLATE = lib

#CONFIG += staticlib

DEFINES += MOMCORE_LIBRARY
DEFINES += _CRT_SECURE_NO_WARNINGS

INCLUDEPATH += \
    . \
    ../MoMEditorTemplate

SOURCES += MoMCore.cpp \
    ../MoMEditorTemplate/MoMProcess$${PLATF}.cpp \
    ../swig/mom_wrap.cxx

HEADERS += MoMCore.h

HEADERS += \
    ../MoMEditorTemplate/MoMCatnip.h \
    ../MoMEditorTemplate/MoMCommon.h \
    ../MoMEditorTemplate/MoMController.h \
    ../MoMEditorTemplate/MoMExeBase.h \
    ../MoMEditorTemplate/MoMExeMagic.h \
    ../MoMEditorTemplate/MoMExeWizards.h \
    ../MoMEditorTemplate/MoMGameBase.h \
    ../MoMEditorTemplate/MoMGameCustom.h \
    ../MoMEditorTemplate/MoMGameMemory.h \
    ../MoMEditorTemplate/MoMGameSave.h \
    ../MoMEditorTemplate/MoMGenerated.h \
    ../MoMEditorTemplate/MoMHookManager.h \
    ../MoMEditorTemplate/MoMLbxBase.h \
    ../MoMEditorTemplate/MoMLocation.h \
    ../MoMEditorTemplate/MoMProcess.h \
    ../MoMEditorTemplate/MoMTemplate.h \
    ../MoMEditorTemplate/MoMTerrain.h \
    ../MoMEditorTemplate/MoMUnit.h \
    ../MoMEditorTemplate/MoMUtility.h \
    ../MoMEditorTemplate/TeeRedirecter.h

SOURCES += \
    ../MoMEditorTemplate/MoMCatnip.cpp \
    ../MoMEditorTemplate/MoMController.cpp \
    ../MoMEditorTemplate/MoMExeBase.cpp \
    ../MoMEditorTemplate/MoMExeMagic.cpp \
    ../MoMEditorTemplate/MoMExeWizards.cpp \
    ../MoMEditorTemplate/MoMGameBase.cpp \
    ../MoMEditorTemplate/MoMGameCustom.cpp \
    ../MoMEditorTemplate/MoMGameMemory.cpp \
    ../MoMEditorTemplate/MoMGameSave.cpp \
    ../MoMEditorTemplate/MoMGenerated.cpp \
    ../MoMEditorTemplate/MoMHookManager.cpp \
    ../MoMEditorTemplate/MoMLbxBase.cpp \
    ../MoMEditorTemplate/MoMProcessCommon.cpp \
    ../MoMEditorTemplate/MoMTerrain.cpp \
    ../MoMEditorTemplate/MoMUnit.cpp \
    ../MoMEditorTemplate/MoMUtility.cpp
