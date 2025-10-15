add_subdirectory(itemScene)

aux_source_directory(. DIR_SYSTEM_SRCS)
add_library(graphicsLib ${DIR_SYSTEM_SRCS})

target_link_libraries(graphicsLib graphicsItemSceneLib)
