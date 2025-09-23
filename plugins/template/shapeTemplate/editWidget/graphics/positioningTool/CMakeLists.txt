add_subdirectory(twoCircle)
add_subdirectory(rightAngle)
add_subdirectory(lineCenter)
aux_source_directory(. DIR_SYSTEM_SRCS)
add_library(graphicsToolItemLib ${DIR_SYSTEM_SRCS})
target_link_libraries(graphicsToolItemLib
    graphicsTwoCircleToolItemLib
    graphicsRightAngleToolItemLib
    graphicsLineCenterToolItemLib)