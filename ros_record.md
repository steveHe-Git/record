# ros 简介

## 二进制vs源码安装

```cpp
区别
下载方式         二进制包                       源代码包
下载方式         apt-get install/直接下载deb    git clone/直接下载源代码
ROS包存放位置 	/opt/ros/kinetic/	          通常 	~/catkin_ws/src	
编译方式        无需编译                        通过make/cmake/caktin
来源            官方apt软件源                   开源项目、第三方开发者
扩展性          无法修改                        通过源代码修改
可读性          无法查看源代码                   方便阅读源代码
优点           下载简单,安装方便                 源码可修改,便于定制功能
缺点           无法修改                         编译工具、软件包依赖、版本和参数
应用场景        基础软件                         需要查看、开发和修改的程序
```

## ROS特点和优缺点

```cpp
1. 分布式点对点
    ROS采用了分布式的框架,通过点对点的设计让机器人的进程可以分别运行,便于模块化的修改和定制,提高了系统的容错能力。
2. 多种语言支持
    ROS支持多种编程语言。C++、Pyhton和已经在ROS中实现编译,是目前应用最广的ROS开发语言；为了支持多语言编程,ROS采用了一种语言中立的接口定义语言来实现各模块之间消息传送。通俗的理解就是,ROS的通信格式和用哪种编程语言来写无关,它使用的是自身定义的一套通信接口。
3. 开源社区
    ROS具有一个庞大的社区ROS	WIKI(http://wiki.ros.org/)，此外,ROS遵从BSD协议,对个人和商业应用及修改完全免费。这也促进了ROS的流行
    
优缺点:                        
    优点                       缺点
    提供框架、工具和功能         通信实时性能有限
    方便移植                   系统稳定性尚不满足工业级要求
    庞大的用户群体              安全性上没有防护措施
    免费开源                   仅支持Linux(Ubuntu)
```

## 什么是ROS

```cpp
	ROS是一个适用于机器人编程的框架,这个框架把原本松散的零部件耦合在了一起,为他们提供了通信架构。ROS虽然叫做操作系统,但并非Windows、Mac那样通常意义的操作系统,它只是连接了操作系统和你开发的ROS应用程序,所以它也算是一个中间件,基于ROS的应用程序之间建立起了沟通的桥梁,所以也是运行在Linux上的运行时环境,在这个环境上,机器人的感知、决策、控制算法可以更好的组织和运行。
```

## source 命令

```cpp
	注意:source命令,编译完成后必须刷新一下工作空间的环境,否则可能找不到工作空间。许多时候我们为了打开终端就能够运行工作空间中编译好的ROS程序,我们习惯把 	source ~/catkin_ws/devel/setup.bash	 命令追加到 	~/.bashrc文件中(rosacademy_ws替换为你的工作空间名称),这样每次打开终端,系统就会刷新工作空间环境。你可以通过 	echo "source ~/catkin_ws/devel/setup.bash" >> ~/.bashrc	 命令来追加。
```

## 分布式特点

```cpp
	键盘控制仿真机器人移动这个demo展现了ROS分布式消息收发的特性。我们打开的虽然是键盘控制程序,但它可以替换为手柄控制、手机平板控制、甚至是路径规划自动控制。模拟器里的机器人并不关心是谁发给它的消息,它只关心这个消息是什么(速度、角速度等指令)。所以,每一个进程(节点)都各司其职,负责不同的任务,同时定义好消息收发的接口。如果我们现在要做路径规划任务,那么我们只用单独再开发一个节点,同样向底盘发送我们解算出的速度/角速度指令就可以了
```

# ROS文件系统

```cpp
	介绍catkin的编译系统,catkin工作空间的创建和结构,package软件包的创建和结构,介绍CMakeLists.txt文件,package.xml以及其他常见文件。从而系统的梳理
了ROS文件空间的结构,对于我们ROS学习和开发有着重要的作用；
```

##　Catkin编译系统

```cpp
catkin_ws(ros package)--(catkin)--> CmakeLists --(Cmake)--> Makefile --(make)--> hello.c-(gcc/g++)->hello.o-(gcc/g++)->helo
    
    早期的ROS编译系统是rosbuild,但随着ROS的不断发展,rosbuild逐渐暴露出许多缺点,不能很好满足系统需要。在Groovy版本面世后,Catkin作为rosbuild的替代品被正式投入使用。
	Catkin操作更加简化且工作效率更高,可移植性更好,而且支持交叉编译和更加合理的功能包分配。目前的ROS同时支持着rosbuild和Catkin两种编译系统,但ROS的核心软件包也已经全部转换为Catkin。
    rosbuild已经被逐步淘汰,所以建议初学者直接上手Catkin
    
1. catkin 特点
    1.1 Catkin是基于CMake的编译构建系统,具有以下特点:
	- Catkin沿用了包管理的传统, 像find_package()基础结构, pkg-config;
	- 扩展了CMake,例如
        。软件包编译之后无需安装即可使用
        。自动生成find_package()代码，pkg-config文件
        。解决多个软件包构建顺序问题
        
	1.2 一个Catkin的软件包(package)必须要包括两个文件:
	- package.xml:	包括了package的描述信息
        。name,description,version,maintainer(s),license
        。authors,url's,dependencies,plugins, and etc...
	- CMakeLists.txt:构建package所需的CMake文件
        。调用Catkin的函数/宏
        。解析package.xml
        。找到其他依赖的catkin软件包
        。将本软件包添加到环境变量中
        
2. catkin编译的工作原理:
	- 首先在工作空间 catkin_ws/src/下递归的查找其中每一个ROS的package。
	- package中会有 package.xml和 CMakeLists.txt文件,Catkin(CMake)编译系统依据 CMakeLists.txt文件,
		从而生成 makefiles(放在catkin_ws/build/)。
	- 然后 make刚刚生成的 makefiles等文件,编译链接生成可执行文件(放在 catkin_ws/devel/)。
    - catkin可以跨依赖项目编译
        
3. catkin工作空间的目录架构
   它包括了 src	、build 、devel三个路径, 它们的具体作用如下:
	- src/:	ROS的catkin软件包(源代码包)
	- build/: catkin(CMake)的缓存信息和中间文件
	- devel/: 生成的目标文件(包括头文件,动态链接库,静态链接库,可执行文件等)、环境变量
```

## package 结构

```cpp
    ├──	CMakeLists.txt				#package的编译规则(必须)
    ├──	package.xml				    #package的描述信息(必须)
    ├──	src/					    #源代码文件
    ├──	include/				    #C++头文件
    ├──	scripts/					#可执行脚本.py或者.sh
    ├──	msg/						#自定义消息
    ├──	srv/						#自定义服务
    ├──	models/						#3D模型文件
    ├──	urdf/						#urdf文件
    ├──	launch/						#launch文件
        
	其中定义package的是 CMakeLists.txt和package.xml,这两个文件是package中必不可少的。catkin编译系统在编译前,首先就要解析这两个文件。这两个文件就定义了一个package。有CMakeLists.txt和package.xml是必须的,其余路径根据软件包是否需要来决定。
    - CMakeLists.txt:	定义package的包名、依赖、源文件、目标文件等编译规则,是package不可少的成分
    - package.xml:	描述package的包名、版本号、作者、依赖等信息,是package不可少的成分
    - src/:	存放ROS的源代码,包括C++的源码和(.cpp)
    - include/:	存放C++源码对应的头文件
    - scripts/:	存放可执行脚本,例如shell脚本(.sh)、Python脚本(.py)
    - msg/:	存放自定义格式的消息(.msg)
    - srv/:	存放自定义格式的服务(.srv)
    - models/:存放机器人或仿真场景的3D模型(.sda,.stl,.dae等)
    - urdf/:存放机器人的模型描述(.urdf或.xacro)
    - launch/:存放launch文件(.launch或.xml)
```

## package 创建

```cpp
创建一个package包，需要使用catkin_create_pkg
    catkin_create_pkg package depends	
    其中package是包名,depends是依赖的包名,可以依赖多个软件包。
例如,新建一个package叫做 	test_pkg	 ,依赖roscpp、rospy、std_msgs(常用依赖)。
    catkin_create_pkg test_pkg roscpp rospy	std_msgs
    
这样就会在当前路径下新建 	test_pkg	 软件包,包括:
		├──	CMakeLists.txt
		├──	include
		│		└──	test_pkg
		├──	package.xml
		└──	src    
```

## package 相关指令

```cpp
1. rospack
	rospack是对package管理的工具,命令的用法如下:
        rostopic命令                    作用
        rospack	help	               显示rospack的用法
        rospack	list	               列出本机所有package
        rospack	depends	[package]	   显示package的依赖包
        rospack	find	[package]	   定位某个package
        rospack	profile	               刷新所有package的位置记录
            
2. roscd
	roscd命令类似与Linux系统的cd,改进之处在于roscd可以直接cd到ROS的软件包。
        rostopic命令                    作用
        roscd[pacakge]	                cd到ROS	package所在路径
            
3. rosls
	rosls也可以视为Linux指令 ls的改进版,可以直接ls	ROS软件包的内容。
        rosls命令                       作用
        rosls	[pacakge]	           列出pacakge下的文件
            
4. rosdep
	rosdep	 是用于管理ROS	package依赖项的命令行工具,用法如下:
        rosdep命令                      作用
        rosdep	check	[pacakge]	    检查package的依赖是否满足
        rosdep	install	[pacakge]	    安装pacakge的依赖
        rosdep	db	                    生成和显示依赖数据库
        rosdep	init	                初始化/etc/ros/rosdep中的源
        rosdep	keys	                检查package的依赖是否满足
        rosdep	update	                更新本地的rosdep数据库
            
	一个较常使用的命令是 	rosdep	install	--from-paths src --ignore-src --rosdistro=kinetic -y,用于安装工作空间中src路径下所有package的依赖项(由pacakge.xml文件指定)。
```

## CMakeLists.txt作用

```cpp
	CMakeLists.txt原本是Cmake编译系统的规则文件,而Catkin编译系统基本沿用了CMake的编译风格,只是针对ROS工程添加了一些宏定义;
	CMakeLists.txt的基本语法都还是按照CMake,而Catkin在其中加入了少量的宏,总体的结构如下:

        cmake_minimum_required()	#CMake的版本号	
        project()					#项目名称	
        find_package()				#找到编译需要的其他CMake/Catkin	package
        catkin_python_setup()	    #catkin新加宏,打开catkin的Python	Module的支持
        add_message_files()			#catkin新加宏,添加自定义Message/Service/Action文件
        add_service_files()
        add_action_files()
        generate_message()			#catkin新加宏,生成不同语言版本的msg/srv/action接口
        catkin_package()			#catkin新加宏,生成当前package的cmake配置,供依赖本包的其他软件包调用
        add_library()				#生成库
        add_executable()			#生成可执行二进制文件
        add_dependencies()			#定义目标文件依赖于其他目标文件,确保其他目标已被构建
        target_link_libraries()		#链接
        catkin_add_gtest()			#catkin新加宏,生成测试
        install()					#安装至本机
```

## CMakeLists.txt example

```cpp
可以roscd到tuetlesim包下查看, 在turtlesim/CMakeLists.txt的写法如下:

cmake_minimum_required(VERSION	2.8.3) #CMake至少为2.8.3版
project(turtlesim) #项目(package)名称为turtlesim,在后续文件中可使用变量${PROJECT_NAME}来引用项目名称turltesim
find_package(catkin	REQUIRED COMPONENTS	geometry_msgs message_generation rosconsole	roscpp roscpp_serialization	roslib rostime std_msgs	std_srvs)
#cmake宏,指定依赖的其他pacakge,实际是生成了一些环境变量,如<NAME>_FOUND,<NAME>_INCLUDE_DIRS, <NAME>_LIBRARYIS
#此处catkin是必备依赖	其余的geometry_msgs...为组件
find_package(Qt5Widgets	REQUIRED)
find_package(Boost REQUIRED	COMPONENTS thread)
    
include_directories(include	${catkin_INCLUDE_DIRS}	${Boost_INCLUDE_DIRS}) #指定C++的头文件路径
link_directories(${catkin_LIBRARY_DIRS}) #指定链接库的路径
add_message_files(DIRECTORY	msg	FILES Color.msg	Pose.msg)#自定义msg文件
add_service_files(DIRECTORY	srv	FILES
    Kill.srv
    SetPen.srv
    Spawn.srv
    TeleportAbsolute.srv
    TeleportRelative.srv)#自定义srv文件
    
generate_messages(DEPENDENCIES	geometry_msgs	std_msgs	std_srvs)
#在add_message_files、add_service_files宏之后必须加上这句话,用于生成srv	msg头文件/module,生
成的文件位于devel/include中
catkin_package(CATKIN_DEPENDS	geometry_msgs	message_runtime	std_msgs	std_srvs)
#	catkin宏命令,用于配置ROS的package配置文件和CMake文件
#	这个命令必须在add_library()或者add_executable()之前调用,该函数有5个可选参数:
#	(1)	INCLUDE_DIRS	-	导出包的include路径
#	(2)	LIBRARIES	-	导出项目中的库
#	(3)	CATKIN_DEPENDS	-	该项目依赖的其他catkin项目
#	(4)	DEPENDS	-	该项目所依赖的非catkin	CMake项目。
#	(5)	CFG_EXTRAS	-	其他配置选项
set(turtlesim_node_SRCS
src/turtlesim.cpp
src/turtle.cpp
src/turtle_frame.cpp
38CMakeLists.txt
)
set(turtlesim_node_HDRS
include/turtlesim/turtle_frame.h
)
#指定turtlesim_node_SRCS、turtlesim_node_HDRS变量
qt5_wrap_cpp(turtlesim_node_MOCS	${turtlesim_node_HDRS})
add_executable(turtlesim_node	${turtlesim_node_SRCS}	${turtlesim_node_MOCS})
#	指定可执行文件目标turtlesim_node
target_link_libraries(turtlesim_node	Qt5::Widgets	${catkin_LIBRARIES}	${Boost_LIBRARIE
S})
#	指定链接可执行文件
add_dependencies(turtlesim_node	turtlesim_gencpp)
add_executable(turtle_teleop_key	tutorials/teleop_turtle_key.cpp)
target_link_libraries(turtle_teleop_key	${catkin_LIBRARIES})
add_dependencies(turtle_teleop_key	turtlesim_gencpp)
add_executable(draw_square	tutorials/draw_square.cpp)
target_link_libraries(draw_square	${catkin_LIBRARIES}	${Boost_LIBRARIES})
add_dependencies(draw_square	turtlesim_gencpp)
add_executable(mimic	tutorials/mimic.cpp)
target_link_libraries(mimic	${catkin_LIBRARIES})
add_dependencies(mimic	turtlesim_gencpp)
#	同样指定可执行目标、链接、依赖
install(TARGETS	turtlesim_node	turtle_teleop_key	draw_square	mimic
RUNTIME	DESTINATION	${CATKIN_PACKAGE_BIN_DESTINATION})
#	安装目标文件到本地系统
install(DIRECTORY	images
DESTINATION	${CATKIN_PACKAGE_SHARE_DESTINATION}
FILES_MATCHING	PATTERN	"*.png"	PATTERN	"*.svg")
```

## packag.xml

```cpp
	pacakge.xml包含了package的名称、版本号、内容描述、维护人员、软件许可、编译构建工具、编译依赖、运行依赖等信息。实际上rospack find
、rosdep等命令之所以能快速定位和分析出package的依赖项信息,就
是直接读取了每一个pacakge中的package.xml文件。它为用户提供了快速了解一个pacakge的渠道。
```

## packag.xml 写法

```cpp
## 老版本
<pacakge>          根标记文件
<name>             包名
<version>          版本号
<description>      内容描述
<maintainer>       维护者
<license>          软件许可证
<buildtool_depend> 编译构建工具,通常为catkin
<build_depend>     编译依赖项
<run_depend>       运行依赖项
    说明： 其中1-6为必备标签,1是根标签,嵌套了其余的所有标签,2-6为包			的各种属性,7-9为编译相关信息。
    
## 新版本
<pacakge>             根标记文件
<name>                包名
<version>             版本号
<description>         内容描述
<maintainer>          维护者
<license>             软件许可证
<buildtool_depend>    编译构建工具,通常为catkin
<depend>              指定依赖项为编译、导出、运行需要的依赖,最常用
<build_depend>        编译依赖项
<build_export_depend> 导出依赖项
<exec_depend>         运行依赖项
<test_depend>         测试用例依赖项
<doc_depend>          文档依赖项    
    
##example old
<?xml version="1.0"?> <!--本示例为老版本的pacakge.xml-->
<package> <!--pacakge为根标签,写在最外面-->
    <name>turtlesim</name>
    <version>0.8.1</version>
    <description>
    turtlesim is a tool made for teaching ROS and ROS packages.
    </description>
    <maintainer email="dthomas@osrfoundation.org">Dirk Thomas</maintainer>
    <license>BSD</license>
    <url type="website">http://www.ros.org/wiki/turtlesim</url>
    <url type="bugtracker">https://github.com/ros/ros_tutorials/issues</url>
    <url type="repository">https://github.com/ros/ros_tutorials</url>
    <author>Josh Faust</author>
    <!--编译工具为catkin-->
    <buildtool_depend>catkin</buildtool_depend>
    <!--编译时需要依赖以下包-->
    <build_depend>geometry_msgs</build_depend>
    <build_depend>qtbase5-dev</build_depend>
    <build_depend>message_generation</build_depend>
    <build_depend>qt5-qmake</build_depend>
    <build_depend>rosconsole</build_depend>
    <build_depend>roscpp</build_depend>
    <build_depend>roscpp_serialization</build_depend>
    <build_depend>roslib</build_depend>
    <build_depend>rostime</build_depend>
    <build_depend>std_msgs</build_depend>
    <build_depend>std_srvs</build_depend>
    <!--运行时需要依赖以下包-->
    <run_depend>geometry_msgs</run_depend>
    <run_depend>libqt5-core</run_depend>
    <run_depend>libqt5-gui</run_depend>
    <run_depend>message_runtime</run_depend>
    <run_depend>rosconsole</run_depend>
    <run_depend>roscpp</run_depend>
    <run_depend>roscpp_serialization</run_depend>
    <run_depend>roslib</run_depend>
    <run_depend>rostime</run_depend>
    <run_depend>std_msgs</run_depend>
    <run_depend>std_srvs</run_depend>
</package>
        
        
## example new
<?xml version="1.0"?>
    <package format="2">
    <!--在声明pacakge时指定format2,为新版格式-->
    <name>turtlesim</name>
    <version>0.8.1</version>
    <description>
    turtlesim is a tool made for teaching ROS and ROS packages.
    </description>
    <maintainer email="dthomas@osrfoundation.org">Dirk Thomas</maintainer>
    <license>BSD</license>
    <url type="website">http://www.ros.org/wiki/turtlesim</url>
    <url type="bugtracker">https://github.com/ros/ros_tutorials/issues</url>
    <url type="repository">https://github.com/ros/ros_tutorials</url>
    <author>Josh Faust</author>
    <!--编译工具为catkin-->
    <buildtool_depend>catkin</buildtool_depend>
    <!--用depend来整合build_depend和run_depend-->
    <depend>geometry_msgs</depend>
    <depend>rosconsole</depend>
    <depend>roscpp</depend>
    <depend>roscpp_serialization</depend>
    <depend>roslib</depend>
    <depend>rostime</depend>
    <depend>std_msgs</depend>
    <depend>std_srvs</depend>
    <!--build_depend标签未变-->
    <build_depend>qtbase5-dev</build_depend>
    <build_depend>message_generation</build_depend>
    <build_depend>qt5-qmake</build_depend>
    <!--run_depend要改为exec_depend-->
    <exec_depend>libqt5-core</exec_depend>
    <exec_depend>libqt5-gui</exec_depend>
    <exec_depend>message_runtime</exec_depend>
</package>
```

## Metapackage(元包)

```cpp
	Metapackage它指的是将多个功能接近、甚至相互依赖的软件包的放到一个集合中去,把一些相近的功能模块、软件包放到一起;

Metapacakge名称           描述                           链接
navigation               导航相关的功能包集                https://github.com/ros-planning/navigation
moveit                   运动规划相关的(主要是机械臂)功能包集 https://github.com/ros-planning/moveit
image_pipeline           图像获取、处理相关的功能包集        https://github.com/ros-perception/image_common
vision_opencv            ROS与OpenCV交互的功能包集         https://github.com/ros-perception/vision_opencv
turtlebot                Turtlebot机器人相关的功能包集      https://github.com/turtlebot/turtlebot
pr2_robot                pr2机器人驱动功能包集              https://github.com/PR2/pr2_robo
 ...                        ...                          ...
     
以navigation metapackage(官方介绍里仍然沿用stack的叫法)为例,它包括了以下软件包:
包名                      功能
navigation               Metapacakge,依赖以下所有pacakge
amcl                     定位
fake_localization        定位
map_server               提供地图
move_base                路径规划节点
nav_core                 路径规划的接口类
base_local_planner       局部规划
dwa_local_planner        局部规划
    ...                   ...
```

## Metapackage 写法

```cpp
	我们以ROS-Academy-for-beginners为例介绍meteapckage的写法,在教学包内,有一个ros-academy-for-beginners软件包,该包即为一个metapacakge,其中有且仅有两个文件:pacakge.xml和CMakeLists.txt
        
CMakeLists.txt写法如下:
    cmake_minimum_required(VERSION 2.8.3)
    project(ros_academy_for_beginners)
    find_package(catkin REQUIRED)
    catkin_metapackage() #声明本软件包是一个metapacakge
        
pacakge.xml写法如下:
<package>
    <name>ros_academy_for_beginners</name>
    <version>17.12.4</version>
    <description>
    --------------------------------------------------------------------------
    A ROS tutorial for beginner level learners. This metapacakge includes some
    demos of topic, service, parameter server, tf, urdf, navigation, SLAM...
    It tries to explain the basic concepts and usages of ROS.
    --------------------------------------------------------------------------
    </description>
    <maintainer email="chaichangkun@163.com">Chai Changkun</maintainer>
    <author>Chai Changkun</author>
    <license>BSD</license>
    <url>http://http://www.droid.ac.cn</url>
    <buildtool_depend>catkin</buildtool_depend>
        
    <run_depend>navigation_sim_demo</run_depend><!--注意这里的run_depend标签,将其他软件包都设为依赖项-->
    <run_depend>param_demo</run_depend>
    <run_depend>robot_sim_demo</run_depend>
    <run_depend>service_demo</run_depend>
    <run_depend>slam_sim_demo</run_depend>
    <run_depend>tf_demo</run_depend>
    <run_depend>topic_demo</run_depend>
        
    <export>
        <!--这里需要有export和metapacakge标签,注意这种固定写法-->
        <metapackage/>
    </export>
</package>
        
metapacakge中的以上两个文件和普通pacakge不同点是:
	。CMakeLists.txt:加入了catkin_metapackage()宏,指定本软件包为一个metapacakge。
    。package.xml:标签将所有软件包列为依赖项,标签中添加标签声明。
metapacakge在我们实际开发一个大工程时可能有用
```

## 其他常见的文件类型

```cpp
1. lanuch文件
    
```

