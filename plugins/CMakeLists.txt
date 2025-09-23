add_compile_definitions(BS_BUILD_PLUGIN)

set(dllVersionRcIn "${CMAKE_CURRENT_SOURCE_DIR}/dllInfo.rc.in")
set(pluginsDir "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/plugins")
set(CMAKE_DEBUG_POSTFIX "")
set(CMAKE_RELWITHDEBINFO_POSTFIX "")

Macro_InitGitInfo()
include_directories(${BSCamera_INCLUDE_DIRS})

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
add_subdirectory(system)
add_subdirectory(ui)
add_subdirectory(template)

add_subdirectory(engineService/hzgEngine)
add_subdirectory(engineService/jczEngine)

# add_custom_command(TARGET hzgEngine PRE_BUILD
# COMMAND ${CMAKE_COMMAND} -E copy_directory ${bsCamera_RUNTIME_DIRS} )
target_link_libraries(hzgEngine PRIVATE pluginUiLib pluginSystemLib templateLib BS::Camera BS::Gui BS::Core ${Qt5model})
target_link_libraries(jczEngine PRIVATE pluginUiLib pluginSystemLib templateLib BS::Camera BS::Gui BS::Core ${Qt5model})

add_dependencies(${app} hzgEngine jczEngine)
add_dependencies(${app} hzgEngine jczEngine)
add_dependencies(jczEngine paraTemplate shapeTemplate)
