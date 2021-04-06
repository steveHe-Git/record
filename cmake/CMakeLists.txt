# cmake_example (cmake示例)
# 指名编译这个cmakelists所需要的cmake的最低版本
cmake_minimum_required (VERSION 2.8)

# 如果是In-source(直接./编译)，CMAKE_SOURCE_DIR和CMAKE_BINARY_DIR一样，这种模式不支持
# 如果是not In-source(新建./build文件夹编译)，CMAKE_SOURCE_DIR和CMAKE_BINARY_DIR不一样，这种模式支持
# FATAL_ERROR输出错误，并且立即停止继续向下cmake
if("${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_BINARY_DIR}")
    message(FATAL_ERROR "
    FATAL: In-source builds are not allowed.
           You should create a separate directory for build files.
           ")
endif()

project (fj_rtsp_server)

# Strip用赖降低应用程序的大小
# 设置cmake_policy(SET CMP0046 OLD)用来解决add_dependencies()依赖的库不存在的error或者warnning
# revmoe warning of add_dependencies:
# The dependency target "Strip" of target "fj_rtsp_server" does not exist
if (POLICY CMP0046)
    cmake_policy(SET CMP0046 OLD)
endif()

# 设置CMAKE_MODULE_PATH的路径为${CMAKE_CURRENT_SOURCE_DIR}/cmake
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
# 执行functions.cmake
include(functions)

#将./src下面的.cpp文件连接到DIR_SRCS变量中(不包含子目录文件)
aux_source_directory(src DIR_SRCS)

set(VERSION_MAJOR 1)
set(VERSION_CHILD 0)
set(VERSION_MINOR 0)
#设置版本号
set(MAIN_VERSION ${VERSION_MAJOR}.${VERSION_CHILD}.${VERSION_MINOR})

#CMAKE_VERBOSE_MAKEFILE被默认初始化成false通过project()命令，输出编译输出的详细链接信息
set(CMAKE_VERBOSE_MAKEFILE false)
#-DCMAKE_BUILD_TYPE=RELEASE可以通过 cmake编译的参数来指定
set(CMAKE_BUILD_TYPE "Debug")

#如果设置CMAKE_BUILD_TYPE后面的config为Debug，则CMAKE_C_FLAGS_DEBUG伴随产生，RELEASE同理
if( CMAKE_BUILD_TYPE STREQUAL "Debug" )
    #toolchain设置了CMAKE_SYSTEM_NAME，这CMAKE_CROSSCOMPILING则认为为true
    IF (CMAKE_CROSSCOMPILING)
	#将环境变量以及后面设置的 -O0 -Wall...等链接到变量CMAKE_C_FLAGS_DEBUG中
        set(CMAKE_C_FLAGS_DEBUG "$ENV{CFLAGS} -O0 -Wall -g -ggdb -Wno-unused-variable -Wno-int-to-pointer-cast -Wno-pointer-to-int-cast -Wno-unused-but-set-variable -Wno-unused-label -Wno-unused-function")
        set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CFLAGS} -O0 -Wall -g -ggdb -fpermissive")
    ELSE (CMAKE_CROSSCOMPILING)
        set(CMAKE_C_FLAGS_DEBUG "$ENV{CFLAGS} -O0 -Wall -g -ggdb -Wno-unused-variable -Wno-int-to-pointer-cast -Wno-pointer-to-int-cast -Wno-unused-label -Wno-unused-function")
        set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CFLAGS} -O0 -Wall -g -ggdb -fpermissive")
    ENDIF(CMAKE_CROSSCOMPILING)
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    set(CMAKE_C_FLAGS_RELEASE "$ENV{CFLAGS} -O3 -Wall")
    set(CMAKE_CXX_FLAGS_RELEASE "$ENV{CFLAGS} -O3 -Wall -fpermissive")
endif()

#CMAKE_C_FLAGS_DEBUG其实就是将变量链接的东西写到CMAKE_C_FLAGS变量中
set(CMAKE_C_FLAGS ${CMAKE_C_FLAGS} -std=c99)
set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} -std=c++11)

#执行GetGitRevisionDescription.cmake
include(GetGitRevisionDescription)
#执行GetCompilerInfoString.cmake
include(GetCompilerInfoString)
get_git_head_revision(GIT_VERSION, GIT_VERSION_SHA1)
get_compiler_info_string(COMPILER_INFO)
string(SUBSTRING ${GIT_VERSION_SHA1} 0 7 GIT_VERSION_SHA1_SHORT)

#命令用于加入一个配置头文件version.h
configure_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/version.h.in
        ${CMAKE_CURRENT_SOURCE_DIR}/include/version.h)

#命令用于加入一个配置头文件config.h
configure_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/config.h.in
        ${CMAKE_CURRENT_SOURCE_DIR}/include/config.h)

option(RUN_STYCLE_CHECK "Run astyle tool for linux c StyleGuide." OFF)
option(RUN_CPPCHECK "Run cppcheck C++ static analysis tool." OFF)
#执行cstyle.make
include(cstyle)

if (RUN_CPPCHECK)
    #如果定义需要进行cppcheck, 通过find_program()去查找应用程序cppcheck
    find_program(CPPCHECK_EXECUTABLE NAMES cppcheck)
    if (CPPCHECK_EXECUTABLE)
        # add a cppcheck target to the "all" target
        add_custom_target(RUN_cppcheck
            #表示目标立即每次编译的时候都会执行
	    ALL
	    # -j表示指定使用的线程数，--enable=all允许所有的信息打印，
            COMMAND ${CPPCHECK_EXECUTABLE} -j 8 cppcheck --enable=all --quiet ${CPPCHECK_ARG_TEMPLATE} ${PROJECT_SOURCE_DIR}/src
            )
	#执行shell脚本，指定--version的输出到CPPCHECK_VERSION变量中，OUTPUT_STRIP_TRAILING_WHITESPACE指名输出的结尾不以空格结尾
        execute_process(COMMAND "${CPPCHECK_EXECUTABLE}" --version OUTPUT_VARIABLE CPPCHECK_VERSION OUTPUT_STRIP_TRAILING_WHITESPACE)
        message(STATUS "Found Cppcheck: ${CPPCHECK_EXECUTABLE} ${CPPCHECK_VERSION}")
    else (CPPCHECK_EXECUTABLE)
        message(STATUS "Could NOT find cppcheck")
    endif (CPPCHECK_EXECUTABLE)
else (RUN_CPPCHECK)
    message(STATUS "RUN_CPPCHECK OFF")
endif (RUN_CPPCHECK)

#如果指定了交叉编译。默认为3519编译
if (CMAKE_CROSSCOMPILING)
    set (LOCALCMAKE_TOOLCHAIN "3519a")
endif (CMAKE_CROSSCOMPILING)

if (LOCALCMAKE_TOOLCHAIN STREQUAL "3559a")
    include_directories(include/hi/3559a)
    set(HI_SDK_LIB "${CMAKE_SOURCE_DIR}/lib/3559a/hi")
    set(GLOG_LIB "${CMAKE_SOURCE_DIR}/lib/3559a/libglog.a")
    set(GFLAGS_LIB "${CMAKE_SOURCE_DIR}/lib/3559a/libgflags.a")
    set(THIRDLIB_DIR "${CMAKE_CURRENT_SOURCE_DIR}/lib/3559a")
elseif (LOCALCMAKE_TOOLCHAIN STREQUAL "3519a")
    include_directories(include/hi/3519a)
    set(HI_SDK_LIB "${CMAKE_SOURCE_DIR}/lib/3519a/hi")
    set(THIRDLIB_DIR "${CMAKE_CURRENT_SOURCE_DIR}/lib/3519a")
    set(GLOG_LIB "${CMAKE_SOURCE_DIR}/lib/3519a/libglog.a")
    set(GFLAGS_LIB "${CMAKE_SOURCE_DIR}/lib/3519a/libgflags.a")
    set(PROTOBUF_LITE_LIB "${CMAKE_SOURCE_DIR}/lib/3519a/libprotobuf-lite.a")
    set(PROTOBUF_LIB "${CMAKE_SOURCE_DIR}/lib/3519a/libprotobuf.a")
    set(SRT_LIB "${CMAKE_SOURCE_DIR}/lib/3519a/libsrt.a")
    set(OPENCV_LIB "${CMAKE_SOURCE_DIR}/lib/3519a/opencv")
elseif (LOCALCMAKE_TOOLCHAIN STREQUAL "1804")
    set(THIRDLIB_DIR "${CMAKE_CURRENT_SOURCE_DIR}/lib/x86/18.04")
    set(GLOG_LIB "${CMAKE_SOURCE_DIR}/lib/x86/18.04/libglog.a")
    set(GFLAGS_LIB "${CMAKE_SOURCE_DIR}/lib/x86/18.04/libgflags.a")
    set(PROTOBUF_LITE_LIB "${CMAKE_SOURCE_DIR}/lib/x86/18.04/libprotobuf-lite.a")
    set(SRT_LIB "${CMAKE_SOURCE_DIR}/lib/x86/18.04/libsrt.a")
elseif (LOCALCMAKE_TOOLCHAIN STREQUAL "1604")
    set(THIRDLIB_DIR "${CMAKE_CURRENT_SOURCE_DIR}/lib/x86/16.04")
    set(GLOG_LIB "${CMAKE_SOURCE_DIR}/lib/x86/16.04/libglog.a")
    set(GFLAGS_LIB "${CMAKE_SOURCE_DIR}/lib/x86/16.04/libgflags.a")
    set(PROTOBUF_LITE_LIB "${CMAKE_SOURCE_DIR}/lib/x86/16.04/libprotobuf-lite.a")
    set(SRT_LIB "${CMAKE_SOURCE_DIR}/lib/x86/16.04/libsrt.a")
endif()

set(LIVE555_LIB
        ${THIRDLIB_DIR}/libliveMedia.a
        ${THIRDLIB_DIR}/libgroupsock.a
        ${THIRDLIB_DIR}/libBasicUsageEnvironment.a
        ${THIRDLIB_DIR}/libUsageEnvironment.a
        )

set(BOOST_LIB
    ${THIRDLIB_DIR}/libboost_thread.a
    ${THIRDLIB_DIR}/libboost_system.a
    ${THIRDLIB_DIR}/libboost_chrono.a
    ${THIRDLIB_DIR}/libboost_filesystem.a
    )

set(GTEST_LIB
    ${THIRDLIB_DIR}/libgtest.a
    ${THIRDLIB_DIR}/libgtest_main.a
    ${THIRDLIB_DIR}/libgmock.a
    ${THIRDLIB_DIR}/libgmock_main.a
    )

set(JPEG_LIB
    ${THIRDLIB_DIR}/libturbojpeg.a
    )

set(FREETYPE_LIB
    ${THIRDLIB_DIR}/libfreetype.a
    )

include_directories(include)
#include_directories(include/AprilTags)
include_directories(usr/local/include)
include_directories(core/proto)
include_directories(src/network)
include_directories(core/net/server)
include_directories(core/net/client)
include_directories(live/liveMedia/include/)
include_directories(live/groupsock/include/)
include_directories(live/UsageEnvironment/include/)
include_directories(live/BasicUsageEnvironment/include/)
include_directories(jpeg_turbo/include)
include_directories(freetype/include)
include_directories(src/camera)
include_directories(src/camera/hi_sdk)
include_directories(src/camera/hi_sdk/nnie)
include_directories(src/proxy)
include_directories(src/vcu)
include_directories(src/ecu)
include_directories(src/ota)
include_directories(src/nav)
include_directories(src/common)
include_directories(src/timer)
include_directories(src/simulate)
include_directories(core/rtsp)

add_subdirectory(core)
add_subdirectory(src)

add_executable(fj_rtsp_server ${DIR_SRCS})

if (CMAKE_CROSSCOMPILING)
    target_link_libraries(fj_rtsp_server
                    src-modules
                    ros-core
                    ${LIVE555_LIB}
                    ${SRT_LIB}
                    pthread
                    dl
                    m
                    ${BOOST_LIB}
                    ${GLOG_LIB}
                    ${GFLAGS_LIB}
                    )

#    string(REPLACE "g++" "strip" CMAKE_CXX_STRIP ${CMAKE_CXX_COMPILER})
#    add_dependencies(fj_rtsp_server Strip)
#    add_custom_command(
#            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/build
#            TARGET fj_rtsp_server
#            COMMAND ${CMAKE_CXX_STRIP} fj_rtsp_server)
else (CMAKE_CROSSCOMPILING)
#add_definitions("-DX86_PLATFORM")
    target_link_libraries(fj_rtsp_server
                    src-modules
                    ros-core
                    ${LIVE555_LIB}
                    ${SRT_LIB}
                    pthread
                    dl
                    m
                    ${BOOST_LIB}
                    ${GLOG_LIB}
                    ${GFLAGS_LIB}
                    )

    aux_source_directory(test DIR_SRCS_TESTS) 
    add_executable(utest ${DIR_SRCS_TESTS})
    target_link_libraries(utest
                    src-modules
                    ros-core
                    ${GTEST_LIB}
                    ${GLOG_LIB}
                    ${GFLAGS_LIB}
                    pthread
                    dl
                    m
                    )
endif (CMAKE_CROSSCOMPILING)
