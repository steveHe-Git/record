aux_source_directory(. DIR_SOURCE)
add_library(common_lib STATIC ${DIR_SOURCE})
set_target_properties(common_lib PROPERTIES LINKER_LANGUAGE CXX)