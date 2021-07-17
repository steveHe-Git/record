aux_source_directory(. DIR_SOURCE)
add_library(quicksort_lib STATIC ${DIR_SOURCE})
set_target_properties(quicksort_lib PROPERTIES LINKER_LANGUAGE CXX)