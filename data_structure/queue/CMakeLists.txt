aux_source_directory(. DIR_SOURCE)
add_library(queue_lib STATIC ${DIR_SOURCE})
set_target_properties(queue_lib PROPERTIES LINKER_LANGUAGE CXX)