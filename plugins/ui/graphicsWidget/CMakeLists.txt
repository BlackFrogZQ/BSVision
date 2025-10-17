add_subdirectory(setTemplateParaWindow)

aux_source_directory(. DIR_SYSTEM_SRCS)
add_library(graphicsWidgetLib ${DIR_SYSTEM_SRCS})

target_link_libraries(graphicsWidgetLib PUBLIC setTemplateParaWindowLib)