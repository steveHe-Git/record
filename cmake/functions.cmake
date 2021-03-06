# cmake参数解析必须要有这句话
include(CMakeParseArguments)

#宏定义的变量在整个所有的cmakelists中都可以使用
macro(_parse_arguments ARGS)
    #设置options,通过ARG_USES_CERES,ARG_USES_EIGEN....等进行访问变量
    set(OPTIONS
            USES_CERES
            USES_EIGEN
            USES_GLOG
            USES_LUA
            USES_BOOST
            USES_WEBP
            )
    #设置单值参数为空
    set(ONE_VALUE_ARG )
    #设置多值参数，通过ARG_SRCS...ARG_DEPENDS等来访问变量
    set(MULTI_VALUE_ARGS SRCS HDRS DEPENDS)
    #设置前缀为ARG
    cmake_parse_arguments(ARG
            "${OPTIONS}" "${ONE_VALUE_ARG}" "${MULTI_VALUE_ARGS}" ${ARGS})
endmacro(_parse_arguments)

# To avoid cmake warnning about CMP0026
cmake_policy(SET CMP0026 OLD)

#这个宏的作用是链接所有的头文件目录以及库目录, 并将头文件安装在指定的位置
macro(_common_compile_stuff VISIBILITY)
    #设置TARGET_COMPILE_FLAGS变量的编译选项为TARGET_COMPILE_FLAGS + GOOG_CXX_FLAGS，两者都没有定义为空
    set(TARGET_COMPILE_FLAGS "${TARGET_COMPILE_FLAGS} ${GOOG_CXX_FLAGS}")
    #PROPERTIES COMPILE_FLAGS 增加目标额外的编译选项TARGET_COMPILE_FLAGS
    set_target_properties(${NAME} PROPERTIES
            COMPILE_FLAGS ${TARGET_COMPILE_FLAGS})
    
    if(ARG_USES_EIGEN)
	#如果指定了SYSTEM，编译器就会被告知这些目录意味着在某些平台上的SYSTEM include目录(暗示这个设置可能会达到这样的效果，比如编译器跳过警告，或者这些固定安装的系统文件不在依赖关系计算中考虑-参见编译器文档)
        target_include_directories("${NAME}" SYSTEM ${VISIBILITY}
                "${EIGEN3_INCLUDE_DIR}")
	#指定目标包含的库目录
        target_link_libraries("${NAME}" "${EIGEN3_LIBRARIES}")
    endif()

    if(ARG_USES_CERES)
        target_include_directories("${NAME}" SYSTEM ${VISIBILITY}
                "${CERES_INCLUDE_DIRS}")
        target_link_libraries("${NAME}" "${CERES_LIBRARIES}")
    endif()

    if(ARG_USES_LUA)
        target_include_directories("${NAME}" SYSTEM ${VISIBILITY}
                "${LUA_INCLUDE_DIR}")
        target_link_libraries("${NAME}" "${LUA_LIBRARIES}")
    endif()

    if(ARG_USES_BOOST)
        target_include_directories("${NAME}" SYSTEM ${VISIBILITY}
                "{Boost_INCLUDE_DIRS}")
        target_link_libraries("${NAME}" "${Boost_LIBRARIES}")
    endif()

    if(ARG_USES_WEBP)
        target_link_libraries("${NAME}" webp)
    endif()

    # Add the binary directory first, so that port.h is included after it has
    # been generated.
    target_include_directories("${NAME}" ${VISIBILITY} "${CMAKE_BINARY_DIR}")
    target_include_directories("${NAME}" ${VISIBILITY} "${CMAKE_SOURCE_DIR}")

    #遍历链接ARG_DEPENDS中的依赖
    foreach(DEPENDENCY ${ARG_DEPENDS})
        target_link_libraries(${NAME} ${DEPENDENCY})
    endforeach()

    # Figure out where to install the header. The logic goes like this: either
    # the header is in the current binary directory (i.e. generated, like port.h)
    # or in the current source directory - a regular header. It could also
    # already be absolute (i.e. generated through a sr_proto_library rule).
    # In all cases we want to install the right header under the right subtree,
    # e.g. foo/bar/baz.h.in will be installed from the binary directory as
    # 'include/foo/bar/baz.h'.
    foreach(HDR ${ARG_HDRS})
        if (EXISTS ${CMAKE_CURRENT_BINARY_DIR}/${HDR})
            set(ABS_FIL "${CMAKE_CURRENT_BINARY_DIR}/${HDR}")
	    #REL_FIL是var, RELATIVE_PATH选项会确定从direcroty参数${CMAKE_BINARY_DIR} 到指定文件${ABS_FIL}的相对路径;
            file(RELATIVE_PATH REL_FIL ${CMAKE_BINARY_DIR} ${ABS_FIL})
        elseif(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${HDR})
            set(ABS_FIL "${CMAKE_CURRENT_SOURCE_DIR}/${HDR}")
	    #REL_FIL是var, RELATIVE_PATH选项会确定从direcroty参数${CMAKE_BINARY_DIR} 到指定文件${ABS_FIL}的相对路径;
            file(RELATIVE_PATH REL_FIL ${CMAKE_SOURCE_DIR} ${ABS_FIL})
        else()
            set(ABS_FIL "${HDR}")
            file(RELATIVE_PATH REL_FIL ${CMAKE_BINARY_DIR} ${ABS_FIL})
        endif()
	#获取${REL_FIL}不包含文件名的目录
        get_filename_component(INSTALL_DIR ${REL_FIL} DIRECTORY)
        install(
                FILES
                ${ABS_FIL}
                DESTINATION
                include/${INSTALL_DIR}
        )
    endforeach()
endmacro(_common_compile_stuff)

# Create a static library out of other static libraries by running a GNU ar
# script over the dependencies.
#
function(sr_combined_library NAME)
    set(MULTI_VALUE_ARGS SRCS)
    cmake_parse_arguments(ARG "" "" "${MULTI_VALUE_ARGS}" ${ARGN})

    # Cmake requires a source file for each library, so we create a dummy file
    # that is empty. Its creation depends on all libraries we want to include in
    # our combined static library, i.e. it will be touched whenever any of them
    # change, which means that our combined library is regenerated.
    #创建一个空文文件${NAME}_dummy.cc
    set(DUMMY_SOURCE ${CMAKE_CURRENT_BINARY_DIR}/${NAME}_dummy.cc)
    add_custom_command(
            OUTPUT  ${DUMMY_SOURCE}
            COMMAND cmake -E touch ${DUMMY_SOURCE}
            DEPENDS ${ARG_SRCS}
    )
    
    #
    add_custom_command(
            OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/${FIL_WE}.pb.cc"
            "${CMAKE_CURRENT_BINARY_DIR}/${FIL_WE}.pb.h"
            COMMAND  ${PROTOBUF_PROTOC_EXECUTABLE}
            ARGS --cpp_out  ${CMAKE_BINARY_DIR} -I
            ${CMAKE_BINARY_DIR} ${REWRITTEN_PROTO}
            DEPENDS ${REWRITTEN_PROTO}
            COMMENT "Running C++ protocol buffer compiler on ${FIL}"
            VERBATIM
    )

    # Just a dummy library, we will overwrite its output directly after again
    # with its POST_BUILD step.
    # 生成静态库
    sr_library(${NAME}
            SRCS ${DUMMY_SOURCE}
            DEPENDS ${ARG_SRCS}
            )

    #获取静态库的位置
    get_property(OUTPUT_FILE TARGET ${NAME} PROPERTY LOCATION)

    # We will delete the static lib generated by the last call to
    # 'sr_library' and recreate it using a GNU ar script that combines the
    # SRCS into the NAME.
    # TODO(hrapp): this is probably not very portable, but should work fine on
    # Linux.
    # 合并多个静态库
    set(AR_SCRIPT "")
    set(AR_SCRIPT "CREATE ${OUTPUT_FILE}\n")
    foreach(SRC ${ARG_SRCS})
        get_property(STATIC_LIBRARY_FILE TARGET ${SRC} PROPERTY LOCATION)
        set(AR_SCRIPT "${AR_SCRIPT}ADDLIB ${STATIC_LIBRARY_FILE}\n")
    endforeach()
    set(AR_SCRIPT "${AR_SCRIPT}SAVE\nEND\n")
    set(AR_SCRIPT_FILE "${CMAKE_CURRENT_BINARY_DIR}/${NAME}_ar.script")
    file(WRITE ${AR_SCRIPT_FILE} ${AR_SCRIPT})

    add_custom_command(TARGET ${NAME} POST_BUILD
            COMMAND rm ${OUTPUT_FILE}
            COMMAND ${CMAKE_AR}
            ARGS -M < ${AR_SCRIPT_FILE}
            COMMENT "Recombining static libraries into ${NAME}."
            )
endfunction()

#生成静态库
function(sr_library NAME)
    _parse_arguments("${ARGN}")

    add_library(${NAME}
            STATIC
            ${ARG_SRCS} ${ARG_HDRS}
            )

    # Update the global variable that contains all static libraries.
    SET(ALL_LIBRARIES "${ALL_LIBRARIES};${NAME}" CACHE INTERNAL "ALL_LIBRARIES")

    # This is needed for header only libraries. While they do not really mean
    # anything for cmake, they are useful to make dependencies explicit.
    set_target_properties(${NAME} PROPERTIES LINKER_LANGUAGE CXX)

    _common_compile_stuff("PUBLIC")
endfunction()

#protobuf生成.cc和.h文件
function(sr_proto_library NAME)
    _parse_arguments("${ARGN}")

    set(PROTO_SRCS)
    set(PROTO_HDRS)
    foreach(FIL ${ARG_SRCS})
        get_filename_component(ABS_FIL ${FIL} ABSOLUTE)
        get_filename_component(FIL_DIR ${ABS_FIL} DIRECTORY)
        get_filename_component(FIL_WE ${FIL} NAME_WE)
        get_filename_component(FIL_NAME ${FIL} NAME)

        list(APPEND PROTO_SRCS "${FIL_DIR}/${FIL_WE}.pb.cc")
        list(APPEND PROTO_HDRS "${FIL_DIR}/${FIL_WE}.pb.h")

        add_custom_command(
                OUTPUT "${FIL_DIR}/${FIL_WE}.pb.cc"
                "${FIL_DIR}/${FIL_WE}.pb.h"
                COMMAND  ${PROTOBUF_PROTOC_EXECUTABLE}
                ARGS --cpp_out ${CMAKE_SOURCE_DIR} -I
                ${CMAKE_SOURCE_DIR} ${ABS_FIL}
                DEPENDS ${ABS_FIL}
                COMMENT "Running C++ protocol buffer compiler on ${FIL}"
                VERBATIM
        )
    endforeach()
    
    #设置生成的源码文件属性GENERATED为TRUE,否则cmake时会因找不到源码而报错
    set_source_files_properties(${PROTO_SRCS} ${PROTO_HDRS}
            PROPERTIES GENERATED TRUE)

    sr_library("${NAME}"
            SRCS "${PROTO_SRCS}"
            HDRS "${PROTO_HDRS}"
            DEPENDS "${ARG_DEPENDS}"
            )

    target_include_directories("${NAME}" SYSTEM "PUBLIC"
            "${PROTOBUF_INCLUDE_DIR}")
    # TODO(hrapp): This should not explicityly list pthread and use
    # PROTOBUF_LIBRARIES, but that failed on first try.
    target_link_libraries("${NAME}" "${PROTOBUF_LIBRARY}" pthread)
endfunction()
