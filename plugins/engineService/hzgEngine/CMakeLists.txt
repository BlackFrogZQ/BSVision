project(plugins_hzgEngine VERSION 0.0.1.0)
set(dllVersionRc "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}_info.rc")
configure_file("${dllVersionRcIn}" "${dllVersionRc}")

file(GLOB uiCpp "${CMAKE_CURRENT_SOURCE_DIR}/widget/*.cpp")
add_library(hzgEngine SHARED hzgEngine.cpp ${uiCpp} ${dllVersionRc})
Macro_SetTargetPositionProperty(hzgEngine ON)

macro_pluginCopyFiles(hzgEngine "${CMAKE_CURRENT_SOURCE_DIR}/UiStyle.hzg.qss" "${appExeDir}/plugins")
macro_pluginCopyCopyTARGETS(hzgEngine BS::Camera "${appExeDir}/plugins")

macro_pluginCopyCommand(hzgEngine "${appExeDir}/plugins")
Macro_windeployqt(hzgEngine "${appExeDir}")