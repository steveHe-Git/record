aux_source_directory(. DIR_SOURCE)
add_library(selectionsort_lib STATIC ${DIR_SOURCE})
set_target_properties(selectionsort_lib PROPERTIES LINKER_LANGUAGE CXX)