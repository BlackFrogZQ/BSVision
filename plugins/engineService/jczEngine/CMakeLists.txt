project(plugins_jczEngine VERSION 0.0.1.0)
set(dllVersionRc "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}_info.rc")
configure_file("${dllVersionRcIn}" "${dllVersionRc}")

file(GLOB uiCpp "${CMAKE_CURRENT_SOURCE_DIR}/widget/*.cpp")
add_library(jczEngine SHARED jczEngine.cpp ${uiCpp} ${dllVersionRc})
Macro_SetTargetPositionProperty(jczEngine ON)

macro_pluginCopyFiles(jczEngine "${CMAKE_CURRENT_SOURCE_DIR}/UiStyle.jcz.qss" "${appExeDir}/plugins")

macro_pluginCopyCommand(jczEngine "${appExeDir}/plugins")
Macro_windeployqt(jczEngine "${appExeDir}")

# hzgEngine 已经拷贝了
# macro_pluginCopyCopyTARGETS(jczEngine BS::camera "${appExeDir}/plugins")
# add_custom_command(TARGET jczEngine POST_BUILD
#     COMMAND ${CMAKE_COMMAND} -E make_directory "${appExeDir}/plugins/Camera/Basler"
#     COMMAND ${CMAKE_COMMAND} -E copy_directory "${bsCamera_Basler_RUNTIME_DIRS}" "${appExeDir}/plugins/Camera/Basler"
#     COMMAND ${CMAKE_COMMAND} -E make_directory "${appExeDir}/plugins/Camera/Hikvision"
#     COMMAND ${CMAKE_COMMAND} -E copy_directory "${bsCamera_Hikvision_RUNTIME_DIRS}" "${appExeDir}/plugins/Camera/Hikvision")