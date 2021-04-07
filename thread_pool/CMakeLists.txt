cmake_minimum_required(VERSION 2.8)

project(threadPool)

add_definitions(-std=c++11)

if (POLICY CMP0037)
    cmake_policy(SET CMP0037 OLD)
endif()

file(GLOB FILE_LIST ${CMAKE_CURRENT_SOURCE_DIR}/*.cc)

foreach(mainfile IN LISTS FILE_LIST)
    get_filename_component(mainfile ${mainfile} NAME_WE)
    add_executable(${mainfile} ${FILE_LIST})
    target_link_libraries(${mainfile} pthread)
endforeach(mainfile)