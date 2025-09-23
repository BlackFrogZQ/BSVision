add_subdirectory(stationProcess)

aux_source_directory(. DIR_SYSTEM_SRCS)
add_library(imageProcessLib STATIC ${DIR_SYSTEM_SRCS})

target_link_libraries(imageProcessLib
        stationProcessLib)