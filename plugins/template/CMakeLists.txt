set(visionEnginePluginDir "visionEngine")
add_subdirectory(paraTemplate)
add_subdirectory(shapeTemplate)

aux_source_directory(. DIR_SYSTEM_SRCS)
add_library(templateLib ${DIR_SYSTEM_SRCS})

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/visionEngine")
add_compile_definitions(__bsVisionEnginePluginDir="${visionEnginePluginDir}")
if(1)
    find_package(BSCV REQUIRED)
    add_library(bscvDepend
        ${CMAKE_CURRENT_SOURCE_DIR}/visionEngine/bscvTool.cpp
    )
    target_link_libraries(bscvDepend PUBLIC ${BSCV_LIBRARYS})
    target_include_directories(bscvDepend PUBLIC ${BSCV_INCLUDE_DIRS})

    foreach(bscvdll IN LISTS(BSCV_DLLS))
        macro_pluginCopyFiles(bscvDepend "${bscvdll}" "${appExeDir}/plugins/${visionEnginePluginDir}")
    endforeach()
    target_link_libraries(paraTemplate PRIVATE bscvDepend)
    target_link_libraries(shapeTemplate PRIVATE bscvDepend)
endif()

set_property(TARGET paraTemplate PROPERTY BUILD_ALWAYS ON)
set_property(TARGET shapeTemplate PROPERTY BUILD_ALWAYS ON)

target_link_libraries(paraTemplate PRIVATE templateLib pluginUiLib BS::Camera BS::Gui BS::Core ${Qt5model})
target_link_libraries(shapeTemplate PRIVATE templateLib pluginUiLib BS::Camera BS::Gui BS::Core ${Qt5model})
