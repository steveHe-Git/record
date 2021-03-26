cmake_minimum_required(VERSION 2.8)
project(decorator)

add_definitions(-std=c++11)

aux_source_directory(. SOURCE_DIR)

ADD_EXECUTABLE(${PROJECT_NAME} ${SOURCE_DIR})