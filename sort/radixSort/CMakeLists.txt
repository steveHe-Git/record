aux_source_directory(. DIR_SOURCE)
add_library(radixsort_lib STATIC ${DIR_SOURCE})
set_target_properties(radixsort_lib PROPERTIES LINKER_LANGUAGE CXX)