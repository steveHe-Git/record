cmake_minimum_required(VERSION 3.5)
project(factory)

add_definitions(-std=c++11)

include_directories("${PROJECT_BINARY_DIR}")


file(GLOB_RECURSE mains ${CMAKE_CURRENT_SOURCE_DIR} *.cpp)

message(STATUS ${mains})
foreach(mainfile IN LISTS mains)
    # Get file name without directory
    get_filename_component(mainname ${mainfile} NAME_WE)
    add_executable(${mainname} ${mainfile})
    target_link_libraries (${mainname} ${PCL_LIBRARIES})
endforeach()