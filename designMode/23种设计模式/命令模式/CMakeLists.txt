cmake_minimum_required(VERSION 2.8)

project(command)

add_definitions(-std=c++11)

file(GLOB file_list ${CMAKE_CURRENT_SOURCE_DIR}/*.cc)

foreach(filename IN LISTS file_list)
    get_filename_component(project_name ${filename} NAME_WE)
    add_executable(${project_name} ${filename})
endforeach(filename)