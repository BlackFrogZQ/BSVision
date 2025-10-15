add_subdirectory(graphics)

aux_source_directory(. DIR_SYSTEM_SRCS)
add_library(graphicsViewLib ${DIR_SYSTEM_SRCS})

target_link_libraries(graphicsViewLib PUBLIC graphicsLib)