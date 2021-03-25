cmake_minimum_required(VERSION 2.8)

project(protoType)

add_definitions(-std=c++11)

aux_source_directory(. source_dir)

add_executable(protoType ${source_dir})