cmake_minimum_required(VERSION 3.5)
project(GoogleTest)

# GoogleTest requires at least C++11
set(CMAKE_CXX_STANDARD 11)

file(GLOB fileList "${CMAKE_CURRENT_SOURCE_DIR}/src/gtest/*.cc"
]                  "${CMAKE_CURRENT_SOURCE_DIR}/src/gmock/*.cc")

include_directories(include)
include_directories(common)

add_subdirectory(common)

set(GLIB ${CMAKE_CURRENT_SOURCE_DIR}/lib/libgmock_main.a
         ${CMAKE_CURRENT_SOURCE_DIR}/lib/libgmock.a
         ${CMAKE_CURRENT_SOURCE_DIR}/lib/libgtest_main.a
         ${CMAKE_CURRENT_SOURCE_DIR}/lib/libgtest.a)

foreach(file_name IN LISTS fileList)
  get_filename_component(project_name ${file_name} NAME_WE)
  add_executable(${project_name} ${file_name})
  target_link_libraries(${project_name}
    ${GLIB}
    example_lib
    pthread
    dl
    m
  )
endforeach(file_name)
         

