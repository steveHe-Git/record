cmake_minimum_required(VERSION 2.8)

project(adapter)

add_definitions(-std=c++11)

#if (POLICY CMP0037)
#    cmake_policy(SET CMP0037 OLD)
#endif()

file(GLOB filelist ${CMAKE_CURRENT_SOURCE_DIR}/*.cc)

foreach(filename IN LISTS filelist)
    get_filename_component(name ${filename} NAME_WE)
    add_executable(${name} ${filename})
endforeach(filename)