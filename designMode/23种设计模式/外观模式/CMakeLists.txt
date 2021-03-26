cmake_minimum_required(VERSION 2.8)

project(facade)

add_definitions(-std=c++11)

set(search_dir ${CMAKE_CURRENT_SOURCE_DIR}/*.cc)
file(GLOB ALLFILE ${search_dir})
message(STATUS "${ALLFILE}")

FOREACH(mainfile IN LISTS ALLFILE)
    get_filename_component(FILENAME ${mainfile} NAME_WE)
    ADD_EXECUTABLE(${FILENAME} ${mainfile})
ENDFOREACH(mainfile)

