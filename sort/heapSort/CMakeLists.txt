aux_source_directory(. DIR_SOURCE)
add_library(heapsort_lib STATIC ${DIR_SOURCE})
set_target_properties(heapsort_lib PROPERTIES LINKER_LANGUAGE CXX)