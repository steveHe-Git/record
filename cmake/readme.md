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
    configure_file 命令用于加入一个配置头文件 config.h ，这个文件由 CMake 从 config.h.in 生成，通过这样的机制，将可以通过预定义一些参数和变量来控制代码的生成
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
  ${PROJECT_BINARY_DIR}
	${PROJECT_SOURCE_DIR}  
1. 如果是in source编译,建立一个build文件夹， cmake ../ 或者cmake ../src ../或者../src指定源文件的和CMakeLists.txt的位置
    ${PROJECT_BINARY_DIR} ==>  ./build
    ${PROJECT_SOURCE_DIR} ==>  ./      (cmake ../)
    ${PROJECT_BINARY_DIR} ==>  ./build
    ${PROJECT_SOURCE_DIR} ==>  ./src      (cmake ../src)
2. 如果是not in source编译,不建立一个build文件夹， cmake ../ 或者cmake ../src ../或者../src指定源文件的和CMakeLists.txt的位置
    ${PROJECT_BINARY_DIR} ==>  ./      （cmake ./）
    ${PROJECT_SOURCE_DIR} ==>  ./	   （cmake ./）
    ${PROJECT_BINARY_DIR} ==>  ./      （cmake ./）
    ${PROJECT_SOURCE_DIR} ==>  ./	   （cmake ./src）

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
环境变量使用$ENV{}方式取值,使用SET(ENV{VAR} VALUE)赋值

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