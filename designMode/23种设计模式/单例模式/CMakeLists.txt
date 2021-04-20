cmake_minimum_required(VERSION 2.8)

project(singleton)

add_definitions(-std=c++11)

set(CMAKE_CXX_FLAGS "-Wextra -Wall -ansi -pedantic -fno-elide-constructors")

file(GLOB filelist ${CMAKE_CURRENT_SOURCE_DIR}/*.cc)

foreach(filename IN LISTS filelist)
    get_filename_component(project_name ${filename} NAME_WE)
    add_executable(${project_name} ${filename})
    target_link_libraries(${project_name} pthread)
endforeach(filename)