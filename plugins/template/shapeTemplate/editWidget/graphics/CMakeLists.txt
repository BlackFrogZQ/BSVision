add_subdirectory(itemScene)
add_subdirectory(item/positioningTool)
aux_source_directory(. DIR_SYSTEM_SRCS)
add_library(graphicsLib ${DIR_SYSTEM_SRCS})
target_link_libraries(graphicsLib
    graphicsToolItemLib
    graphicsItemSceneLib
    )