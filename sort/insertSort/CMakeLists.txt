aux_source_directory(. DIR_SOURCE)
add_library(insertsort_lib STATIC ${DIR_SOURCE})
set_target_properties(insertsort_lib PROPERTIES LINKER_LANGUAGE CXX)