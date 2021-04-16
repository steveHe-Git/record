cmake_minimum_required(VERSION 2.8)

project("delegate")

add_definitions(-std=c++11)

file(GLOB filelist ${CMAKE_CURRENT_SOURCE_DIR}/*.cc)

foreach(filename IN LISTS filelist)
    get_filename_component(name ${filename} NAME_WE)
    add_executable(${name} ${filename})
endforeach(filename)