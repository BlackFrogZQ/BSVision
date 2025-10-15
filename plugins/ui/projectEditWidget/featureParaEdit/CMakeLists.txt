add_subdirectory(navigationBar)

aux_source_directory(. DIR_SYSTEM_SRCS)
add_library(featureParaEditLib ${DIR_SYSTEM_SRCS})

target_link_libraries(featureParaEditLib PUBLIC navigationBarLib)