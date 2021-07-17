aux_source_directory(. DIR_SOURCE)
add_library(mergesort_lib STATIC ${DIR_SOURCE})
set_target_properties(mergesort_lib PROPERTIES LINKER_LANGUAGE CXX)