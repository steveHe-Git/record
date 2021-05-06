cmake_minimum_required(VERSION 2.8)

project(bridge)

add_definitions(-std=c++11)

file(GLOB file_list ${CMAKE_CURRENT_SOURCE_DIR}/*.cc)

foreach(file_name IN LISTS file_list)
    get_filename_component(project_name ${file_name} NAME_WE)
    add_executable(${project_name} ${file_name})
endforeach(file_name)