SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

file(GLOB filelist in ${CMAKE_CURRENT_SOURCE_DIR}/*.cc)
foreach(filename IN LISTS filelist)
    get_filename_component(project_name ${filename} NAME_WE)
    add_executable(${project_name} ${filename})
    target_link_libraries(${project_name} 
        ${GLIB}
        listlink_lib
        common_lib
        pthread)
endforeach(filename)