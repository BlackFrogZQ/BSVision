# 基础配置
cmake_minimum_required(VERSION 3.27.0)
project(test_cameraApp VERSION 1.0.0.0)

set(appOutDir ${CMAKE_SOURCE_DIR}/build/test/${PROJECT_NAME})
set(appRcc "${CMAKE_CURRENT_BINARY_DIR}/res.rcc")
set(app "${PROJECT_NAME}App")
set(appExeDir "${appOutDir}/${CMAKE_BUILD_TYPE}")

# 编译器配置（针对 MSVC）
if(MSVC)
    message(STATUS "MSVC " ${MSVC_VERSION})
    add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
    add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")
endif(MSVC)

# Qt环境配置
file(TO_CMAKE_PATH $ENV{QT_Dir} QtDir)
set(CMAKE_PREFIX_PATH "${QtDir}")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5 5.15.2 REQUIRED COMPONENTS Core Gui Widgets Network LinguistTools)

set(Qt5model
    Qt5::Network
    Qt5::Widgets
    Qt5::Gui
    Qt5::Core
)
include_directories(${Qt5Network_INCLUDE_DIRS})
include_directories(${Qt5Widgets_INCLUDE_DIRS})
include_directories(${Qt5Gui_INCLUDE_DIRS})
include_directories(${Qt5Core_INCLUDE_DIRS})

# 自定义BS库配置
find_package(BS 1.0.0.0 REQUIRED COMPONENTS Core Gui Camera Dongle)

Macro_SetOutPath(${appOutDir})

if(BS_FOUND)
    message(STATUS "Found BS " ${BS_VERSION})
endif()

Macro_InitGitInfo()

include_directories(${BSGui_INCLUDE_DIRS})
include_directories(${BSCore_INCLUDE_DIRS})
include_directories(${BSDongle_INCLUDE_DIRS})

# 版本文件配置
set(ConfigFileDir ${CMAKE_CURRENT_BINARY_DIR}/ConfigFiles)
configure_file(${BSCore_versionFileConfig} ${ConfigFileDir}/version.h @ONLY)
include_directories(${ConfigFileDir})

# 可执行文件与链接配置
add_executable(${app} main.cpp visionMainWindow.cpp visionApplication.cpp ${versionRc})
target_link_libraries(${app} PRIVATE BS::Dongle BS::Gui BS::Core ${Qt5model})

# 资源文件复制（配置、样式、语言等）
file(GLOB jsonFiles "${CMAKE_CURRENT_SOURCE_DIR}/*.json")
foreach(jsonfile IN LISTS(jsonFiles))
    macro_pluginCopyFiles(${app} "${jsonfile}" "${appExeDir}")
endforeach()

file(GLOB jsonFiles "${CMAKE_CURRENT_SOURCE_DIR}/plugins/*.json")
macro_pluginCopyFiles(${app} "${jsonFiles}" "${appExeDir}/plugins")

file(GLOB qssFiles "${CMAKE_CURRENT_SOURCE_DIR}/*.qss")
foreach(qssfile IN LISTS(qssFiles))
    macro_pluginCopyFiles(${app} "${qssfile}" "${appExeDir}")
endforeach()

add_custom_command(TARGET ${app} PRE_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory $<TARGET_FILE_DIR:${app}>/Lang
    COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_CURRENT_SOURCE_DIR}/Lang" $<TARGET_FILE_DIR:${app}>/Lang)

# 依赖库部署（Windows平台）
macro_pluginCopyCopyTARGETS(${app} BS::Core "${appExeDir}")
macro_pluginCopyCopyTARGETS(${app} BS::Gui "${appExeDir}")
macro_pluginCopyCopyTARGETS(${app} BS::Dongle "${appExeDir}")
macro_pluginCopyCopyTARGETS(${app} BS::Camera "${appExeDir}/plugins")

foreach(camera_Runtime ${BSCamera_Runtime_Files})
    macro_pluginCopyFiles(${app} "${camera_Runtime}" "${appExeDir}/plugins/Camera")
endforeach()

Macro_windeployqtexe(${app})
Macro_windeployqtOutDir(${app} BS::Core "${appExeDir}")
Macro_windeployqtOutDir(${app} BS::Gui "${appExeDir}")
Macro_windeployqtOutDir(${app} BS::Dongle "${appExeDir}")
Macro_windeployqtOutDir(${app} BS::Camera "${appExeDir}")

# 插件与翻译配置
Macro_SetOutPath(${appOutDir}/plugins)
add_subdirectory(plugins)

set(AppTsFile "${CMAKE_SOURCE_DIR}/build/ts/temp/${PROJECT_NAME}App.ts")
Macro_buildDirsTsqt(${app} ${AppTsFile} "${CMAKE_CURRENT_SOURCE_DIR}")

set(AppJsonTsFile "${CMAKE_SOURCE_DIR}/build/ts/temp/${PROJECT_NAME}AppJson.ts")
Macro_jsonFiedDirs(${app} "${AppJsonTsFile}" "${CMAKE_CURRENT_SOURCE_DIR}" "Lang,res" "'Text','text','Tip','tip','Title','title','unit','Unit','FileTypeName'")

set(allTsFile "${CMAKE_SOURCE_DIR}/build/ts/merge/${PROJECT_NAME}App.ts")
set(chineseQmFile "${appExeDir}/Lang/${PROJECT_NAME}.Chinese.qm")
Macro_mergeTsFile(${app} "${allTsFile}" "${AppJsonTsFile}" "${AppTsFile}" "${BSCore_TsFile}" "${BSGui_TsFile}" "${BSCamera_TsFile}")
Macro_qmqt(${app} "${chineseQmFile}" "${allTsFile}")

# Qt资源文件处理
set(appRccQrc "${CMAKE_CURRENT_SOURCE_DIR}/qml.qrc")
Macro_updateQrcFied(${app} ${appRccQrc} ${CMAKE_CURRENT_SOURCE_DIR}/res ${CMAKE_CURRENT_SOURCE_DIR})
Macro_rccqt(${app} ${appRccQrc} ${appRcc})

macro_pluginCopyFiles(${app} "${appRcc}" "${appExeDir}")