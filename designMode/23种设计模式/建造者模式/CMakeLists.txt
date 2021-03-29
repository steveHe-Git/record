cmake_minimum_required(VERSION 2.8)

project(builder)

add_definitions(-std=c++11)

file(GLOB ALL_FILE ${CMAKE_CURRENT_SOURCE_DIR}/*.cc)

foreach(mainfile IN LISTS ALL_FILE)
    get_filename_component(mainname ${mainfile} NAME_WE)
    add_executable(${mainname} ${mainfile})
endforeach(mainfile)