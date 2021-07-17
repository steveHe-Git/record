SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

set(DEFAULT_BUILD_TYPE "Debug")

file(GLOB filelist in ${CMAKE_CURRENT_SOURCE_DIR}/*.cc)
foreach(filename IN LISTS filelist)
    get_filename_component(project_name ${filename} NAME_WE)
    add_executable(${project_name} ${filename})
    target_link_libraries(${project_name} 
        ${GLIB}
        bubblesort_lib
        selectionsort_lib
        insertsort_lib
        shellsort_lib
        mergesort_lib
        quicksort_lib
        heapsort_lib

        radixsort_lib
        bucketsort_lib
        pthread)
endforeach(filename)