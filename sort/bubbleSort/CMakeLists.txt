aux_source_directory(. DIR_SOURCE)
add_library(bubblesort_lib STATIC ${DIR_SOURCE})
set_target_properties(bubblesort_lib PROPERTIES LINKER_LANGUAGE CXX)