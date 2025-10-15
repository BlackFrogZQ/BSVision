add_subdirectory(projectManager)
add_subdirectory(tool)

aux_source_directory(. DIR_SYSTEM_SRCS)
add_library(pluginSystemLib STATIC ${DIR_SYSTEM_SRCS})

target_link_libraries(pluginSystemLib PUBLIC projectManagerLib sysToolFunLib)