cmake_minimum_required(VERSION 2.6)
project(ioc_simple)

add_definitions(-std=c++11)

aux_source_directory(./ioc_simple/. CPP_LIST)

add_executable(ioc_simple ${CPP_LIST})