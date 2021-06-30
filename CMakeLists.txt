cmake_minimum_required(VERSION 3.17)
project(STU_LBX_Manager)

set(CMAKE_CXX_STANDARD 14)

set(LBXMGR_SOURCES
        LbxMgr/LbxMgr.cpp
        LbxMgr/LbxMgr.h
        LbxMgr/LbxMgrExport.cpp
        LbxMgr/LbxMgrExport.h
        LbxMgr/LbxMgrPathsNames.cpp
        LbxMgr/LbxMgrPathsNames.h
        LbxMgr/LbxMgrProcess.cpp
        LbxMgr/LbxMgrProcess.h
        LbxMgr/ATS/ats.h
        LbxMgr/ATS/ats_filenames.c
        LbxMgr/ATS/ats_filenames.h
        LbxMgr/ATS/ats_filesystem.c
        LbxMgr/ATS/ats_filesystem.h
        LbxMgr/ATS/Util_CmdLnArg.cpp
        LbxMgr/ATS/Util_CmdLnArg.h
        LbxMgr/ATS/Util_ProgOperParm.cpp
        LbxMgr/ATS/Util_ProgOperParm.h
        LbxMgr/SDL2/sdl2_test.cpp
        LbxMgr/SDL2/sdl2_test.h
        LbxMgr/SDL2/palette_viewer.cpp
        LbxMgr/SDL2/palette_viewer.h
        LbxMgr/SDL2/sdl_colormap.cpp
        LbxMgr/SDL2/sdl_colormap.h
        LbxMgr/SDL2/mom_main_colormap.h)

# message(WARNING CMAKE_MODULE_PATH: ${CMAKE_MODULE_PATH})
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake")
# message(WARNING CMAKE_MODULE_PATH: ${CMAKE_MODULE_PATH})

set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} C:/Development/STU-LBX_Library/cmake-build-debug)
find_library(LIBLBX_LOCATION LibLBX)
set(LIBLBX_INCLUDE_DIR C:/Development/STU-LBX_Library/LibLBX)
include_directories(${LIBLBX_INCLUDE_DIR})

#C:\Users\jbalcomb\AppData\Local\JetBrains\Toolbox\apps\CLion\ch-0\203.5981.166\bin\cmake\win\share\cmake-3.17\Modules\FindSDL2.cmake
#CLion -> File -> Settings -> CMake -> Profiles -> Environment -> SDL2DIR=E:\devel_lib\SDL2-devel-2.0.14-mingw\SDL2-2.0.14\x86_64-w64-mingw32\
message("ENV{SDL2DIR}: $ENV{SDL2DIR}")
#set(SDL2_PATH "C:/devel_lib/SDL2-devel-2.0.14-mingw/SDL2-2.0.14/x86_64-w64-mingw32")
#set(SDL2_PATH "C:/devel_lib/SDL2-devel-2.0.14-mingw/SDL2-2.0.14/i686-w64-mingw32")
#set(SDL2_PATH "E:/devel_lib/SDL2-devel-2.0.14-mingw/SDL2-2.0.14/x86_64-w64-mingw32")
set(SDL2_PATH "E:/devel_lib/SDL2-devel-2.0.14-mingw/SDL2-2.0.14/i686-w64-mingw32")
find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIR})
message("SDL2_LIBRARY: ${SDL2_LIBRARY}")
message("SDL2_INCLUDE_DIR: ${SDL2_INCLUDE_DIR}")

#set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} E:/devel_lib/SDL2-devel-2.0.14-mingw/SDL2-2.0.14/x86_64-w64-mingw32/lib)
#find_library(SDL2_LOCATION SDL2)
#find_library(SDL2main_LOCATION SDL2main)
#set(SDL2_INCLUDE_DIR E:/devel_lib/SDL2-devel-2.0.14-mingw/SDL2-2.0.14/x86_64-w64-mingw32/include/SDL2)
#include_directories(${SDL2_INCLUDE_DIR})

#set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} E:/devel_lib/SDL2_image-devel-2.0.5-mingw/SDL2_image-2.0.5/x86_64-w64-mingw32/lib)
#find_library(SDL2_IMAGE_LOCATION SDL2_image)
#set(SDL2_IMAGE_INCLUDE_DIR E:/devel_lib/SDL2_image-devel-2.0.5-mingw/SDL2_image-2.0.5/x86_64-w64-mingw32/include/SDL2)
#include_directories(${SDL2_IMAGE_INCLUDE_DIR})

##set(SDL2_Flags "-mwindows -Wl,--no-undefined -static-libgcc")
##set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${SDL2_Flags}")
###set(SDL2_Libs mingw32 SDL2 SDL2main m dinput8 dxguid dxerr8 user32 gdi32 winmm imm32 ole32 oleaut32 shell32 version uuid)
##set(SDL2_Libs mingw32 SDL2 m dinput8 dxguid dxerr8 user32 gdi32 winmm imm32 ole32 oleaut32 shell32 version uuid)
###target_link_libraries(hello_sdl2 ${SDL2_Libs})

#set(CMAKE_EXE_LINKER_FLAGS " -static")
#add_executable(LbxMgr ${LBXMGR_SOURCES})
#target_link_libraries(LbxMgr -static-libgcc -static-libstdc++ ${LIBLBX_LOCATION} ${SDL2main_LOCATION} ${SDL2_LOCATION})
##add_executable(STU_PaletteViewer ${LBXMGR_SOURCES})
##target_link_libraries(STU_PaletteViewer -static-libgcc -static-libstdc++ ${LIBLBX_LOCATION} ${SDL2main_LOCATION} ${SDL2_LOCATION})

add_executable(LbxMgr ${LBXMGR_SOURCES})
target_link_libraries(LbxMgr ${LIBLBX_LOCATION} ${SDL2_LIBRARY})
