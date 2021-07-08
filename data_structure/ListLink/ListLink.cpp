/*
* CMake Error: CMake can not determine linker language for target: listlink_lib.c
* 意思是说不确定用什么语言编译,最终确定确实是我的对应的lib都是由模板类头文件.hpp生成的，而其中并没有.cpp或者.c文件
* 解决办法：
* 1．增加空的.c或者.cpp文件
* 2.  在CMakeLists.txt对应的你要生成的lib或可执行文件的项目下面加上如下语句
* #要生成的库
* add_library(lib_name template_header.hpp)
* #强制按照C++语言编译
* set_target_properties(lib_name PROPERTIES LINKER_LANGUAGE CXX)
*/


//t***********/
// 注意:
// c++模板的实现和申明是不能分开的，
// 必须放在一个文件里面，不然链接库函数会出现　undefined reference to　函数找不到的情况
