cmake_minimum_required(VERSION 2.8)

project(proxy)

add_definitions(-std=c++11)

aux_source_directory(. DIR_SOURCE)

add_executable(proxy ${DIR_SOURCE})