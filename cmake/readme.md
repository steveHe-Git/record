# cmake语法学习
1. cmake_minimum_required (VERSION 3.5) #需要支持编译的cmake的最低版本
> project

```cpp
project命令用于指定cmake工程的名称，实际上，它还可以指定cmake工程的版本号（VERSION关键字）、简短的描述（DESCRIPTION关键字）、主页URL（HOMEPAGE_URL关键字）和编译工程使用的语言（LANGUAGES关键字）
project(<PROJECT-NAME> [<language-name>...])
或
project(<PROJECT-NAME>
[VERSION <major>[.<minor>[.<patch>[.<tweak>]]]]
[DESCRIPTION <project-description-string>]
[HOMEPAGE_URL <url-string>]
[LANGUAGES <language-name>...])
    
cmake ./ #在当前文件下编译的话题
    PROJECT_BINARY_DIR和PROJECT_SOURCE_DIR是等价的。也就是当前源码的目录
    
cmake ../或者cmake ../src
   	PROJECT_BINARY_DIR=全路径/build
	PROJECT_SOURCE_DIR=全路径/src
```

> add_executable

```cpp
# 指定生成目标
add_executable(Demo main.cc)
将名为 main.cc 的源文件编译成一个名称为 Demo 的可执行文件。
```

> aux_source_directory

```cpp
但是如果源文件很多，把所有源文件的名字都加进去将是一件烦人的工作。更省事的方法是使用 aux_source_directory 命令，该命令会查找指定目录下的所有源文件，然后将结果存进指定变量名
aux_source_directory(<dir> <variable>)
    
# 查找当前目录下的所有源文件
# 并将名称保存到 DIR_SRCS 变量
aux_source_directory(. DIR_SRCS)
# 指定生成目标
add_executable(Demo ${DIR_SRCS})
```

> add_subdirectory    target_link_libraries (多级目录使用)

```cpp
使用命令 add_subdirectory 指明本项目包含一个子目录 math，这样 math 目录下的 CMakeLists.txt 文件和源代码也会被处理 。配合使用命令 target_link_libraries 指明可执行文件 main 需要连接一个名为 MathFunctions 的链接库 

子目录中的 CMakeLists.txt
# 查找当前目录下的所有源文件
# 并将名称保存到 DIR_LIB_SRCS 变量
aux_source_directory(. DIR_LIB_SRCS)
# 生成链接库
add_library (MathFunctions ${DIR_LIB_SRCS})
在该文件中使用命令 add_library 将 src 目录中的源文件编译为静态链接库。
    
父目录中的CMakeLists.txt
# 查找当前目录下的所有源文件
# 并将名称保存到 DIR_SRCS 变量
aux_source_directory(. DIR_SRCS)
# 添加 math 子目录
add_subdirectory(math)
# 指定生成目标
add_executable(Demo main.cc)
# 添加链接库
target_link_libraries(Demo MathFunctions)
```

> add_compile_options
```cpp
1. 设置编译选项可以通过add_compile_options命令，也可以通过set命令修改CMAKE_CXX_FLAGS或CMAKE_C_FLAGS。
使用这两种方式在有的情况下效果是一样的，但请注意它们还是有区别的：
2. add_compile_options命令添加的编译选项是针对所有编译器的(包括c和c++编译器)，而set命令设置CMAKE_C_FLAGS或CMAKE_CXX_FLAGS变量则是分别只针对c和c++编译器的
    
#判断编译器类型,如果是gcc编译器,则在编译选项中加入c++11支持
if(CMAKE_COMPILER_IS_GNUCXX)
    add_compile_options(-std=c++11)
    message(STATUS "optional:-std=c++11")   
endif(CMAKE_COMPILER_IS_GNUCXX)
    
    使用add_compile_options添加-std=c++11选项，是想在编译c++代码时加上c++11支持选项。但是因为add_compile_options是针对所有类型编译器的，所以在编译c代码时，就会产生如下warning
    所以如下修改代码，则警告消除。
if(CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "-std=c++11 ${CMAKE_CXX_FLAGS}")
    message(STATUS "optional:-std=c++11")   
endif(CMAKE_COMPILER_IS_GNUCXX)

add_definitions这个命令也是同样针对所有编译器，一样注意这个区别
```

> 自定义编译选项

```cpp
# 加入一个配置头文件，用于处理 CMake 对源码的设置
configure_file (
"${PROJECT_SOURCE_DIR}/config.h.in"
"${PROJECT_BINARY_DIR}/config.h"
)
    configure_file 命令用于加入一个配置头文件config.h ，这个文件由 CMake 从 config.h.in 生成，通过这样的机制，将可以通过预定义一些参数和变量来控制代码的生成
    这里引用了一个 config.h 文件，这个文件预定义了USE_MYMATH的值。但我们并不直接编写这个文件，为了方便从 CMakeLists.txt 中导入配置，我们编写一个 config.h.in 文件，内容如下
    #cmakedefine USE_MYMATH
    在输出文件config.h中就会被转化为：
	#define CODE_MODE_DEBUG 1
	
配合option可以实现cmake的选择编译
# 是否使用自己的 MathFunctions 库
option (USE_MYMATH "Use provided math implementation" ON)

if (USE_MYMATH)
include_directories ("${PROJECT_SOURCE_DIR}/math")
add_subdirectory (math)
set (EXTRA_LIBS ${EXTRA_LIBS} MathFunctions)
endif (USE_MYMATH)

# 查找当前目录下的所有源文件
# 并将名称保存到 DIR_SRCS 变量
aux_source_directory(. DIR_SRCS)
# 指定生成目标
add_executable(Demo ${DIR_SRCS})
target_link_libraries (Demo ${EXTRA_LIBS})
```

> 安装和测试

```cpp
CMake 也可以指定安装规则，以及添加测试。这两个功能分别可以通过在产生 Makefile 后使用 make install 和 make test 来执行。在以前的 GNU Makefile 里，你可能需要为此编写 install 和 test 两个伪目标和相应的规则，但在 CMake 里，这样的工作同样只需要简单的调用几条命令

1.定制安装规则
首先先在 math/CMakeLists.txt 文件里添加下面两行
# 指定 MathFunctions 库的安装路径
install (TARGETS MathFunctions DESTINATION bin)
install (FILES MathFunctions.h DESTINATION include)

之后同样修改根目录的 CMakeLists 文件，在末尾添加下面几行：
# 指定安装路径
install (TARGETS Demo DESTINATION bin)
install (FILES "${PROJECT_BINARY_DIR}/config.h"
DESTINATION include)
    
通过上面的定制，生成的 Demo 文件和 MathFunctions 函数库 libMathFunctions.o 文件将会被复制到 /usr/local/bin 中，而 MathFunctions.h 和生成的 config.h 文件则会被复制到 /usr/local/include 中
这里的 /usr/local/ 是默认安装到的根目录，可以通过修改 CMAKE_INSTALL_PREFIX 变量的值来指定这些文件应该拷贝到哪个根目录
    
# Set default install prefix
set(CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR})
    
    
2. 为工程添加测试
添加测试同样很简单。CMake 提供了一个称为 CTest 的测试工具。我们要做的只是在项目根目录的 CMakeLists 文件中调用一系列的 add_test 命令

# 启用测试
enable_testing()
    
# 测试 5 的平方
add_test (test_5_2 Demo 5 2)
set_tests_properties (test_5_2
PROPERTIES PASS_REGULAR_EXPRESSION "is 25")

# 测试 10 的 5 次方
add_test (test_10_5 Demo 10 5)
set_tests_properties (test_10_5
PROPERTIES PASS_REGULAR_EXPRESSION "is 100000")

# 测试 2 的 10 次方
add_test (test_2_10 Demo 2 10)
set_tests_properties (test_2_10
PROPERTIES PASS_REGULAR_EXPRESSION "is 1024")
    
如果要测试更多的输入数据，像上面那样一个个写测试用例未免太繁琐。这时可以通过编写宏来实现：
# 定义一个宏，用来简化测试工作
macro (do_test arg1 arg2 result)
add_test (test_${arg1}_${arg2} Demo ${arg1} ${arg2})
set_tests_properties (test_${arg1}_${arg2}
PROPERTIES PASS_REGULAR_EXPRESSION ${result})
endmacro (do_test)

# 使用该宏进行一系列的数据测试
do_test (5 2 "is 25")
do_test (10 5 "is 100000")
do_test (2 10 "is 1024")
```

> 使用cmake工具辅助gtest单元测试的步骤记录

```cpp
1. 建立测试子项目
    将gtest解压后的文件夹放到开发代码文件夹的同级目录下；
在开发代码文件夹的同级目录下新建文件夹，命名test，存放单元测试文件；
找到该目录下的CMakeLists.txt文件，在文件中添加如下语句：
add_subdirectory("test")
add_subdirectory("gtest")
如此，在项目编译时便会一起编译gtest和test子项目。

2. 编写测试项目CMakeLists.txt文件
在test文件夹下新建txt文件，命名CMakeLists.txt；
下图展示一个基本功能的单元测试CMakeLists文件:
cmake_minimum_required(VERSION 3.1.0)
project(ProjectTest)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
message(STATUS "C++11 support has been enabled by default.")
#将test文件夹以及子文件夹下所有的编码文件（*.h，*.cpp）组成一个列表，并存储在变量TEST_SRC中，后续便可以使用${TEST_SRC}代表这些文件；
FILE(GLOB_RECURSE TEST_SRC "${CMAKE_SOURCE_DIR}/test/*.cpp" "${CMAKE_SOURCE_DIR}/test/*.h")
add_executable(${PROJECT_NAME} ${TEST_SRC})
target_link_libraries(${PROJECT_NAME} gtest ProjectSource)
    
接下来在test文件夹下编写单元测试文件即可
```

> 支持gdb

```cpp
make中对环境变量读写都是通过ENV前缀来访问环境变量
读取环境变量则要使用 $ENV{JAVA_HOME}这样的格式 
set(CMAKE_BUILD_TYPE "Debug")
set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g -ggdb")
set(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")
```

# cmake 常用的变量，常用的环境变量以及常用的语法总结

> cmake变量的引用方式

```cpp
使用${}进行变量的引用。在 IF 等语句中,是直接使用变量名而不通过${}取值
```

> cmake 自定义变量

```cpp
主要有隐式定义和显式定义两种。
隐式定义:会隐式的定义<projectname>_BINARY_DIR 和<projectname>_SOURCE_DIR 两个变量
  ${PROJECT_BINARY_DIR}    <=> ${CMAKE_BINARY_DIR}
	${PROJECT_SOURCE_DIR}  <=> ${CMAKE_SOURCE_DIR}
1. 如果是not in source编译,建立一个build文件夹， cmake ../ 或者cmake ../src ../或者../src指定源文件的和CMakeLists.txt的位置
    ${PROJECT_BINARY_DIR} ==>  ./build		(cmake ../)
    ${PROJECT_SOURCE_DIR} ==>  ./      	  	(cmake ../)
    ${PROJECT_BINARY_DIR} ==>  ./build		(cmake ../src)
    ${PROJECT_SOURCE_DIR} ==>  ./src      	(cmake ../src)
2. 如果是in source编译,不建立一个build文件夹， cmake ../ 或者cmake ../src ../或者../src指定源文件的和CMakeLists.txt的位置
    ${PROJECT_BINARY_DIR} ==>  ./      		（cmake ./）
    ${PROJECT_SOURCE_DIR} ==>  ./	   		（cmake ./）
    ${PROJECT_BINARY_DIR} ==>  ./      		（cmake ./src）
    ${PROJECT_SOURCE_DIR} ==>  ./src	   	（cmake ./src）

set用来显式的定义变量
 1. 定义了一个变量libs，并且变量的值为${CMAKE_SOURCE_DIR}/src/main/jnilibs
 set(libs "${CMAKE_SOURCE_DIR}/src/main/jnilibs")
 2.这个命令用于给CMAKE_LIBRARY_OUTPUT_DIRECTORY宏赋值，该宏指定了cmake编译输出的东西应该放在什么地方
 set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/src/main/jnilibs/${ANDROID_ABI}) 
```

> cmake 常用的变量

```cpp
PROJECT_NAME: 				project(name)命令中写的项目名name;
CMAKE_VERSION: 				当前使用CMake的版本
CMAKE_SOURCE_DIR: 			工程顶层目录，即入口CMakeLists文件所在路径 (./)
PROJECT_SOURCE_DIR: 		同CMAKE_SOURCE_DIR (./)
CMAKE_BINARY_DIR: 			工程编译发生的目录，即执行cmake命令进行项目配置的目录，一般为build (./build)
PROJECT_BINARY_DIR: 		同CMAKE_BINARY_DIR(./build)
CMAKE_CURRENT_SOURCE_DIR:	当前处理的CMakeLists.txt所在的路径(./)
CMAKE_CURRRENT_BINARY_DIR:	当前处理的CMakeLists.txt中生成目标文件所在编译目录(./build)
    使用ADD_SURDIRECTORY(src bin)可以更改此变量的值
	SET(EXECUTABLE_OUTPUT_PATH <新路径>)并不会对此变量有影响,只是改变了最终目标文件的存储路径
CMAKE_CURRENT_LIST_FILE:	输出调用这个变量的CMakeLists.txt文件的完整路径（./CMakeLists.txt）
CMAKE_CURRENT_LIST_LINE:    输出当前所在行
CMAKE_CURRENT_LIST_DIR:		当前处理的CMakeLists.txt文件所在目录的路径(./)
CMAKE_INSTALL_PREFIX:		指定make install命令执行时包安装路径
CMAKE_MODULE_PATH:			find_package命令搜索包路径之一，默认为空
    
CMAKE_BUILD_TYPE: 			编译选项，Release或者Debug，如set(CMAKE_BUILD_TYPE "Release")
CMAKE_CXX_FLAGS:			编译标志，设置C++11编译，set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
CMAKE_CXX_STANDARD:			也可以设置C++11编译，set(CMAKE_CXX_STANDARD 11)
    
CMAKE_MAJOR_VERSION cmake主版本号,如2.8.6中的2
CMAKE_MINOR_VERSION cmake次版本号,如2.8.6中的8
CMAKE_PATCH_VERSION cmake补丁等级,如2.8.6中的6
CMAKE_SYSTEM 系统名称,例如Linux-2.6.22
CAMKE_SYSTEM_NAME 不包含版本的系统名,如Linux
CMAKE_SYSTEM_VERSION 系统版本,如2.6.22
CMAKE_SYSTEM_PROCESSOR 处理器名称,如i686
UNIX 在所有的类UNIX平台为TRUE,包括OS X和cygwin
WIN32 在所有的win32平台为TRUE,包括cygwin
    
EXECUTABLE_OUTPUT_PATH:重新定义目标二进制可执行文件的存放位置
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
LIBRARY_OUTPUT_PATH:重新定义目标链接库文件的存放位置
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

CMAKE_MODULE_PATH
这个变量用来定义自己的 cmake 模块所在的路径。如果你的工程比较复杂,有可能会自己编写一些 cmake 模块,这些 cmake 模块是随你的工程发布的,为了让 cmake 在处理CMakeLists.txt 时找到这些模块,你需要通过 SET 指令,将自己的 cmake 模块路径设置一下。
比如
SET(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)
这时候你就可以通过 INCLUDE 指令来调用自己的模块了
```

> cmake 常用的命令

```cpp
cmake变量使用${}方式取值,但是在IF控制语句中是直接使用变量名
环境变量使用$ENV{}方式取值,使用SET($ENV{VAR} VALUE)赋值

指令(参数1 参数2…) 参数使用括弧括起,参数之间使用空格或分号分开。
以ADD_EXECUTABLE指令为例：
ADD_EXECUTABLE(hello main.c func.c)或者
ADD_EXECUTABLE(hello main.c;func.c)
    
--PROJECT
PROJECT(projectname [CXX] [C] [Java])
指定工程名称,并可指定工程支持的语言。支持语言列表可忽略,默认支持所有语言
    
--MESSAGE
MESSAGE([SEND_ERROR | STATUS | FATAL_ERROR] “message to display” …)
向终端输出用户定义的信息或变量的值
SEND_ERROR, 产生错误,生成过程被跳过
STATUS, 输出前缀为—的信息
FATAL_ERROR, 立即终止所有cmake过程
    
--ADD_EXECUTABLE
ADD_EXECUTABLE(bin_file_name ${SRC_LIST})
生成可执行文件
    
--ADD_LIBRARY
ADD_LIBRARY(libname [SHARED | STATIC | MODULE] [EXCLUDE_FROM_ALL] SRC_LIST)
生成动态库或静态库
SHARED 动态库
STATIC 静态库
MODULE 在使用dyld的系统有效,若不支持dyld,等同于SHARED
EXCLUDE_FROM_ALL 表示该库不会被默认构建

--SET_TARGET_PROPERTIES
设置输出的名称,设置动态库的版本和API版本
# cmake在构建一个新的target时，会尝试清理掉其他使用这个名字的库，
# 因此，在构建libhello.a时，就会清理掉libhello.so.
# 为了回避这个问题，比如再次使用SET_TARGET_PROPERTIES定义 CLEAN_DIRECT_OUTPUT属性。
SET_TARGET_PROPERTIES (hello_static PROPERTIES CLEAN_DIRECT_OUTPUT 1)
SET_TARGET_PROPERTIES (hello PROPERTIES CLEAN_DIRECT_OUTPUT 1)
# 按照规则，动态库是应该包含一个版本号的，
# VERSION指代动态库版本，SOVERSION指代API版本。
SET_TARGET_PROPERTIES (hello PROPERTIES VERSION 1.2 SOVERSION 1)
# 我们需要将libhello.a, libhello.so.x以及hello.h安装到系统目录，才能真正让其他人开发使用，
# 在本例中我们将hello的共享库安装到<prefix>/lib目录；
# 将hello.h安装<prefix>/include/hello目录。
INSTALL (TARGETS hello hello_static LIBRARY DESTINATION lib
ARCHIVE DESTINATION lib) #静态库需要加ARCHIVE
INSTALL (FILES hello.h DESTINATION include/hello)
    
--CMAKE_MINIMUM_REQUIRED
CMAKE_MINIMUM_REQUIRED(VERSION version_number [FATAL_ERROR])
声明CMake的版本要求
    
--ADD_SUBDIRECTORY
ADD_SUBDIRECTORY(src_dir [binary_dir] [EXCLUDE_FROM_ALL])
向当前工程添加存放源文件的子目录,并可以指定中间二进制和目标二进制的存放位置
EXCLUDE_FROM_ALL含义：将这个目录从编译过程中排除

--INCLUDE_DIRECTORIES
INCLUDE_DIRECTORIES([AFTER | BEFORE] [SYSTEM] dir1 dir2 … )
向工程添加多个特定的头文件搜索路径,路径之间用空格分隔,如果路径包含空格,可以使用双引号将它括起来,默认的行为为追加到当前头文件搜索路径的后面。有如下两种方式可以控制搜索路径添加的位置：
    CMAKE_INCLUDE_DIRECTORIES_BEFORE,通过SET这个cmake变量为on,可以将添加的头文件搜索路径放在已有路径的前面
    通过AFTER或BEFORE参数,也可以控制是追加还是置前

--LINK_DIRECTORIES
LINK_DIRECTORIES(dir1 dir2 …)
添加非标准的共享库搜索路径
    
--TARGET_LINK_LIBRARIES
TARGET_LINK_LIBRARIES(target lib1 lib2 …)
为target添加需要链接的共享库
    
--ADD_DEFINITIONS
向C/C++编译器添加-D定义
ADD_DEFINITIONS(-DENABLE_DEBUG -DABC),参数之间用空格分隔
    
--ADD_DEPENDENCIES
ADD_DEPENDENCIES(target-name depend-target1 depend-target2 …)
定义target依赖的其他target,确保target在构建之前,其依赖的target已经构建完毕
    
--AUX_SOURCE_DIRECTORY
AUX_SOURCE_DIRECTORY(dir VAR)
发现一个目录下所有的源代码文件并将列表存储在一个变量中
把当前目录下的所有源码文件名赋给变量DIR_HELLO_SRCS
    
--EXEC_PROGRAM
EXEC_PROGRAM(Executable [dir where to run] [ARGS <args>][OUTPUT_VARIABLE <var>] [RETURN_VALUE <value>])
用于在指定目录运行某个程序（默认为当前CMakeLists.txt所在目录）,通过ARGS添加参数,通过OUTPUT_VARIABLE和RETURN_VALUE获取输出和返回值,如下示例
# 在src中运行ls命令,在src/CMakeLists.txt添加
EXEC_PROGRAM(ls ARGS "*.c" OUTPUT_VARIABLE LS_OUTPUT RETURN_VALUE LS_RVALUE)
IF (not LS_RVALUE)
    MESSAGE(STATUS "ls result: " ${LS_OUTPUT}) # 缩进仅为美观,语法无要求
ENDIF(not LS_RVALUE)
    
 --INCLUDE
INCLUDE(file [OPTIONAL]) 用来载入CMakeLists.txt文件
INCLUDE(module [OPTIONAL])用来载入预定义的cmake模块
OPTIONAL参数的左右是文件不存在也不会产生错误
可以载入一个文件,也可以载入预定义模块（模块会在CMAKE_MODULE_PATH指定的路径进行搜索）
载入的内容将在处理到INCLUDE语句时直接执行

--FIND_
FIND_FILE(<VAR> name path1 path2 …)
VAR变量代表找到的文件全路径,包含文件名

FIND_LIBRARY(<VAR> name path1 path2 …)
	VAR变量代表找到的库全路径,包含库文件名
     FIND_LIBRARY(libX X11 /usr/lib)
     IF (NOT libx)
        MESSAGE(FATAL_ERROR "libX not found")
     ENDIF(NOT libX)

FIND_PATH(<VAR> name path1 path2 …)
	VAR变量代表包含这个文件的路径
    
FIND_PROGRAM(<VAR> name path1 path2 …)
	VAR变量代表包含这个程序的全路径
    
FIND_PACKAGE(<name> [major.minor] [QUIET] [NO_MODULE] [[REQUIRED | COMPONENTS] [componets …]])
  	用来调用预定义在CMAKE_MODULE_PATH下的Find<name>.cmake模块,你也可以自己定义Find<name>
    模块,通过SET(CMAKE_MODULE_PATH dir)将其放入工程的某个目录供工程使用
    
--list
Reading
  list(LENGTH <list> <out-var>)
  list(GET <list> <element index> [<index> ...] <out-var>)
  list(JOIN <list> <glue> <out-var>)
  list(SUBLIST <list> <begin> <length> <out-var>)

Search
  list(FIND <list> <value> <out-var>)

Modification
  list(APPEND <list> [<element>...])
  list(FILTER <list> {INCLUDE | EXCLUDE} REGEX <regex>)
  list(INSERT <list> <index> [<element>...])
  list(POP_BACK <list> [<out-var>...])
  list(POP_FRONT <list> [<out-var>...])
  list(PREPEND <list> [<element>...])
  list(REMOVE_ITEM <list> <value>...)
  list(REMOVE_AT <list> <index>...)
  list(REMOVE_DUPLICATES <list>)
  list(TRANSFORM <list> <ACTION> [...])

Ordering
  list(REVERSE <list>)
  list(SORT <list> [...])
```
> cmake中的FILE指令
```cpp
文件操作指令,基本语法为:
  file(WRITE filename "message to write"... )
  WRITE选项将会写一条消息到名为filename的文件中。如果文件已经存在，该命令会覆盖已有的文件；如果文件不存在，它将创建该文件。
      
  file(APPEND filename "message to write"... )
  APPEND选项和WRITE选项一样，将会写一条消息到名为filename的文件中，只是该消息会附加到文件末尾。
      
  file(READ filename variable [LIMIT numBytes] [OFFSET offset] [HEX])
  READ选项将会读一个文件中的内容并将其存储在变量里。读文件的位置从offset开始，最多读numBytes个字节。如果指定了HEX参数，二进制代码将会转换为十六进制表达方式，并存储在变量里。
      
  file(STRINGS filename variable [LIMIT_COUNT num]
       [LIMIT_INPUT numBytes] [LIMIT_OUTPUT numBytes]
       [LENGTH_MINIMUM numBytes] [LENGTH_MAXIMUM numBytes]
       [NEWLINE_CONSUME] [REGEX regex]
       [NO_HEX_CONVERSION])
  STRINGS将会从一个文件中将一个ASCII字符串的list解析出来，然后存储在variable变量中。文件中的二进制数据会被忽略。回车换行符会被忽略。它也可以用在Intel的Hex和Motorola的S-记录文件；读取它们时，它们会被自动转换为二进制格式。可以使用NO_HEX_CONVERSION选项禁止这项功能。LIMIT_COUNT选项设定了返回的字符串的最大数量。LIMIT_INPUT设置了从输入文件中读取的最大字节数。LIMIT_OUTPUT设置了在输出变量中存储的最大字节数。LENGTH_MINIMUM设置了要返回的字符串的最小长度；小于该长度的字符串会被忽略。LENGTH_MAXIMUM设置了返回字符串的最大长度；更长的字符串会被分割成不长于最大长度的字符串。NEWLINE_CONSUME选项允许新行被包含到字符串中，而不是终止它们。REGEX选项指定了一个待返回的字符串必须满足的正则表达式。典型的使用方式是：
  file(STRINGS myfile.txt myfile)
该命令在变量myfile中存储了一个list，该list中每个项是输入文件中的一行文本。

  file(GLOB variable [RELATIVE path] [globbing expressions]...)
  GLOB选项将会为所有匹配查询表达式的文件生成一个文件list，并将该list存储进变量variable里。文件名查询表达式与正则表达式类似，只不过更加简单。如果为一个表达式指定了RELATIVE标志，返回的结果将会是相对于给定路径的相对路径。文件名查询表达式的例子有：
   *.cxx      - 匹配所有扩展名为cxx的文件。
   *.vt?      - 匹配所有扩展名是vta,...,vtz的文件。
   f[3-5].txt - 匹配文件f3.txt, f4.txt, f5.txt

  file(GLOB_RECURSE variable [RELATIVE path] 
       [FOLLOW_SYMLINKS] [globbing expressions]...)
  GLOB_RECURSE选项将会生成一个类似于通常的GLOB选项的list，只是它会寻访所有那些匹配目录的子路径并同时匹配查询表达式的文件。作为符号链接的子路径只有在给定FOLLOW_SYMLINKS选项或者cmake策略CMP0009被设置为NEW时，才会被寻访到。参见cmake --help-policy CMP0009 查询跟多有用的信息。
      
  file(RENAME <oldname> <newname>)
  RENAME选项对同一个文件系统下的一个文件或目录重命名。（类似于mv命令——译注
      
  file(REMOVE [file1 ...])
  REMOVE选项将会删除指定的文件，包括在子路径下的文件。（类似于rm命令——译注
      
  file(REMOVE_RECURSE [file1 ...])
  REMOVE_RECURSE选项会删除给定的文件以及目录，包括非空目录。（类似于rm -r 命令——译注）
      
  file(MAKE_DIRECTORY [directory1 directory2 ...])
  MAKE_DIRECTORY选项将会创建指定的目录，如果它们的父目录不存在时，同样也会创建。（类似于mkdir命令——译注）
      
  file(RELATIVE_PATH variable directory file)
  RELATIVE_PATH选项会确定从direcroty参数到指定文件的相对路径
   
  file(TO_CMAKE_PATH path result)
   TO_CMAKE_PATH选项会把path转换为一个以unix的 / 开头的cmake风格的路径。输入可以是一个单一的路径，也可以是一个系统路径，比如"$ENV{PATH}"。注意，在调用TO_CMAKE_PATH的ENV周围的双引号只能有一个参数(Note the double quotes around the ENV call TO_CMAKE_PATH only takes one argument. 原文如此。quotes和后面的takes让人后纠结，这句话翻译可能有误。欢迎指正——译注)。

  file(TO_NATIVE_PATH path result)
  TO_NATIVE_PATH选项与TO_CMAKE_PATH选项很相似，但是它会把cmake风格的路径转换为本地路径风格：windows下用\，而unix下用/
      
  file(DOWNLOAD url file [TIMEOUT timeout] [STATUS status] [LOG log]
       [EXPECTED_MD5 sum] [SHOW_PROGRESS])
	DOWNLOAD 将给定的URL下载到指定的文件中。如果指定了LOG var选项，下载日志将会被输出到var中。如果指定了STATUS var选项，下载操作的状态会被输出到var中。该状态返回值是一个长度为2的list。list的第一个元素是操作的数字返回值，第二个返回值是错误的字符串值。错误信息如果是数字0，操作中没有发生错误。如果指定了TIMEOUT time选项，在time秒之后，操作会超时退出；time应该是整数。如果指定了EXPECTED_MD5 sum选项，下载操作会认证下载的文件的实际MD5和是否与期望值匹配。如果不匹配，操作将返回一个错误。如果指定了SHOW_PROGRESS选项，进度信息会以状态信息的形式被打印出来，直到操作完成。
　　file命令还提供了COPY和INSTALL两种格式：
  file(<COPY|INSTALL> files... DESTINATION <dir>
       [FILE_PERMISSIONS permissions...]
       [DIRECTORY_PERMISSIONS permissions...]
       [NO_SOURCE_PERMISSIONS] [USE_SOURCE_PERMISSIONS]
       [FILES_MATCHING]
       [[PATTERN <pattern> | REGEX <regex>]
        [EXCLUDE] [PERMISSIONS permissions...]] [...])
　　COPY版本把文件、目录以及符号连接拷贝到一个目标文件夹。相对输入路径的评估是基于当前的源代码目录进行的，相对目标路径的评估是基于当前的构建目录进行的。复制过程将保留输入文件的时间戳；并且如果目标路径处存在同名同时间戳的文件，复制命令会把它优化掉。赋值过程将保留输入文件的访问权限，除非显式指定权限或指定NO_SOURCE_PERMISSIONS选项（默认是USE_SOURCE_PERMISSIONS）。参见install(DIRECTORY)命令中关于权限（permissions），PATTERN，REGEX和EXCLUDE选项的文档。
　　INSTALL版本与COPY版本只有十分微小的差别：它会打印状态信息，并且默认使用NO_SOURCE_PERMISSIONS选项。install命令生成的安装脚本使用这个版本（它会使用一些没有在文档中涉及的内部使用的选项。）  
```

> cmake 常用的语法
```cpp
--IF
IF (expression)
    COMMAND1(ARGS ...)
    COMMAND2(ARGS ...)
    ...
ELSE (expression)
    COMMAND1(ARGS ...)
    COMMAND2(ARGS ...)
    ...
ENDIF (expression) # 一定要有ENDIF与IF对应
    
IF (expression), expression不为：空,0,N,NO,OFF,FALSE,NOTFOUND或<var>_NOTFOUND,为真
IF (not exp), 与上面相反
IF (var1 AND var2)
IF (var1 OR var2)
IF (COMMAND cmd) 如果cmd确实是命令并可调用,为真
IF (EXISTS dir) IF (EXISTS file) 如果目录或文件存在,为真
IF (file1 IS_NEWER_THAN file2),当file1比file2新,或file1/file2中有一个不存在时为真,文件名需使用全路径
IF (IS_DIRECTORY dir) 当dir是目录时,为真
IF (DEFINED var) 如果变量被定义,为真
IF (var MATCHES regex) 此处var可以用var名,也可以用${var}
IF (string MATCHES regex)
    
当给定的变量或者字符串能够匹配正则表达式regex时为真。比如：
IF ("hello" MATCHES "ell")
    MESSAGE("true")
ENDIF ("hello" MATCHES "ell")
    
数字比较表达式
IF (variable LESS number)
IF (string LESS number)
IF (variable GREATER number)
IF (string GREATER number)
IF (variable EQUAL number)
IF (string EQUAL number)

按照字母表顺序进行比较
IF (variable STRLESS string)
IF (string STRLESS string)
IF (variable STRGREATER string)
IF (string STRGREATER string)
IF (variable STREQUAL string)
IF (string STREQUAL string)
    
一个小例子,用来判断平台差异：
IF (WIN32)
    MESSAGE(STATUS “This is windows.”)
ELSE (WIN32)
    MESSAGE(STATUS “This is not windows”)
ENDIF (WIN32)
上述代码用来控制在不同的平台进行不同的控制,但是,阅读起来却并不是那么舒服,ELSE(WIN32)之类的语句很容易引起歧义。
可以SET(CMAKE_ALLOW_LOOSE_LOOP_CONSTRUCTS ON)
这时候就可以写成:
IF (WIN32)
ELSE ()
ENDIF ()
配合ELSEIF使用,可能的写法是这样:
IF (WIN32)
    #do something related to WIN32
ELSEIF (UNIX)
    #do something related to UNIX
ELSEIF(APPLE)
    #do something related to APPLE
ENDIF (WIN32)
    
    
    
--while
    WHILE(condition)
    	COMMAND1(ARGS ...)
    	COMMAND2(ARGS ...)
    	...
    ENDWHILE(condition)
    其真假判断条件可以参考IF指令
    
    
--FOREACH
    1. 列表
    FOREACH(loop_var arg1 arg2 ...)
     COMMAND1(ARGS ...)
     COMMAND2(ARGS ...)
     ...
    ENDFOREACH(loop_var)
    
    AUX_SOURCE_DIRECTORY(. SRC_LIST)
    FOREACH(F ${SRC_LIST})
     MESSAGE(${F})
    ENDFOREACH(F)
    
    2. 范围
    FOREACH(loop_var RANGE total)
        COMMAND1(ARGS ...)
        COMMAND2(ARGS ...)
        ...
    ENDFOREACH(loop_var)
    
    从0到total以１为步进
    FOREACH(VAR RANGE 10)
   		MESSAGE(${VAR})
	ENDFOREACH(VAR)
	输出：
	012345678910
    
    3. 范围和步进
    FOREACH(loop_var RANGE start stop [step])
    	COMMAND1(ARGS ...)
    	COMMAND2(ARGS ...)
    	...
    ENDFOREACH(loop_var)
    
    FOREACH(A RANGE 5 15 3)
        MESSAGE(${A})
    ENDFOREACH(A)
    输出：
    581114

    4. 列表 
    foreach(<loop_var> IN [LISTS [<lists>]] [ITEMS [<items>]])
    set(A 0;1)
    set(B 2 3)
    set(C "4 5")
    set(D 6;7 8)
    set(E "")
    foreach(X IN LISTS A B C D E)
        message(STATUS "X=${X}")
    endforeach()

    -- X=0
    -- X=1
    -- X=2
    -- X=3
    -- X=4 5
    -- X=6
    -- X=7
    -- X=8

    5. foreach(<loop_var>... IN ZIP_LISTS <lists>)
    list(APPEND English one two three four)
    list(APPEND Bahasa satu dua tiga)

    foreach(num IN ZIP_LISTS English Bahasa)
        message(STATUS "num_0=${num_0}, num_1=${num_1}")
    endforeach()

    foreach(en ba IN ZIP_LISTS English Bahasa)
        message(STATUS "en=${en}, ba=${ba}")
    endforeach()

    //输出
    -- num_0=one, num_1=satu
    -- num_0=two, num_1=dua
    -- num_0=three, num_1=tiga
    -- num_0=four, num_1=
    -- en=one, ba=satu
    -- en=two, ba=dua
    -- en=three, ba=tiga
    -- en=four, ba=

```

> cmake 一些实例
```cpp
1. cmake中如何增加编译选项
使用变量CMAKE_C_FLAGS添加C编译选项
使用变量CMAKE_CXX_FLAGS添加C++编译选项
使用ADD_DEFINITION添加
   
2. cmake中如何指定生成文件的输出路径
    如上ADD_SUBDIRECTORY的时候指定目标二进制文件输出路径（推荐使用下面这种）
    使用SET命令重新定义EXECUTABLE_OUTPUT_PATH和LIBRARY_OUTPUT_PATH变量来指定最终的二进制文件的位置
        SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
        SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)
    上面的两条命令通常紧跟ADD_EXECUTABLE和ADD_LIBRARY,与其写在同一个CMakeLists.txt即可
    
3. cmake中如何生成动态库和静态库
参考ADD_LIBRARY和SET_TARGET_PROPERTIES用法
    CMakeLists.txt内容如下
    # 若变量BUILD_SHARED_LIBS没有设置，并且在ADD_LIBRARY时没有指定库类型
    # 默认生成的都是静态库
    #SET(BUILD_SHARED_LIBS on)
    AUX_SOURCE_DIRECTORY(. SRC_LIST)
    ADD_LIBRARY(hello_shared SHARED ${SRC_LIST}) # lib的名字不能重复
    ADD_LIBRARY(hello_static STATIC ${SRC_LIST})
    SET_TARGET_PROPERTIES(hello_static PROPERTIES OUTPUT_NAME "hello")
    SET_TARGET_PROPERTIES(hello_shared PROPERTIES OUTPUT_NAME "hello")

    # 设置动态库的版本号
    SET_TARGET_PROPERTIES(hello_shared PROPERTIES VERSION 1.2 SOVERSION 1)

    # 如果发现后来生成的lib清除了同名的早先生成的lib，（只有一个静态库或动态库生成）
    # 试下下面两个命令
    # 较新版本的cmake不再需要下面两个命令就可以同时生成hello.so hello.a
    #SET_TARGET_PROPERTIES(hello_shared PROPERTIES CLEAN_DIRECT_OUTPUT 1)
    #SET_TARGET_PROPERTIES(hello_static PROPERTIES CLEAN_DIRECT_OUTPUT 1)
    SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)
   
4. cmake中如何使用动态库和静态库（查找库的路径）
    AUX_SOURCE_DIRECTORY(. SRC_LIST)
    # 这行一定要在ADD_EXECUTABLE前面，
    # 链接动态库目录
    LINK_DIRECTORIES(/home/carl/cmake/t3/build/lib)
    ADD_EXECUTABLE(t4 ${SRC_LIST})
    SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

    SET(CMAKE_PROJECT_DIR /home/carl/cmake) # cmake文件夹的位置
    #链接库的头文件
    INCLUDE_DIRECTORIES(${CMAKE_PROJECT_DIR}/t3/lib_hello)
    #链接库的名字
    TARGET_LINK_LIBRARIES(t4 hello) # 链接动态库
    #TARGET_LINK_LIBRARIES(t4 libhello.a) # 链接静态库
    
 5.t5示例如何使用cmake预定义的cmake模块(以FindCURL.cmake为例演示)
    AUX_SOURCE_DIRECTORY(. SRC_LIST)
    ADD_EXECUTABLE(curltest ${SRC_LIST})
    SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
    #INCLUDE_DIRECTORIES(/usr/include)
    #TARGET_LINK_LIBRARIES(curltest curl)
    FIND_PACKAGE(CURL)
    IF (CURL_FOUND)
        MESSAGE(STATUS "CURL_INCLUDE_DIR: " ${CURL_INCLUDE_DIR} " CURL_LIBRARY: " ${CURL_LIBRARY})
        INCLUDE_DIRECTORIES(${CURL_INCLUDE_DIR})
        TARGET_LINK_LIBRARIES(curltest ${CURL_LIBRARY})
    ELSE(CURL_FOUND)
        MESSAGE(FATAL_ERROR "CURL library not found")
    ENDIF(CURL_FOUND)

    #对于系统预定义的Find<name>.cmake模块，使用方法一般如上所示：
    #每一个模块都会定义以下几个变量
    #• <name>_FOUND   通过此变量判断模块是否被找到
    #• <name>_INCLUDE_DIR or <name>_INCLUDES
    #• <name>_LIBRARY or <name>_LIBRARIES
    
    
 6.t6示例如何使用自定义的cmake模块(编写了自定义的FindHELLO.cmake)
    cmakelists.txt(根目录./)
  	CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
    PROJECT(HELLO)
    MESSAGE(STATUS "此Project演示了如何使用自定义的cmake模块(FindHELLO.cmake)")
    SET(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)
    ADD_SUBDIRECTORY(src)
    
    cmakelists.txt(子目录./src)
    AUX_SOURCE_DIRECTORY(. SRC_LIST)
    ADD_EXECUTABLE(t6 ${SRC_LIST})
    SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

    # 没有指定QUIET参数, HELLO_FIND_QUIETLY为假
    # 指定REQUIRED参数, HELLO_FIND_REQUIRED为真, 意味着此lib为该Project必须
    FIND_PACKAGE(HELLO REQUIRED) 
    # 下面的HELLO_FOUND  HELLO_INCLUDE_DIR  HELLO_LIBRARY
    # 3个变量在FindHELLO.cmake中定义
    IF (HELLO_FOUND)
        INCLUDE_DIRECTORIES(${HELLO_INCLUDE_DIR})
        TARGET_LINK_LIBRARIES(t6 ${HELLO_LIBRARY})
    ENDIF (HELLO_FOUND)
    
    FindHELLO.cmake (./cmake/)
    # 注意此cmake文件中的3个变量名
    # HELLO_INCLUDE_DIR -- <name>_INCLUDE_DIR
    # HELLO_LIBRARY     -- <name>_LIBRARY
    # HELLO_FOUND       -- <name>_FOUND
    # 惯例需要定义这3个变量，命名规则如上，以便与cmake预定义的cmake模块中的变量名统一
    # FIND_PATH和FIND_LIBARY的path部分，一定要是.h文件和lib文件所在的当前目录，否则找不到
    FIND_PATH(HELLO_INCLUDE_DIR hello.h /home/steve/Downloads/cmake_example/demo-master/cmake/t3/lib_hello)
    MESSAGE(STATUS "HELLO_INCLUDE_DIR: " ${HELLO_INCLUDE_DIR})
    FIND_LIBRARY(HELLO_LIBRARY hello /home/steve/Downloads/cmake_example/demo-master/cmake/t3/build/lib)
    MESSAGE(STATUS "HELLO_LIBRARY: " ${HELLO_LIBRARY})
    IF (HELLO_INCLUDE_DIR AND HELLO_LIBRARY)
        SET(HELLO_FOUND TRUE)
    ENDIF(HELLO_INCLUDE_DIR AND HELLO_LIBRARY)
    IF (HELLO_FOUND)
        # 若FIND_PACKAGE命令没有指定QUIET参数，那下面的IF语句就为真
        IF (NOT HELLO_FIND_QUIETLY)
            MESSAGE(STATUS "Found Hello: ${HELLO_LIBRARY}")
        ENDIF (NOT HELLO_FIND_QUIETLY)
    ELSE (HELLO FOUND)
        # 如果FIND_PACKAGE命令指定了REQUIRED参数，说明这个库是此Project必须的
        # 对应下面的HELLO_FIND_REQUIRED变量
        IF (HELLO_FIND_REQUIRED)
            MESSAGE(FATAL_ERROR "Could not find hello library")
        ENDIF (HELLO_FIND_REQUIRED)
    ENDIF (HELLO_FOUND)
```
> cmake的add_custom_command和add_custom_target指令
```cpp
1. add_custom_target()
在很多时候，需要在cmake中创建一些目标，如clean、copy等等，这就需要通过add_custom_target来指定。同时，add_custom_command可以用来完成对add_custom_target生成的target的补充
    add_custom_target(Name [ALL] [command1 [args1...]]
                  [COMMAND command2 [args2...] ...]
                  [DEPENDS depend depend depend ... ]
                  [BYPRODUCTS [files...]]
                  [WORKING_DIRECTORY dir]
                  [COMMENT comment]
                  [JOB_POOL job_pool]
                  [VERBATIM] [USES_TERMINAL]
                  [COMMAND_EXPAND_LISTS]
                  [SOURCES src1 [src2...]])

cmake_minimum_required(VERSION 3.0)
project(test)
add_custom_target(CopyTask
  COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/config ${CMAKE_CURRENT_SOURCE_DIR}/etc
  COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/log.txt ${CMAKE_CURRENT_SOURCE_DIR}/etc
  )

mkdir build && cd build
cmake ..
make CopyTask //执行目标

初始目录结构
|---build
|---config
        |---log.log
|---CMakeLists.txt
|---log.txt
    
执行目标之后的目录结构
|---build
|---config
        |---log.log
|---etc
        |---log.log
        |---log.txt
|---CMakeLists.txt
|---log.txt
    
其实可以看出，这段代码的目的就是将config文件夹的内容和log.txt文件复制到新的etc文件夹内。
add_custom_target生成一个目标CopyTask，该目标是用来复制文件夹或者复制文件的！也就是COMMAND中定义的操作
    其中：${CMAKE_COMMAND}(/usr/bin/cmake)是CMake的路径，-E使CMake运行命令而不是构建，copy_directory和copy是cmake_command_line，再后面两个就是command_line的参数。
当然，生成文件不仅仅只能是复制，还可以是其他的操作。而这些COMMAND操作，都在command_line中规定了。
至于cmake_command_line的内容，可参考cmake的官方资料;
https://cmake.org/cmake/help/latest/manual/cmake.1.html#run-a-command-line-tool

该命令的其他一些参数的含义：
    ALL：表明该目标会被添加到默认的构建目标，使得它每次都被运行；
    COMMAND：指定要在构建时执行的命令行；
    DEPENDS：指定命令所依赖的文件；
    COMMENT：在构建时执行命令之前显示给定消息；
    WORKING_DIRECTORY：使用给定的当前工作目录执行命令。如果它是相对路径，它将相对于对应于当前源目录的构建树目录；
    BYPRODUCTS：指定命令预期产生的文件。

    
2. add_custom_command()
    将自定义构建规则添加到生成的构建系统。
2.1 生成文件
    添加定制命令来生成文件
    add_custom_command(OUTPUT output1 [output2 ...]
                   COMMAND command1 [ARGS] [args1...]``
                   [COMMAND command2 [ARGS] [args2...] ...]
                   [MAIN_DEPENDENCY depend]
                   [DEPENDS [depends...]]
                   [BYPRODUCTS [files...]]
                   [IMPLICIT_DEPENDS <lang1> depend1
                                    [<lang2> depend2] ...]
                   [WORKING_DIRECTORY dir]
                   [COMMENT comment]
                   [DEPFILE depfile]
                   [JOB_POOL job_pool]
                   [VERBATIM] [APPEND] [USES_TERMINAL]
                   [COMMAND_EXPAND_LISTS])
    
cmake_minimum_required(VERSION 3.0)
project(test)
add_custom_command(OUTPUT COPY_RES
  COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/config ${CMAKE_CURRENT_SOURCE_DIR}/etc
  COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/log.txt ${CMAKE_CURRENT_SOURCE_DIR}/etc
  )
add_custom_target(CopyTask ALL DEPENDS COPY_RES)

    add_custom_target生成一个目标CopyTask，该目标依赖于COPY_RES。而对于COPY_RES而言，它实际上是用来复制文件夹或者复制文件的！也就是COMMAND中定义的操作
    该命令的其他一些参数的含义：
    OUTPUT：指定命令预期产生的输出文件。如果输出文件的名称是相对路径，即相对于当前的构建的源目录路径；
    COMMAND：指定要在构建时执行的命令行；
    DEPENDS：指定命令所依赖的文件；
    COMMENT：在构建时执行命令之前显示给定消息；
    WORKING_DIRECTORY：使用给定的当前工作目录执行命令。如果它是相对路径，它将相对于对应于当前源目录的构建树目录；
    DEPFILE：为生成器指定一个.d depfile .d文件保存通常由自定义命令本身发出的依赖关系；
    MAIN_DEPENDENCY：指定命令的主要输入源文件；
    BYPRODUCTS：指定命令预期产生的文件。

    
2.2 构建事件
    为某个目标(如库或可执行程序)添加一个定制命令。
    这种定制命令可以设置在，构建这个目标过程中的某些时机。也就是就，这种场景可以在目标构建的过程中，添加一些额外执行的命令。这些命令本身将会成为该目标的一部分。注意，仅在目标本身被构建过程才会执行。如果该目标已经构建，命令将不会执行。
那么这些时机是什么呢？如下表所示：
    参数	       含义
    PRE_BUILD	在目标中执行任何其他规则之前运行
    PRE_LINK	在编译源代码之后，链接二进制文件或库文件之前运行
    POST_BUILD	在目标内所有其他规则均已执行后运行
    
   add_custom_command(TARGET <target>
                   PRE_BUILD | PRE_LINK | POST_BUILD
                   COMMAND command1 [ARGS] [args1...]
                   [COMMAND command2 [ARGS] [args2...] ...]
                   [BYPRODUCTS [files...]]
                   [WORKING_DIRECTORY dir]
                   [COMMENT comment]
                   [VERBATIM] [USES_TERMINAL])
    
cmake_minimum_required(VERSION 3.0)
project(test)
add_custom_target(CopyTask)
add_custom_command(TARGET CopyTask
  POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/config ${CMAKE_CURRENT_SOURCE_DIR}/etc
  COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/log.txt ${CMAKE_CURRENT_SOURCE_DIR}/etc
  )
    
这段代码的功能和上一段是一样的，将config文件夹的内容和log.txt文件复制到新的etc文件夹内。
需要注意的是，此时add_custom_command需要写在add_custom_target之后，否则将cmake不通过。
    该命令的其他一些参数的含义：
    TARGET：指定命令运行的目标；
    COMMAND：指定要在构建时执行的命令行；
    COMMENT：在构建时执行命令之前显示给定消息；
    WORKING_DIRECTORY：使用给定的当前工作目录执行命令。如果它是相对路径，它将相对于对应于当前源目录的构建树目录；
    BYPRODUCTS：指定命令预期产生的文件。
    
 
example1:
    cmake_minimum_required(VERSION 3.5 FATAL_ERROR)
    project(recipe-01 LANGUAGES CXX)
    set(CMAKE_CXX_STANDARD 11)
    set(CMAKE_CXX_EXTENSIONS OFF)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
    2. 我们将自定义目标添加到构建系统中，自定义目标将提取构建目录中的库文件 
       构建系统中引入了一个名为unpack-eigen的目标。因为我们传递了ALL参数，目标将始终被执行。
    add_custom_target(unpack-eigen
      ALL
      COMMAND
          ${CMAKE_COMMAND}-E tar xzf ${CMAKE_CURRENT_SOURCE_DIR}/eigen-eigen-5a0156e40feb.tar.gz
      COMMAND
          ${CMAKE_COMMAND}-E rename eigen-eigen-5a0156e40feb eigen-3.3.4
      WORKING_DIRECTORY
          ${CMAKE_CURRENT_BINARY_DIR}
      COMMENT
    "Unpacking Eigen3 in ${CMAKE_CURRENT_BINARY_DIR}/eigen-3.3.4"
    )
    3. 为源文件添加了一个可执行目标:
	    add_executable(linear-algebra linear-algebra.cpp)
    4. 由于源文件的编译依赖于Eigen头文件，需要显式地指定可执行目标对自定义目标的依赖关系:
	    add_dependencies(linear-algebra unpack-eigen)
    5. 最后，指定包含哪些目录:
		target_include_directories(linear-algebra
      		PRIVATE
          		${CMAKE_CURRENT_BINARY_DIR}/eigen-3.3.4
    	)
            
    补充：
    Usage: cmake -E <command>[arguments...]
    Available commands:
      capabilities              -Report capabilities built into cmake in JSON format
      chdir dir cmd [args...]- run command in a given directory
      compare_files file1 file2 - check if file1 is same as file2
      copy <file>... destination  - copy files to destination (either file or directory)
      copy_directory <dir>... destination   - copy content of <dir>... directories to 'destination' directory
      copy_if_different <file>... destination  - copy files if it has changed
      echo [<string>...]- displays arguments as text
      echo_append [<string>...]- displays arguments as text but nonew line
      env [--unset=NAME]...[NAME=VALUE]... COMMAND [ARG]...
    - run command in a modified environment
      environment               - display the current environment
      make_directory <dir>...- create parent and<dir> directories
      md5sum <file>...- create MD5 checksum of files
      sha1sum <file>...- create SHA1 checksum of files
      sha224sum <file>...- create SHA224 checksum of files
      sha256sum <file>...- create SHA256 checksum of files
      sha384sum <file>...- create SHA384 checksum of files
      sha512sum <file>...- create SHA512 checksum of files
      remove [-f]<file>...- remove the file(s),use-f to force it
      remove_directory dir      - remove a directory and its contents
      rename oldname newname    - rename a file or directory (on one volume)
      server                    - start cmake in server mode
      sleep <number>...- sleep for given number of seconds
      tar [cxt][vf][zjJ] file.tar [file/dir1 file/dir2 ...]
    - create or extract a tar or zip archive
      time command [args...]- run command and display elapsed time
      touch file                - touch a file.
      touch_nocreate file       - touch a file but donot create it.
    Available on UNIX only:
      create_symlink old new- create a symbolic link new-> old

add_custom_command总结
其实，可以看出尽管官方给了两种的使用情景，但是本质上没有什么区别，区别在于：
    如果使用OUTPUT参数，需要在目标的构建中指定依赖于该OUTPUT；
    如果使用TARGET参数，直接指定目标就可以了。
```

> cmake中的几种include 
```cpp
include_directories（）
target_include_directories（）
add_executable( xx.cpp     xx.h)

1. include_directories（）
include_directories（）的影响范围最大，可以为CMakelists.txt后的所有项目添加头文件目录
一般写在最外层CMakelists.txt中影响全局

2. target_include_directories（）
target_include_directories（）的影响范围可以自定义。如加关键子PRIVATE或这PUBLIC。
一般引用库路径使用这个命令，作为外部依赖项引入进来，target是自己项目生成的lib。
如:
project(myLib)
target_include_directories（myLib PRIVATE ${OpenCV_Include_dir}）
    
3. add_executable（）
add_executable( )中添加的引用路径一般是当前目录下的源文件对应的头文件。是生成项目时引入的头文件。
这种方式一般用于自己写的或某项目需要的头文件，这种方式需要加添加文件名字，而非头文件目录
如：
project(addFunc)
add_executable（addFunc addFunc.h  addFunc.cpp）

    
1. 指令说明
target_include_directories()：指定目标包含的头文件路径。
target_link_libraries()：指定目标链接的库。
target_compile_options()：指定目标的编译选项。
目标 由 add_library() 或 add_executable() 生成。
这三个指令类似，这里以 target_include_directories() 为例进行讲解。
    
target_include_directories()和target_link_libraries()
    如果源文件(例如CPP)中包含第三方头文件，但是头文件（例如hpp）中不包含该第三方文件头，采用PRIVATE。
    如果源文件和头文件中都包含该第三方文件头，采用PUBLIC。
    如果头文件中包含该第三方文件头，但是源文件(例如CPP)中不包含，采用 INTERFACE。

 
2. 指令讲解
测试工程目录结构：
cmake-test/                 工程主目录，main.c 调用 libhello-world.so
├── CMakeLists.txt
├── hello-world             生成 libhello-world.so，调用 libhello.so 和 libworld.so
│   ├── CMakeLists.txt
│   ├── hello               生成 libhello.so 
│   │   ├── CMakeLists.txt
│   │   ├── hello.c
│   │   └── hello.h         libhello.so 对外的头文件
│   ├── hello_world.c
│   ├── hello_world.h       libhello-world.so 对外的头文件
│   └── world               生成 libworld.so
│       ├── CMakeLists.txt
│       ├── world.c
│       └── world.h         libworld.so 对外的头文件
└── main.c
    
调用关系：
                                 ├────libhello.so
可执行文件────libhello-world.so
                                 ├────libworld.so

关键字用法说明：
PRIVATE：私有的。生成 libhello-world.so时，只在 hello_world.c 中包含了 hello.h，libhello-world.so 对外的头文件——hello_world.h 中不包含 hello.h。而且 main.c 不会调用 hello.c 中的函数，或者说 main.c 不知道 hello.c 的存在，那么在 hello-world/CMakeLists.txt 中应该写入：
target_link_libraries(hello-world PRIVATE hello)
target_include_directories(hello-world PRIVATE hello)

INTERFACE：接口。生成 libhello-world.so 时，只在libhello-world.so 对外的头文件——hello_world.h 中包含 了 hello.h， hello_world.c 中不包含 hello.h，即 libhello-world.so 不使用 libhello.so 提供的功能，只使用 hello.h 中的某些信息，比如结构体。但是 main.c 需要使用 libhello.so 中的功能。那么在 hello-world/CMakeLists.txt 中应该写入：
target_link_libraries(hello-world INTERFACE hello)
target_include_directories(hello-world INTERFACE hello)

PUBLIC：公开的。PUBLIC = PRIVATE + INTERFACE。生成 libhello-world.so 时，在 hello_world.c 和 hello_world.h 中都包含了 hello.h。并且 main.c 中也需要使用 libhello.so 提供的功能。那么在 hello-world/CMakeLists.txt 中应该写入：
target_link_libraries(hello-world PUBLIC hello)
target_include_directories(hello-world PUBLIC hello)

实际上，这三个关键字指定的是目标文件依赖项的使用范围（scope）或者一种传递（propagate）。
可执行文件依赖 libhello-world.so， libhello-world.so 依赖 libhello.so 和 libworld.so。

    main.c 不使用 libhello.so 的任何功能，因此 libhello-world.so 不需要将其依赖—— libhello.so 传递给 main.c，hello-world/CMakeLists.txt 中使用 PRIVATE 关键字；main.c 使用 libhello.so 的功能，但是libhello-world.so 不使用，hello-world/CMakeLists.txt 中使用 INTERFACE 关键字；main.c 和 libhello-world.so 都使用 libhello.so 的功能，hello-world/CMakeLists.txt 中使用 PUBLIC 关键字；

3. include_directories(dir)
target_include_directories() 的功能完全可以使用 include_directories() 实现。但是我还是建议使用 target_include_directories()。为什么？保持清晰！
include_directories(header-dir) 是一个全局包含，向下传递。什么意思呢？就是说如果某个目录的 CMakeLists.txt 中使用了该指令，其下所有的子目录默认也包含了header-dir 目录。
上述例子中，如果在顶层的 cmake-test/CMakeLists.txt 中加入：
include_directories(hello-world)
include_directories(hello-world/hello)
include_directories(hello-world/world)
那么整个工程的源文件在编译时都会增加：
-I hello-world -I hello-world/hello -I hello-world/world ...
各级子目录中无需使用 target_include_directories() 或者 include_directories()了。如果此时查看详细的编译过程（make VERBOSE=1）就会发现编译过程是一大坨，很不舒服。
当然了，在最终子目录的 CMakeLists.txt 文件中，使用 include_directories() 和 target_include_directories() 的效果是相同的。
    
4. 目录划分
每一个目录都是一个模块，目录内部应将对外和对内的头文件进行区分，由模块的调用者决定模块是否被传递（PRIVATE，INTERFACE，PUBLIC）。
```

> execute_process
```cpp
execute_process命令将从当前正在执行的CMake进程中派生一个或多个子进程，从而提供了在配置项目时运行任意命令的方法。可以在一次调用execute_process时执行多个命令。但请注意，每个命令的输出将通过管道传输到下一个命令中。该命令接受多个参数:

    WORKING_DIRECTORY，指定应该在哪个目录中执行命令。
    RESULT_VARIABLE将包含进程运行的结果。这要么是一个整数，表示执行成功，要么是一个带有错误条件的字符串。
    OUTPUT_VARIABLE和ERROR_VARIABLE将包含执行命令的标准输出和标准错误。由于命令的输出是通过管道传输的，因此只有最后一个命令的标准输出才会保存到OUTPUT_VARIABLE中。
    INPUT_FILE指定标准输入重定向的文件名
    OUTPUT_FILE指定标准输出重定向的文件名
    ERROR_FILE指定标准错误输出重定向的文件名
    设置OUTPUT_QUIET和ERROR_QUIET后，CMake将静默地忽略标准输出和标准错误。
    设置OUTPUT_STRIP_TRAILING_WHITESPACE，可以删除运行命令的标准输出中的任何尾随空格
    设置ERROR_STRIP_TRAILING_WHITESPACE，可以删除运行命令的错误输出中的任何尾随空格。
    set(_module_name "cffi")
    execute_process(
      COMMAND
          ${PYTHON_EXECUTABLE}"-c""import ${_module_name}; print(${_module_name}.__version__)"
      OUTPUT_VARIABLE _stdout
      ERROR_VARIABLE _stderr
      OUTPUT_STRIP_TRAILING_WHITESPACE
      ERROR_STRIP_TRAILING_WHITESPACE
    )
    if(_stderr MATCHES "ModuleNotFoundError")
        message(STATUS "Module ${_module_name} not found")
    else()
      message(STATUS "Found module ${_module_name} v${_stdout}")
    endif()
        
    输出：
        --FoundPythonInterp:/home/user/cmake-cookbook/chapter-05/recipe-02/example/venv/bin/python (found version "3.6.5")
		--Foundmodule cffi v1.11.5
```

> get_filename_component
```cpp
get_filename_component(<VAR> <FileName> <COMP> [CACHE])
Set <VAR> to a component of <FileName>, where <COMP> is one of:
DIRECTORY = Directory without file name
NAME      = File name without directory
EXT       = File name longest extension (.b.c from d/a.b.c)
NAME_WE   = File name without directory or longest extension
ABSOLUTE  = Full path to file
REALPATH  = Full path to existing file with symlinks resolved
PATH      = Legacy alias for DIRECTORY (use for CMake <= 2.8.11)
```


# cmake 交叉编译

```cpp
cmake交叉编译配置
    CMake给交叉编译预留了一个很好的变量CMAKE_TOOLCHAIN_FILE，它定义了一个文件的路径，这个文件即toolChain，里面set了一系列你需要改变的变量和属性，包括C_COMPILER,CXX_COMPILER,如果用Qt的话需要更改QT_QMAKE_EXECUTABLE以及如果用BOOST的话需要更改的BOOST_ROOT(具体查看相关Findxxx.cmake里面指定的路径)。CMake为了不让用户每次交叉编译都要重新输入这些命令，因此它带来toolChain机制，简而言之就是一个cmake脚本，内嵌了你需要改变以及需要set的所有交叉环境的设置;

# toolChainz中的变量可以在CMakeLists.txt中使用
# toolChain的主要作用是可以让程序员选择性指定文件编译
toolChain脚本中设置的几个重要变量
1. CMAKE_SYSTEM_NAME
即你目标机target所在的操作系统名称，比如ARM或者Linux你就需要写"Linux",如果Windows平台你就写"Windows",如果你的嵌入式平台没有相关OS你即需要写成"Generic",只有当CMAKE_SYSTEM_NAME这个变量被设置了，CMake才认为此时正在交叉编译，它会额外设置一个变量CMAKE_CROSSCOMPILING为TRUE.
    
2. CMAKE_C_COMPILER
    顾名思义，即C语言编译器，这里可以将变量设置成完整路径或者文件名，设置成完整路径有一个好处就是CMake会去这个路径下去寻找编译相关的其他工具比如linker,binutils等，如果你写的文件名带有arm-elf等等前缀，CMake会识别到并且去寻找相关的交叉编译器。

3.CMAKE_CXX_COMPILER
    同上，此时代表的是C++编译器。
    
4.CMAKE_FIND_ROOT_PATH
    指定了一个或者多个优先于其他搜索路径的搜索路径。比如你设置了/opt/arm/，所有的Find_xxx.cmake都会优先根据这个路径下的/usr/lib,/lib等进行查找，然后才会去你自己的/usr/lib和/lib进行查找，如果你有一些库是不被包含在/opt/arm里面的，你也可以显示指定多个值给
set(CMAKE_FIND_ROOT_PATH /opt/arm /opt/inst)
    
5.CMAKE_FIND_ROOT_PATH_MODE_PROGRAM
	对FIND_PROGRAM()起作用，有三种取值，NEVER,ONLY,BOTH,第一个表示不在你CMAKE_FIND_ROOT_PATH下进行查找，第二个表示只在这个路径下查找，第三个表示先查找这个路径，再查找全局路径，对于这个变量来说，一般都是调用宿主机的程序，所以一般都设置成NEVER

6.CMAKE_FIND_ROOT_PATH_MODE_LIBRARY
     对FIND_LIBRARY()起作用，表示在链接的时候的库的相关选项，因此这里需要设置成ONLY来保证我们的库是在交叉环境中找的.

7.CMAKE_FIND_ROOT_PATH_MODE_INCLUDE
    对FIND_PATH()和FIND_FILE()起作用，一般来说也是ONLY,如果你想改变，一般也是在相关的FIND命令中增加option来改变局部设置，有NO_CMAKE_FIND_ROOT_PATH,ONLY_CMAKE_FIND_ROOT_PATH,BOTH_CMAKE_FIND_ROOT_PATH

8.BOOST_ROOT
    对于需要boost库的用户来说，相关的boost库路径配置也需要设置，因此这里的路径即ARM下的boost路径，里面有include和lib
    
9.QT_QMAKE_EXECUTABLE
    对于Qt用户来说，需要更改相关的qmake命令切换成嵌入式版本，因此这里需要指定成相应的qmake路径（指定到qmake本身）
```

# cmake:macro,function中ARGV,ARGN参数的区别
```cpp
cmake中的宏(macro)和函数(function)都支持动态参数
变量ARGC记录传入的参数个数
变量ARGV0,ARGV1,...顺序代表传入的参数
变量ARGV则是一个包含所有传入参数的list
变量ARGN也是一个包含传入参数的list，但不是所有参数，而是指macro/function声明的参数之后的所有传入参数

example1
# 定义一个宏，显式声明了两个参数hello,world
macro(argn_test hello world)
	MESSAGE(STATUS ARGV=${ARGV})
	MESSAGE(STATUS ARGN=${ARGN})
	MESSAGE(STATUS ARGV0=${ARGV0})
	MESSAGE(STATUS ARGV1=${ARGV1})
	MESSAGE(STATUS ARGV2=${ARGV2})
	MESSAGE(STATUS ARGV3=${ARGV3})
endmacro

# 调用宏时传入4个参数
argn_test(TOM JERRY SUSAN BERN)

# 输出结果
-- ARGV=TOMJERRYSUSANBERNexample2
-- ARGN=SUSANBERN
-- ARGV0=TOM
-- ARGV1=JERRY
-- ARGV2=SUSAN
-- ARGV3=BERN
    
example2
使用cmake的cmake_parse_arguments来解析函数参数，它有点像解析一个map键值对，首先看下它的函数原型：
    
include (CMakeParseArguments)  #必须包含这个cmake文件才能使用cmake_parse_arguments
CMAKE_PARSE_ARGUMENTS(<prefix> <options> <one_value_keywords> <multi_value_keywords> args...)
 
首先，prefix是一个前缀，等会儿在引用参数的时候会提到，<option>是一个列表，里面可以包含一些你感兴趣的KeyWord，随后可以通过它来看看你所需要的KeyWord是否被设置。 <one_value_keywords>是一个单值参数的KeyWord列表。 <multi_value_keywords> 是一个多值参数的KeyWord列表（如list），下面举个例子，看看如何使用它们，首先定义所需要的函数，由于参数是由CMAKE_PARSE_ARGUMENTS来解析的，所以在函数声明中就不需要定义参数了：
    
function(tst_arguments)
  (
    TEST "" "NAME;COMMAND;BASELINE"
       "ARGSLIST"
       ${ARGN}
  )
  message("TEST_DEFAULT_ARGS is ${TEST_DEFAULT_ARGS} from ${ARGN}")
  message("TEST_NAME is ${TEST_NAME}")
  message("TEST_COMMAND is ${TEST_COMMAND}")
  message("TEST_ARGSLIST is ${TEST_ARGSLIST}")
  message("TEST_BASELINE is ${TEST_BASELINE}")
endfunction(tst_arguments)
这里的前缀是TEST， <one_value_keywords>我们设置单值参数的KeyWord（NAME;COMMAND;BASELINE），这将在随后的函数调用中注明KeyWord和Value的关系，<multi_value_keywords>我们设置多值参数的KeyWord（"ARGSLIST"），调用函数：

tst_arguments(
    NAME
      testiso
    COMMAND
      "RunMe"
    ARGSLIST
      ${SRC}//a.cpp;b.cpp;c.cpp;d.cpp;
    BASELINE
      "/home/sakaue/iWork"
)

==== output ====
TEST_DEFAULT_ARGS is  from NAME;testiso;COMMAND;RunMe;ARGSLIST;a.cpp;b.cpp;c.cpp;d.cpp;BASELINE;/home/sakaue/iWork
TEST_NAME is testiso
TEST_COMMAND is RunMe
TEST_ARGSLIST is a.cpp;b.cpp;c.cpp;d.cpp
TEST_BASELINE is /home/sakaue/iWork

example3:
cmake_minimum_required(VERSION 3.5)
project(test)
function(test_prase)
    set(options op1 op2 op3)
    set(oneValueArgs v1 v2 v3)
    set(multiValueArgs m1 m2)
    messages(STATUS "options=${options}")
    messages(STATUS "oneValueArgs=${oneValueArgs}")
    messages(STATUS "multiValueArgs=${multiValueArgs}")
    cmake_parse_arguments(MYPER "${options}" "${oneValeArgs}" "${multiValueArgs}" ${ARGN})
    messages(STATUS "op1=${MYPER_op1}")
    messages(STATUS "op2=${MYPER_op2}")
    messages(STATUS "op3=${MYPER_op3}")
    
    messages(STATUS "v1=${MYPER_v1}")
    messages(STATUS "v2=${MYPER_v2}")
    messages(STATUS "v3=${MYPER_v3}")
    
    messages(STATUS "m1=${MYPER_m1}")
    messages(STATUS "m2=${MYPER_m2}")
endfunction(test_prase)
    
messages(STATUS "\n")
test_prase(op1 op2 op3 v1 aaa v2 111 v3 bbb m1 1 2 3 4 5 m2 a b c)
    
messages(STATUS "\n")
test_prase(op1 op3 v1 aaa v2 111 v3 bbb m1 1 2 3 4 5 m2 a b c)
    
messages(STATUS "\n")
test_prase(op1 op3 v1 aaa v2 v3 bbb m1 1 2 3 4 5)
    
messages(STATUS "\n")
test_prase(op1 v1 aaa v2 111 v3 bbb m1 1 2 3 4 5 m2 a b c op3)
    
输出：
    options = op1;op2;op3;
	oneValueArgs=v1;v2;v3;
	multiValueArgs=m1;m2;
	op1 = TRUE
    op2 = TRUE
    op3 = TRUE
    v1 = aaa
    v2 = 111
    v3 = bbb
    m1 = 1;2;3;4;5
    m2 = a;b;c;

    options = op1;op2;op3;
	oneValueArgs=v1;v2;v3;
	multiValueArgs=m1;m2;
	op1 = TRUE
    op2 = FALSE
    op3 = TRUE
    v1 = aaa
    v2 = 111
    v3 = bbb
    m1 = 1;2;3;4;5
    m2 = a;b;c;

    options = op1;op2;op3;
	oneValueArgs=v1;v2;v3;
	multiValueArgs=m1;m2;
	op1 = TRUE
    op2 = False
    op3 = TRUE
    v1 = aaa
    v2 = 
    v3 = bbb
    m1 = 1;2;3;4;5
    m2
        
    options = op1;op2;op3;
	oneValueArgs=v1;v2;v3;
	multiValueArgs=m1;m2;
	op1 = TRUE
    op2 = TRUE
    op3 = TRUE
    v1 = aaa
    v2 = 
    v3 = bbb
    m1 = 1;2;3;4;5
    m2 = a;b;c;
```


# cmake两种变量的原理

```cpp
本文记录一下 CMake 变量的定义、原理及其使用。CMake 变量包含 Normal Variables、Cache Variables。通过 set 指令可以设置两种不同的变量。也可以在 CMake 脚本中使用和设置环境变量。set(ENV{<variable>} <value>...)，本文重点讲述 CMake 脚本语言特有的两种变量。
    

1. Normal Variables
    通过 set(<variable> <value>... [PARENT_SCOPE])这个命令来设置的变量就是 Normal Variables。例如 set(MY_VAL "666") ，此时 MY_VAL 变量的值就是 666。
    
2.Cache Variables
    通过 set(<variable> <value>... CACHE <type> <docstring> [FORCE])这个命令来设置的变量就是 Cache Variables。例如 set(MY_CACHE_VAL "666" CACHE STRING INTERNAL)，此时 MY_CACHE_VAL 就是一个 CACHE 变量。

example1
.
├── CMakeLists.txt
└── src
    └── CMakeLists.txt
    
根目录 CMakeLists.txt 文件内容如下:
cmake_minimum_required(VERSION 3.10)
set(MY_GLOBAL_VAR "666" CACHE STRING INTERNAL )
message("第一次在父目录 CMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX}")
message("第一次在父目录 MY_GLOBAL_VAR=${MY_GLOBAL_VAR}")
add_subdirectory(src)
message("第二次在父目录 CMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX}")
message("第二次在父目录 MY_GLOBAL_VAR=${MY_GLOBAL_VAR}")
set(CMAKE_INSTALL_PREFIX "-->usr" )
message("第三次在父目录 CMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX}")

src/CMakeLists.txt 文件内容如下:
cmake_minimum_required(VERSION 3.10)
message("子目录,CMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX}")
message("子目录,MY_GLOBAL_VAR=${MY_GLOBAL_VAR}")
set(CMAKE_INSTALL_PREFIX "/usr" CACHE STRING INTERNAL FORCE)
set(MY_GLOBAL_VAR "777" CACHE STRING INTERNAL FORCE )
    
$ cmake .
第一次在父目录 CMAKE_INSTALL_PREFIX=/usr/local
第一次在父目录 MY_GLOBAL_VAR=666
子目录,CMAKE_INSTALL_PREFIX=/usr/local
子目录,MY_GLOBAL_VAR=666
第二次在父目录 CMAKE_INSTALL_PREFIX=/usr
第二次在父目录 MY_GLOBAL_VAR=777
第三次在父目录 CMAKE_INSTALL_PREFIX=-->usr
    
tips:
对于修改 Cache 变量的另一种方式就是cmake -D CMAKE_INSTALL_PREFIX=/usr。可以自己验证。这里说一个重要的点，就是在终端中输入的 cmake -D var=value . 如果 CMake 中默认有这个 var Cache 变量，那么此时就是赋值，没有的话，CMake 就会默认创建了一个全局 Cache 变量然后赋值。(tips: $CACHE{VAR}表示获取 CACHE 缓存变量的值)
    
1. 总结：
    结合 include() 、macro() 最后结果，能够得出一个结论：通过 include() 和 macro() 相当于把这两部分包含的代码直接加入根目录 CMakeLists.txt 文件中去执行，相当于他们是一个整体。因此变量直接都是互通的。这就有点像 C/C++ 中的 #include 包含头文件的预处理过程了。这一点其实与刚开始讲的 function() 、add_subdirectory() 完全不同，在函数以及 add_subdirectory() 中，他们本身就是一个不同的作用域范围，仅仅通过拷贝调用者的 Normal 值(仅仅在调用 add_subdirectory() / function() 之前的 Normal 变量)，如果要修改调用者包含的 Normal 变量，那么只能通过 set(MY_VAL "某个值" PARENT_SCOPE)注明我们修改的是调用者 Normal 值。虽然在 C/C++ 中，可以通过指针的方式，通过函数可以修改外部变量值，但是在 CMake 脚本语言中 function() 虽然能够传入形式参数，但是者本质上就是 C/C++ 中的值拷贝。而不是引用。上面所说的 Normal 变量其实就是一个局部变量。
    
2、Cache Variables
    ache 变量 CMAKE_INSTALL_PREFIX 默认值是 /usr/local (可以在生成的 CMakeCache.txt 文件中查看)，这时候如果我们 在某个 CMakeLists.txt 中，仍然使用 set(CMAKE_INSTALL_PREFIX "/usr")，那么此时我们 install 的时候，CMake 以后面的 /usr 作为 CMAKE_INSTALL_PREFIX 的值，这是因为 CMake 规定，有一个与 Cache 变量同名的 Normal 变量出现时，后面使用这个变量的值都是以 Normal 为准，如果没有同名的 Normal 变量，CMake 才会自动使用 Cache 变量。
所有的 Cache 变量都会出现在 CMakeCache.txt 文件中。这个文件是我们键入 cmake .命令后自动出现的文件。打开这个文件发现，CMake 本身会有一些默认的全局 Cache 变量。例如：CMAKE_INSTALL_PREFIX、CMAKE_BUILD_TYPE、CMAKE_CXX_FLAGSS 等等。可以自行查看。当然，我们自己定义的 Cache 变量也会出现在这个文件中。Cache 变量定义格式为 set(<variable> <value> CACHE STRING INTERNAL)。这里的 STRING可以替换为 BOOL FILEPATH PATH ，但是要根据前面 value 类型来确定。参考。
修改 Cache 变量。可以通过 set(<variable> <value> CACHE INSTERNAL FORCE)，另一种方式是直接在终端中使用 cmake -D var=value .. 来设定默认存在的CMake Cache 变量。
```

# cmake (Linux ar合并多个静态库)

```cpp
为什么要合并多个静态库

实际嵌入式项目开发中我们经常会使用静态库，这些静态库一般都是通过自动构建工具来生成。这种方式虽然很方便，但当静态库的数量非常多的时候管理起来就不太容易了。这时我们可以尝试将这些静态库进行合并，这样就能够减少静态库的数量，从而减轻【管理多个静态库的工作量】。
ar 命令
使用 GNU 编译套件来编译并打包生成静态库时，ar 命令完成打包生成静态库的任务。ar 命令既可以创建新的静态库，也可以查看、修改已经生成的静态库。这里要合并静态库从 ar 命令着手是一个比较好的方向。
    
查看命令说明文档
如果你是在 Windows 中使用交叉编译器来编译，那么你可以在编译器的安装目录的【share】子目录中找到 GNU 编译器相关的文档。ar 命令手册并不在独立的文档中，它在【binutils.pdf】 中。

查看与 ar 命令相关的部分，我发现 ar 命令可以使用脚本来进行控制。按照手册的描述，这里提到的脚本与 【MRI “librarian”】 程序使用的脚本一致。脚本实际非常简单，提供了几种不同的命令来使用。你可以在 ar 的交互式模式中输入这些命令来操作，也可以预先将这些命令写入到文件中，以这个文件来重定向标准输入也能完成控制，这里我使用后者。
    
ar 控制脚本的主要内容
研究发现这个脚本的内容可以分为如下几个方面：
    使用 create、open 来指定一个当前使用的静态库，这个静态库是大部分其它命令需要的临时文件
    create 用于【创建】新的文件，open 用于【打开】已经存在的静态库。
    打开一个静态库后，你可以执行【添加、删除、替换、提取、查看】等命令
    执行 save 命令【保存修改】到文件中
    执行 end 退出 ar，并返回 0 表示操作成功执行。这个命令并不会保存输出文件。这也就意味着你在最后一次 save 命令执行后如果还执行了其它修改库的命令，这些修改将会【丢失】。

一个示例
下面是我用【awtk】编译出的静态库进行的测试。脚本内容如下：
    create  libawtk-all.a
    addlib  libassets.a
    addlib  libawtk.a
    addlib  libcommon.a
    addlib  libfont_gen.a
    addlib  libglad.a
    addlib  libgpinyin.a
    addlib  libimage_gen.a
    addlib  liblinebreak.a
    addlib  libnanovg.a
    addlib  libSDL2.a
    addlib  libstr_gen.a
    addlib  libtheme_gen.a
    save
    end
上述脚本将【awtk】中 lib 目录下的所有静态库合并为一个命名为 libawtk-all.a 的静态库。
将上述文件保存为 ar-src 文件，执行 ar -M < ar-src 命令就能够完成静态库的合并。执行结果如下：
```

