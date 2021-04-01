cmake_minimum_required(VERSION 3.5)
project(factory)

add_definitions(-std=c++11)

include_directories("${PROJECT_BINARY_DIR}")

#${CMAKE_CURRENT_SOURCE_DIR} 是相对于CMakeLists.txt所在的绝对路径
file(GLOB_RECURSE mains ${CMAKE_CURRENT_SOURCE_DIR}/*.cc)

message(STATUS ${mains})
message(STATUS ${PROJECT_BINARY_DIR})
message(STATUS ${PROJECT_SOURCE_DIR})

message(STATUS ${CMAKE_BINARY_DIR})
message(STATUS ${CMAKE_SOURCE_DIR})

message(STATUS ${CMAKE_CURRENT_SOURCE_DIR})
message(STATUS ${CMAKE_CURRENT_BINARY_DIR})
message(STATUS ${CMAKE_CURRENT_LIST_FILE})
message(STATUS ${CMAKE_CURRENT_LIST_DIR})
message(STATUS ${CMAKE_CURRENT_LIST_LINE})

foreach(mainfile IN LISTS mains)
    # Get file name without directory
    get_filename_component(mainname ${mainfile} NAME_WE)
    add_executable(${mainname} ${mainfile})
    #target_link_libraries (${mainname} ${PCL_LIBRARIES})
endforeach()