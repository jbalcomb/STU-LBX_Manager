cmake_minimum_required(VERSION 3.17)
project(STU_LBX_Manager)

set(CMAKE_CXX_STANDARD 14)

set(LBXMGR_SOURCES
        LbxMgr/LbxMgr.cpp
        LbxMgr/LbxMgr.h
        LbxMgr/LbxMgrExport.cpp
        LbxMgr/LbxMgrExport.h
        #LbxMgr/LbxMgrProcess_v1.cpp
        #LbxMgr/LbxMgrProcess_v1.h
        LbxMgr/LbxMgrProcess_v2.cpp
        LbxMgr/LbxMgrProcess_v2.h
        LbxMgr/LbxMgrPathsNames.cpp
        LbxMgr/LbxMgrPathsNames.h
        LbxMgr/ATS/Util_ProgOperParm.cpp
        LbxMgr/ATS/Util_ProgOperParm.h
        LbxMgr/ATS/Util_CmdLnArg.cpp
        LbxMgr/ATS/Util_CmdLnArg.h
        LbxMgr/ATS/ats_filenames.c
        LbxMgr/ATS/ats_filenames.h
        LbxMgr/ATS/ats_filesystem.c
        LbxMgr/ATS/ats_filesystem.h
        #LbxMgr/MoMRTGT__convertLbxToImages.cpp
        #LbxMgr/MoMRTGT__convertLbxToImages.h
        LbxMgr/LbxMgrDecodeRunLengthEncoded.cpp
        LbxMgr/LbxMgrDecodeRunLengthEncoded.h
        LbxMgr/ATS/ats_file_type_bmp.c
        LbxMgr/ATS/ats_file_type_bmp.h
        LbxMgr/ATS/ats_file_type_colormap.c
        LbxMgr/ATS/ats_file_type_colormap.h
        LbxMgr/MoM131/MoM_131_LBX_Files.h
        LbxMgr/LBX_Library_FrontEnd/ClassLBX.cpp
        LbxMgr/LBX_Library_FrontEnd/ClassLBX.h
        LbxMgr/LbxMgrLoadLbxData.cpp
        LbxMgr/LbxMgrLoadLbxData.h
        LbxMgr/LbxMgrExportCsv.cpp
        LbxMgr/LbxMgrExportCsv.h
        LbxMgr/LbxMgrExportGraphics.cpp
        LbxMgr/LbxMgrExportGraphics.h
        LbxMgr/ATS/ats_cli_menu.cpp
        LbxMgr/ATS/ats_cli_menu.h
        LbxMgr/LbxMgrTest.cpp
        LbxMgr/LbxMgrTest.h)

set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} C:/Development/STU-LBX_Library/cmake-build-debug)
find_library(LIBLBX_LOCATION LibLBX)
set(LIBLBX_INCLUDE_DIR C:/Development/STU-LBX_Library/LibLBX)
include_directories(${LIBLBX_INCLUDE_DIR})

#find_package(Doxygen)
if(DOXYGEN_FOUND)
    set(DOXYGEN_INPUT ${LBXMGR_SOURCES})
    set(DOXYGEN_OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)
    add_custom_command(
            OUTPUT ${DOXYGEN_OUTPUT}
            COMMAND ${CMAKE_COMMAND} -E echo_append "Building API Documentation..."
            COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_INPUT}
            COMMAND ${CMAKE_COMMAND} -E echo "Done."
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
            DEPENDS ${DOXYGEN_INPUT}
    )
    add_custom_target(apidoc ALL DEPENDS ${DOXYGEN_OUTPUT})
    add_custom_target(apidoc_forced
            COMMAND ${CMAKE_COMMAND} -E echo_append "Building API Documentation..."
            COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_INPUT}
            COMMAND ${CMAKE_COMMAND} -E echo "Done."
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
            )
endif(DOXYGEN_FOUND)

# Tell cmake where Qt is located
#set(Qt5_DIR  "E:/Qt/Qt5.12.10/5.12.10/mingw73_32/lib/cmake/Qt5")
set(Qt5_DIR  "E:/Qt/Qt5.12.10/5.12.10/mingw73_64/lib/cmake/Qt5")
# Tell cmake to find the modules Qt5Core and Qt5widgets
find_package(Qt5 COMPONENTS Core Widgets REQUIRED)


set(CMAKE_EXE_LINKER_FLAGS " -static")
add_executable(LbxMgr ${LBXMGR_SOURCES})
target_link_libraries(LbxMgr -static-libgcc -static-libstdc++ ${LIBLBX_LOCATION})
target_link_libraries(LbxMgr Qt5::Core Qt5::Widgets)
