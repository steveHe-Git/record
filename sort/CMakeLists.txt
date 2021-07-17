cmake_minimum_required(VERSION 2.8)

project(List)

add_definitions(-std=c++11)

include_directories(include)
include_directories(bubbleSort)
include_directories(selectionSort)
include_directories(insertSort)
include_directories(shellSort)
include_directories(mergeSort)
include_directories(quickSort)
include_directories(heapSort)


include_directories(radixSort)
include_directories(bucketSort)

set(GLIB ${CMAKE_CURRENT_SOURCE_DIR}/lib/libgmock_main.a
         ${CMAKE_CURRENT_SOURCE_DIR}/lib/libgmock.a
         ${CMAKE_CURRENT_SOURCE_DIR}/lib/libgtest_main.a
         ${CMAKE_CURRENT_SOURCE_DIR}/lib/libgtest.a)

add_subdirectory(test)
add_subdirectory(bubbleSort)
add_subdirectory(selectionSort)
add_subdirectory(insertSort)
add_subdirectory(shellSort)
add_subdirectory(mergeSort)
add_subdirectory(quickSort)
add_subdirectory(heapSort)


add_subdirectory(radixSort)
add_subdirectory(bucketSort)



