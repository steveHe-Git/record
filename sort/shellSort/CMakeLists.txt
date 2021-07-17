aux_source_directory(. DIR_SOURCE)
add_library(shellsort_lib STATIC ${DIR_SOURCE})
set_target_properties(shellsort_lib PROPERTIES LINKER_LANGUAGE CXX)