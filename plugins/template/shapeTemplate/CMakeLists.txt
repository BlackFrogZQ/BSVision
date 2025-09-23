project(plugins_shapeTemplate VERSION 0.0.1.0)
set(dllVersionRc "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}_info.rc")
configure_file("${dllVersionRcIn}" "${dllVersionRc}")
set(dllName "BSShapeTemplateEngine")
set(shapeTemplateRccBasicName "${dllName}")

add_compile_definitions(__bsShapeTemplateRcc="${visionEnginePluginDir}/${shapeTemplateRccBasicName}")

file(GLOB srcs
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/editWidget/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/editWidget/wizard/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/editWidget/wizard/clipper2/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/editWidget/graphics/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/editWidget/graphics/fitItem/*.cpp"
)

add_library(shapeTemplate SHARED ${srcs} ${dllVersionRc})
set_target_properties(shapeTemplate PROPERTIES OUTPUT_NAME "${dllName}")
Macro_SetTargetPositionProperty(shapeTemplate ON)

set(shapeRccQrc "${CMAKE_CURRENT_SOURCE_DIR}/${shapeTemplateRccBasicName}.qrc")
set(shapeRcc "${CMAKE_CURRENT_BINARY_DIR}/${shapeTemplateRccBasicName}.rcc")
Macro_updateQrcFied(shapeTemplate ${shapeRccQrc} ${CMAKE_CURRENT_SOURCE_DIR}/shapeTemplateRes ${CMAKE_CURRENT_SOURCE_DIR})

Macro_rccqt(shapeTemplate ${shapeRccQrc} ${shapeRcc})

macro_pluginCopyCommand(shapeTemplate "${appExeDir}/plugins/${visionEnginePluginDir}")
Macro_windeployqt(shapeTemplate "${appExeDir}")

macro_pluginCopyFiles(shapeTemplate "${shapeRcc}" "${appExeDir}/plugins/${visionEnginePluginDir}")
