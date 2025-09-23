project(plugins_paraTemplate VERSION 0.0.1.0)
set(dllVersionRc "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}_info.rc")
configure_file("${dllVersionRcIn}" "${dllVersionRc}")
set(dllName "BSParaTemplateEngine")

add_subdirectory(markWidget)
aux_source_directory(. DIR_SYSTEM_SRCS)
add_library(paraTemplate SHARED ${DIR_SYSTEM_SRCS} ${dllVersionRc})
set_target_properties(paraTemplate PROPERTIES OUTPUT_NAME "${dllName}")
Macro_SetTargetPositionProperty(paraTemplate ON)

target_link_libraries(paraTemplate PRIVATE paraTemplate_MarkWidget)

macro_pluginCopyCommand(paraTemplate "${appExeDir}/plugins/${visionEnginePluginDir}")
Macro_windeployqt(paraTemplate "${appExeDir}")

