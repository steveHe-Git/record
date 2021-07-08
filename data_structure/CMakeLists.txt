cmake_minimum_required(VERSION 2.8)

project(List)

add_definitions(-std=c++11)

include_directories(include)
include_directories(ListLink)

set(GLIB ${CMAKE_CURRENT_SOURCE_DIR}/lib/libgmock_main.a
         ${CMAKE_CURRENT_SOURCE_DIR}/lib/libgmock.a
         ${CMAKE_CURRENT_SOURCE_DIR}/lib/libgtest_main.a
         ${CMAKE_CURRENT_SOURCE_DIR}/lib/libgtest.a)

add_subdirectory(ListLink)

add_subdirectory(test)
