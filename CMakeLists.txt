cmake_minimum_required(VERSION 3.17)
project(STU_LBX_Manager)

set(CMAKE_CXX_STANDARD 14)

set(LBXMGR_SOURCES
        LbxMgr/LbxMgr.cpp
        LbxMgr/LbxMgr.h
        LbxMgr/LbxMgrExport.cpp
        LbxMgr/LbxMgrExport.h
        LbxMgr/LbxMgrProcess.cpp
        LbxMgr/LbxMgrProcess.h
        LbxMgr/LbxMgrPathsNames.cpp
        LbxMgr/LbxMgrPathsNames.h
        LbxMgr/ATS/Util_ProgOperParm.cpp
        LbxMgr/ATS/Util_ProgOperParm.h
        LbxMgr/ATS/Util_CmdLnArg.cpp
        LbxMgr/ATS/Util_CmdLnArg.h
        LbxMgr/ATS/ats_filenames.c
        LbxMgr/ATS/ats_filenames.h
        LbxMgr/ATS/ats_filesystem.c
        LbxMgr/ATS/ats_filesystem.h)

set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} C:/Development/STU-LBX_Library/cmake-build-debug)
find_library(LIBLBX_LOCATION LibLBX)
set(LIBLBX_INCLUDE_DIR C:/Development/STU-LBX_Library/LibLBX)
include_directories(${LIBLBX_INCLUDE_DIR})

set(CMAKE_EXE_LINKER_FLAGS " -static")
add_executable(LbxMgr ${LBXMGR_SOURCES})
target_link_libraries(LbxMgr -static-libgcc -static-libstdc++ ${LIBLBX_LOCATION})
