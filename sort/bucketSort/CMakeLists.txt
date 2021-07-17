aux_source_directory(. DIR_SOURCE)
add_library(bucketsort_lib STATIC ${DIR_SOURCE})
set_target_properties(bucketsort_lib PROPERTIES LINKER_LANGUAGE CXX)