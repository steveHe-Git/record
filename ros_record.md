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
    launch文件一般以.launch或.xml结尾,它对ROS需要运行程序进行了打包,通过一句命令来启动。一般launch文件中会指定要启动哪些package下的哪些可执行程序,指定以什么参数启动,以及一些管理控制的命令。	launch文件通常放在软件包的 	launch/	 路径中中。	launch文件的具体写法见3.2节。
    
2. msg/srv/action文件
    ROS程序中有可能有一些自定义的消息/服务/动作文件,为程序的发者所设计的数据结构,这类的文件以 .msg,.srv,.action结尾,通常放在package的msg/, srv/	 ,action/ 路径下。msg文件写法见3.4节,srv文件写法见3.6节。
    
3. urdf/xacro文件
    urdf/xacro文件是机器人模型的描述文件,以.urdf或.xacro结尾。它定义了机器人的连杆和关节的信息,以及它们之间的位置、角度等信息,通过urdf文件可以将机器人的物理连接信息表示出来。并在可视化调试和仿真中显示。urdf文件的写法见第七章。
    
4. yaml文件
    yaml文件一般存储了ROS需要加载的参数信息,一些属性的配置。通常在launch文件或程序中读取.yaml文件,把参数加载到参数服务器上。通常我们会把yaml文件存放在 param/路径下
    
5. dae/stl文件
    dae或stl文件是3D模型文件,机器人的urdf或仿真环境通常会引用这类文件,它们描述了机器人的三维模型。相比urdf文件简单定义的性状,dae/stl文件可以定义复杂的模型,可以直接从solidworks或其他建模软件导出机器人装配模型,从而显示出更加精确的外形。
    
6. rviz文件
    rviz文件本质上是固定格式的文本文件,其中存储了RViz窗口的配置(显示哪些控件、视角、参数)。通常rviz文件不需要我们去手动修改,而是直接在RViz工具里保存,下次运行时直接读取。
```

#  ROS通信架构

## master 节点

```cpp
当我们要启动ROS时,首先输入命令:　roscore
    - 此时ROS	master启动,同时启动的还有 rosout	和 parameter	server,其中rosout	是负责日志输出的一个节点,其作用是告知用户当前系统的状态,包括输出系统的error、warning等等,并且将log记录于日志文件中, parameter server即是参数服务器,它并不是一个node,而是存储参数配置的一个服务器;
	- master在整个网络通信架构里相当于管理中心,管理着各个node。node首先在master处进行注册,之后master会将该node纳入整个ROS程序中。node之间的通信也是先由master进行“牵线”,才能两两的进行点对点通信;
	- master之后,节点管理器就开始按照系统的安排协调进行启动具体的节点。节点就是一个进程,我们知道一个package中存放着可执行文件,可执行文件是静态的,当系统执行这些可执行文件,将这些文件加载到内存中,它就成为了动态的node。具体启动node的语句是:
	rosrun pkg_name node_name
	- 通常我们运行ROS,就是按照这样的顺序启动,有时候节点太多,我们会选择用launch文件来启动;

## rosrun和rosnode命令
    rosrun命令的详细用法如下:
        rosrun	[--prefix	cmd]	[--debug]	pkg_name	node_name	[ARGS]  
        rosrun将会寻找PACKAGE下的名为EXECUTABLE的可执行程序,将可选参数ARGS传入。
        例如在GDB下运行ros程序:
        rosrun	--prefix　'gdb　-ex　run　--args'　pkg_name　node_name
        
    rosnode命令的详细作用列表如下:
        rosnode命令 　　　　　　作用
        rosnode	list	 　　列出当前运行的node信息
        rosnode	info	　　　node_name	 显示出node的详细信息
        rosnode	kill	　　　node_name	 结束某个node
        rosnode	ping	 　　测试连接节点
        rosnode	machine	 　　列出在特定机器或列表机器上运行的节点
        rosnode	cleanup	 　　清除不可到达节点的注册信息
```

## launch文件

```cpp
ROS为我们提供了一个命令能一次性启动master和多个node。该命令是:
	roslaunch pkg_name file_name.launch
    
	roslaunch命令首先会自动进行检测系统的roscore有没有运行,也即是确认节点管理器是否在运行状态中,如果master没有启动,那么roslaunch就会首先启动master,然后再按照launch的规则执行。launch文件里已经配置好了启动的规则。	所以 	roslaunch	 就像是一个启动工具,能够一次性把多个节点按照我们预先的配置启动起来,减少我们在终端中一条条输入指令的麻烦；
        
** 写法与格式 **
launch文件同样也遵循着xml格式规范,是一种标签文本,它的格式包括以下标签:
<launch>				<!--根标签-->
<node>				    <!--需要启动的node及其参数-->
<include>				<!--包含其他launch-->
<machine>				<!--指定运行的机器-->
<env-loader>			<!--设置环境变量-->
<param>				    <!--定义参数到参数服务器-->
<rosparam>				<!--启动yaml文件参数到参数服务器-->
<arg>					<!--定义变量-->
<remap>					<!--设定参数映射-->
<group>					<!--设定命名空间-->
</launch>				<!--根标签-->
    
** example1 **
<launch>
	<node name="talker"	pkg="rospy_tutorials" type="talker"/>
</launch>
文本中的信息是,它启动了一个单独的节点 talker, 该节点是包 rospy_tutorials软件包中的节点;

** example2 **
<launch>
    <!--arg是launch标签中的变量声明,arg的name为变量名,default或者value为值-->
    <arg	name="robot"	default="xbot2"/>
    <arg	name="debug"	default="false"/>
    <arg	name="gui"	default="true"/>
    <arg	name="headless"	default="false"/>

    <!--Start Gazebo with a blank world	-->
    <include	file="$(find	gazebo_ros)/launch/empty_world.launch">	<!--include用来嵌套仿真场景的launch文件-->
    <arg	name="world_name"	value="$(find	robot_sim_demo)/worlds/ROS-Academy.world"/>
    <arg	name="debug"	value="$(arg	debug)"	/>
    <arg	name="gui"	value="$(arg	gui)"	/>
    <arg	name="paused"	value="false"/>
    <arg	name="use_sim_time"	value="true"/>
    <arg	name="headless"	value="$(arg	headless)"/>
    </include>

    <!--	Oh,	you	wanted	a	robot?	-->	<!--嵌套了机器人的launch文件-->
    <include	file="$(find	robot_sim_demo)/launch/include/$(arg	robot).launch.xml"	/>

    <!--如果你想连同RViz一起启动,可以按照以下方式加入RViz这个node-->
    <!--node	name="rviz"	pkg="rviz"	type="rviz"	args="-d	$(find	robot_sim_demo)/urdf_gazebo.rviz"	/-->
</launch>
```

## ROS topic

```cpp
ROS的通信方式有以下四种:
	- Topic主题
    - Service服务
    - Parameter Service参数服务器
    - Actionlib动作库

	ROS中的通信方式中,topic是常用的一种。对于实时性、周期性的消息,使用topic来传输是最佳的选择。topic是一种点对点的单向通信方式,这里的“点”指的是node,也就是说node之间可以通过topic方式来传递信息；
        
    那么怎么样来理解“异步”这个概念呢?在node1每发布一次消息之后,就会继续执行下一个动作,至于消息是什么状态、被怎样处理,它不需要了解;而对于node2图像处理程序,它只管接收和处理 	/camera_rgb	 上的消息,至于是谁发来的,它不会关心。所以node1、node2两者都是各司其责,不存在协同工作,我们称这样的通信方式是异步的。
        
** 总结 **
	1.	 topic通信方式是异步的,发送时调用publish()方法,发送完成立即返回,不用等待反馈。
    2.	 subscriber通过回调函数的方式来处理消息。
    3.	 topic可以同时有多个subscribers,也可以同时有多个publishers。
        
** 操作指令 **
        命令                              作用
	rostopic list	              	列出当前所有的topic
	rostopic info topic_name	 	显示某个topic的属性信息
	rostopic echo topic_name	 	显示某个topic的内容
	rostopic pub topic_name	...	 	向某个topic发布内容
	rostopic bw	topic_name	      	查看某个topic的带宽
	rostopic hz	topic_name	      	查看某个topic的频率
	rostopic find topic_type	 	查找某个类型的topic
	rostopic type topic_name	 	查看某个topic的类型(msg)
        
** 小结 **
    topic的通信方式是ROS中比较常见的单向异步通信方式,它在很多时候的通信是比较易用且高效的。但是有些需要交互的通信时该方式就显露出自己的不足之处了,后续我们会介绍双向同步的通信方式service。    
```

## Ros Message

```cpp
topic有很严格的格式要求,比如上节的摄像头进程中的rgb图像topic,它就必然要遵循ROS中定义好的rgb图像格式。这种数据格式就是Message
    
** 结构与类型 **
	基本的msg包括bool、int8、int16、int32、int64(以及uint)、float、float64、string、time、duration、header、可变长数组array[]、固定长度数组array[C]。那么具体的一个msg是怎么组成的呢?我们用一个具体的msg来了解,例如上例中的msg sensor_msg/image,位置存放在 sensor_msgs/msg/image.msg里,它的结构如下:
    std_msg/Header	   header
    uint32				seq
    time				stamp
    string				frame_id
    uint32				height
    uint32				width
    string				encoding
    uint8				is_bigendian
    uint32				step
    uint8[]				data
    可以理解msg是一个“类”,那么我们每次发布的内容可以理解为“对象”， 通常说Message既指的是类,也是指它的对象。而msg文件则相当于类的定义了。
    
** 操作指令 **
	rosmsg命令        		作用
	rosmsg	list	         列出系统上所有的msg
	rosmsg	show msg_name	 显示某个msg的内容
       
** 常见的message **
    本小节主要介绍常见的message类型,包括std_msgs,	sensor_msgs,nav_msgs,geometry_msgs等;
	1. Vector3.msg
        #文件位置:geometry_msgs/Vector3.msg
        float64	x
        float64	y
        float64	z
        
    2. Accel.msg
        #定义加速度项,包括线性加速度和角加速度
        #文件位置:geometry_msgs/Accel.msg
        Vector3	linear
        Vector3	angular
        
    3. Header.msg
        #定义数据的参考时间和参考坐标
        #文件位置:std_msgs/Header.msg
        uint32	seq							#数据ID
        time	stamp						#数据时间戳
        string	frame_id					#数据的参考坐标系
        
    4. Echos.msg
        #定义超声传感器
        #文件位置:自定义msg文件
        Header	header
        uint16	front_left
        uint16	front_center
        uint16	front_right
        uint16	rear_left
        uint16	rear_center
        uint16	rear_right
        
    5. Quaternion.msg
        #消息代表空间中旋转的四元数
        #文件位置:geometry_msgs/Quaternion.msg
        float64	x
        float64	y
        float64	z
        float64	w
        
    6. Imu.msg
        #消息包含了从惯性原件中得到的数据,加速度为m/^2,角速度为rad/s
        #如果所有的测量协方差已知,则需要全部填充进来如果只知道方差,则
        #只填充协方差矩阵的对角数据即可
        #位置:sensor_msgs/Imu.msg
        Header	header
        Quaternion	orientation
        float64[9]	orientation_covariance
        Vector3	angular_velocity
        float64[9]	angular_velocity_covariance
        Vector3	linear_acceleration
        float64[]	linear_acceleration_covariance
        
    7. LaserScan.msg
        #平面内的激光测距扫描数据,注意此消息类型仅仅适配激光测距设备
        #如果有其他类型的测距设备(如声呐),需要另外创建不同类型的消息
        #位置:sensor_msgs/LaserScan.msg
        Header	header									#时间戳为接收到第一束激光的时间
        float32	angle_min								#扫描开始时的角度(单位为rad)
        float32	angle_max								#扫描结束时的角度(单位为rad)
        float32	angle_increment							#两次测量之间的角度增量(单位为rad)
        float32	time_increment							#两次测量之间的时间增量(单位为s)
        float32	scan_time								#两次扫描之间的时间间隔(单位为s)
        float32	range_min								#距离最小值(m)
        float32	range_max								#距离最大值(m)
        float32[]	ranges								#测距数据(m,如果数据不在最小数据和最大数据之间,则抛弃)
        float32[]	intensities							#强度,具体单位由测量设备确定,如果仪器没有强度测量,则数组为空即可
        
	8. Point.msg
        #空间中的点的位置
        #文件位置:geometry_msgs/Point.msg
        float64	x
        float64	y
        float64	z
        
    9. Pose.msg
        #消息定义自由空间中的位姿信息,包括位置和指向信息
        #文件位置:geometry_msgs/Pose.msg
        Point	position
        Quaternion	orientation
        
    10. PoseStamped.msg
        #定义有时空基准的位姿
        #文件位置:geometry_msgs/PoseStamped.msg
        Header	header
        Pose	pose
        
    11. PoseWithCovariance.msg
        #表示空间中含有不确定性的位姿信息
        #文件位置:geometry_msgs/PoseWithCovariance.msg
        Pose	pose
        float64[36]	covariance
        
    12. Power.msg
        #表示电源状态,是否开启
        #文件位置:自定义msg文件
        Header	header
        bool	power
        ######################
        bool	ON		=	1
        bool	OFF	=	0
        
    13. Twist.msg
        #定义空间中物体运动的线速度和角速度
        #文件位置:geometry_msgs/Twist.msg
        Vector3	linear
        Vector3	angular
        
    14. TwistWithCovariance.msg
        #消息定义了包含不确定性的速度量,协方差矩阵按行分别表示:
        #沿x方向速度的不确定性,沿y方向速度的不确定性,沿z方向速度的不确定性
        #绕x转动角速度的不确定性,绕y轴转动的角速度的不确定性,绕z轴转动的
        #角速度的不确定性
        #文件位置:geometry_msgs/TwistWithCovariance.msg
        Twist	twist
        float64[36]	covariance		#分别表示[x;	y;	z;	Rx;	Ry;	Rz]
        
    15. Odometry.msg
        #消息描述了自由空间中位置和速度的估计值
        #文件位置:nav_msgs/Odometry.msg
        Header	header
        string	child_frame_id
        PoseWithCovariance	pose
        TwistWithCovariance	twist
```

## Ros service

```cpp
	我们知道topic是ROS中的一种单向的异步通信方式。然而有些时候单向的通信满足不了通信要求,比如当一些节点只是临时而非周期性的需要某些数据,如果用topic通信方式时就会消耗大量不必要的系统资源,造成系统的低效率高功耗。这种情况下,就需要有另外一种请求-查询式的通信模型。这节我们来介绍ROS通信中的另一种通信方式——service(服务)
        
** 工作原理 **
    Service通信是双向的,它不仅可以发送消息,同时还会有反馈。所以service包括两部分,一部分是请求方(Clinet),另一部分是应答方/服务提供方(Server)。这时请求方(Client)就会发送一个request,要等待server处理,反馈回一个reply,这样通过类似“请求-应答”的机制完成整个服务通信;
	Node A --(request)-->   service  -----> Node B
   (client)<--(reply)----  	         <----- (server)
        
** 工作过程 **
    Service是同步通信方式,所谓同步就是说,此时Node	A发布请求后会在原地等待reply,直到Node B处理完了请求并且完成了reply,Node	A才会继续执行。Node A等待过程中,是处于阻塞状态的成通信。这样的通信模型没有频繁的消息传递,没有冲突与高系统资源的占用,只有接受请求才执行服务,简单而且高效。
        
** topic vs service **
    名称     Topic                        	Service
    通信方式  异步通信							同步通信
    实现原理  TCP/IP 					       TCP/IP
    通信模型  Publish-Subscribe 			   Request-Reply
    映射关系  Publish-Subscribe(多对多)      	 Request-Reply(多对一)
    特点     接受者收到数据会回调(Callback)      远程过程调用(RPC)服务器端的服务
    应用场景  连续、高频的数据发布            	偶尔使用的功能/具体的任务
    举例     激光雷达、里程计发布数据           	开关传感器、拍照、逆解计算
        
** 操作指令 **
	rosservice	命令 作用
	rosservice	list	 显示服务列表
	rosservice	info	 打印服务信息
	rosservice	type	 打印服务类型
	rosservice	uri	 打印服务ROSRPC	uri
	rosservice	find	 按服务类型查找服务
	rosservice	call	 使用所提供的args调用服务
	rosservice	args	 打印服务参数
```

## Ros srv

```cpp
	类似msg文件,srv文件是用来描述服务(service数据类型的,service通信的数据格式定义在*.srv中。它声明了一个服务,包括请求(request)和响应(reply)两部分。其格式声明如下:
msgs_demo/srv/DetectHuman.srv
    #数据格式
    bool	start_detect
    ---
    my_pkg/HumanPose[]	pose_data
                         
msgs_demo/msg/HumanPose.msg
    std_msgs/Header	header
    string	uuid
    int32	number_of_joints
    my_pkg/JointPose[]joint_data 
                      
msgs_demo/msg/JointPose.msg
    string	joint_name
    geometry_msgs/Pose	pose
    floar32	confidence         
                         
	以 DetectHUman.srv 文件为例,该服务例子取自OpenNI的人体检测ROS软件包。它是用来查询当前深度摄像头中的人体姿态和关节数的。srv文件格式很固定,第一行是请求的格式,中间用---隔开,第三行是应答的格式。在本例中,请求为是否开始检测,应答为一个数组,数组的每个元素为某个人的姿态(HumanPose)。而对于人的姿态,其实是一个msg,所以srv可以嵌套msg在其中,但它不能嵌套srv。
                         
** 操作指令 **
	rossrv	命令      	作用
	rossrv	show	 	显示服务描述
	rossrv	list	 	列出所有服务
	rossrv	md5	 		显示服务md5sum
	rossrv	package	 	列出包中的服务
	rossrv	packages	列出包含服务的包       
                       
** 修改编译文件 **
	定义完了msg、srv文件,还有重要的一个步骤就是修改package.xml和修改CMakeList.txt。这些文件需要添加一些必要的依赖等；
    - package.xml修改:
        <build_depend>	message_generation	</build_depend>
        <run_depend> message_runtime </run_depend>	
    - CMakeList.txt修改：
        find_package(...roscpp rospy std_msgs **message_generation**)
        catkin_package(
            ...
            CATKIN_DEPENDS	**message_runtime**...
            ...)
        add_message_file(
        	FILES
            **DetectHuman.srv**
            **HumanPose.msg**
            **JointPos.msg**)
        **generate_messages(DEPENDENCIES std_msgs)**                         
```

## parameter server

```cpp
	参数服务器也可以说是特殊的“通信方式”。特殊点在于参数服务器是节点存储参数的地方、用于配置参数,全局共享参数。参数服务器使用互联网传输,在节点管理器中运行,实现整个通信过程。
        
** 维护方式 **
   参数服务器的维护方式非常的简单灵活,总的来讲有三种方式:
       - 命令行维护
       - launch文件内读写
       - node源码
           
** 命令行维护 **
 	使用命令行来维护参数服务器,主要使用 	rosparam语句来进行操作的各种命令,如下:
	rosparam命令             					作用
	rosparam set param_key	param_value	 	设置参数
	rosparam get param_key	 				显示参数
	rosparam load file_name	 				从文件加载参数
	rosparam dump file_name	 				保存参数到文件
	rosparam delete	 						删除参数
	rosparam list	 						列出参数名称
        
   	load&&dump文件
	load和dump文件需要遵守YAML格式,YAML格式具体示例如下: 
        name:'Zhangsan'
        age:20
        gender:'M'
        score{Chinese:80,Math:90}
        score_history:[85,82,88,90]
	其实就可以把YAML文件的内容理解为字典,因为它也是键值对的形式。
        
** launch文件读写 **
   	<node pkg="turtlesim" type="turtlesim_node" name="turtlesim_node">
        <param name="turtle_name1" value="Tom"/>
        <param name="turtle_name2" value="Jerry"/>
        <rosparam file="$(find learning_launch)/config/param.yaml" command="load"/>
    </node>
    param：设置一个参数（设置ros系统运行中的参数，存储在参数服务器中。把节点程序中的一个值重新给值）
    rosparam：加载参数文件设置多个参数;
    
    <node pkg="turtlesim" type="turtlesim_node" name="turtlesim_node">
        <remap from="turtle/cmd_vel" to="/cmd_vel"/>
    </node>
    remap：将from的名称重命名为to后面的

** node源码 **
除了上述最常用的两种读写参数服务器的方法,还有一种就是修改ROS的源码,也就是利用API来对参数服务器进行操作。
        
** 参数类型 **
ROS参数服务器为参数值使用XMLRPC数据类型,其中包括:strings,	integers,	floats,
booleans,	lists,	dictionaries,	iso8601	dates,	and	base64-encoded	data。
        
**总结 参数说明**
    <node >要启动的node参数
        pkg=''mypackage''
        type=''nodetype''
        name=''nodename''
        arg=''arg1 ....''(可选)
        respawn=''ture''(可选)如果节点停止，自动重启节点
        ns=''foo''（可选）在foo命名空间启动节点
        output=''log|screen''(可选)
    <rosparam>操作yaml文件参数
        command=''load|dump|delete''(默认load)
        file=''$(find pkg-name)/path/foo.yaml''(load或dump命令)yaml文件的名字
        param=''param-name''参数名
    <param>定义一个设置在参数服务器的参数，它可以添加到<node>中
        name=''namespace/name''
        value=''value''（可选）如果省略这个参数，则应指定一个文件(binfile/textfile)或命令
        type=''str|int|double|boot''(可选)指定参数的类型
        textfile=''$(find pkg-name)/path/file''(可选)   

        binfile=''$(find pkg-name)/path/file''()
        command=''(find pkg-name)/exe '$(find pkg-name)/arg.txt' ''(可选)exe是可执行文件（cpp、py），arg.txt是参        数文件
    <include>在当前launch文件中调用另一个launch文件
        file=''$(find pkg-name)/path/launch-file.launch''    
    <env>设置节点的环境变量
        name=''environment-variable-name''
        value=''environment-variable-value''    
    <remap>将一个参数名映射为另一个名字
        from=''original-name''
        to=''new-name''    
    <arg>定义一个局部参数，该参数只能在一个launch文件中使用
        <arg name=''foo''/>声明一个参数foo，后面需要给它赋值
        <arg name=''foo'' default=''1''/>声明一个参数foo，如不赋值取默认值
        <arg name=''foo'' value=''bar''/>声明一常量foo，它的值不能修改
```

## Ros action

```cpp
	Actionlib是ROS中一个很重要的库,类似service通信机制,actionlib也是一种请求响应机制的通信方式,actionlib主要弥补了service通信的一个不足,就是当机器人执行一个长时间的任务时,假如利用service通信方式,那么publisher会很长时间接受不到反馈的reply,致使通信受阻。当service通信不能很好的完成任务候,actionlib则可以比较适合实现长时间的通信过程,actionlib通信过程可以随时被查看过程进度,也可以终止请求,这样的一个特性,使得它在一些特别的机制中拥有很高的效率。
        
    通信双方在ROS Action	Protocal下进行交流通信是通过接口来实现；
    我们可以看到,客户端会向服务器发送目标指令和取消动作指令,而服务器则可以给客户端发送实时的状态信息,结果信息,反馈信息等等,从而完成了service没法做到的部分. 
        
** action规范 **
    利用动作库进行请求响应,动作的内容格式应包含三个部分,目标、反馈、结果。
        目标
            机器人执行一个动作,应该有明确的移动目标信息,包括一些参数的设定,方向、角度、速度等等。从而使机器人完成动作任务。
        反馈
            在动作进行的过程中,应该有实时的状态信息反馈给服务器的实施者,告诉实施者动作完成的状态,可以使实施者作出准确的判断去修正命令。
        结果
            当运动完成时,动作服务器把本次运动的结果数据发送给客户端,使客户端得到本次动作的全部信息,例如可能包含机器人的运动时长,最终姿势等等。
        
** Action规范文件格式 **
   Action规范文件的后缀名是.action,它的内容格式如下:
		# Define the goal
        uint32	dishwasher_id		#Specify which dishwasher we want to use
        ---
        # Define the result
        uint32	total_dishes_cleaned
        ---
        # Define a feedback message
        float32	percent_complete
       
** Action实例 **
   	Actionlib是一个用来实现action的一个功能包集。我们在demo中设置一个场景,执行一个搬运的action,搬运过程中客户端会不断的发回反馈信息,最终完成整个搬运过程；
	首先写handling.action文件,类比如上的格式.包括三个部分,目标,结果,反馈.如下: 
        #	Define	the	goal
        uint32	handling_id	
        ---
        #	Define	the	result
        uint32	Handling_completed
        ---
        #	Define	a	feedback	message
        float32	percent_complete
            
    写完之后修改文件夹里CmakeLists.txt如下内容:
        1.	 find_package(catkin	REQUIRED	genmsg	actionlib_msgs	actionlib)
        2.	 add_action_files(DIRECTORY	action	FILES	DoDishes.action)
                generate_messages(DEPENDENCIES	actionlib_msgs)
        3.	 add_action_files(DIRECTORY	action	FILES	Handling.action)
        4.	 generate_messages(	DEPENDENCIES	actionlib_msgs)	
            
    修改package.xml,添加所需要的依赖如下:
        1.	 <build_depend>actionlib	</build_depend>	
        2.	 <build_depend>actionlib_msgs</build_depend>	
        3.	 <run_depend>actionlib</run_depend>	
        4.	 <run_depend>actionlib_msgs</run_depend>	
        然后回到工作空间catkin_ws进行编译.
```

## Ros 常见的通信srv,action类型

```cpp
常见的srv类型及其定义srv类型相当于两个message通道,一个发送,一个接收;
** 常见的srv类型 **
1. AddTwoInts.srv
    #对两个整数求和,虚线前是输入量,后是返回量
    #文件位置:自定义srv文件
    int32	a
    int32	b
    ---
    int32	sum
    
2. Empty.srv
    #文件位置:std_srvs/Empty.srv
    #代表一个空的srv类型
    ---
    
3. GetMap.srv
    #文件位置:nav_msgs/GetMap.srv
    #获取地图,注意请求部分为空
    
    ---
    nav_msgs/OccupancyGrid	map
    
4. GetPlan.srv
    #文件位置:nav_msgs/GetPlan.srv
    #得到一条从当前位置到目标点的路径
    geometry_msgs/PoseStamped	start								#起始点
    geometry_msgs/PoseStamped	goal								#目标点
    float32	tolerance												#到达目标点的x,y方向的容错距离
    ---
    nav_msgs/Path	plan
    
5. SetBool.srv
    #文件位置:std_srvs/SetBools.srv
    bool	data		#	启动或者关闭硬件
    ---
    bool	success		#	标示硬件是否成功运行
    string	message		#	运行信息
    
6. SetCameraInfo.srv
    #文件位置:sensor_msgs/SetCameraInfo.srv
    #通过给定的CameraInfo相机信息,来对相机进行标定
    sensor_msgs/CameraInfo	camera_info								#相机信息
    ---
    bool	success													#如果调用成功,则返回true
    string	status_message											#给出调用成功的细节
    
7. SetMap.srv
    #文件位置:nav_msgs/SetMap.srv
    #以初始位置为基准,设定新的地图
    nav_msgs/OccupancyGrid	map
    geometry_msgs/PoseWithCovarianceStamped	initial_pose
    ---
    bool	success
    
8. TalkerListener.srv
    #文件位置:	自定义srv文件
    ---
    bool	success			#	标示srv是否成功运行
    string	message	#	信息,如错误信息等
    
9. Trigger.srv
    #文件位置:std_srvs/Trigger.srv
    ---
    bool	success			#	标示srv是否成功运行
    string	message	#	信息,如错误信息等
    
** 常见action类型 **
1. AddTwoInts.action
    #文件位置:自定义action文件
    #表示将两个整数求和
    int64	a
    int64	b
    ---
    int64	sum
    ---
    
2. AutoDocking.action
    #文件位置:自定义action文件
    #goal
    ---
    #result
    string	text
    ---
    #feedback
    string	state
    string	text
    
3. GetMap.action
    #文件位置:nav_msgs/GetMap.action
    #获取地图信息,响应部分为空
    ---
    nav_msgs/OccupancyGrid	map
    ---
    #无返回部分
    
4. MoveBase.action
    #文件位置:geometry_msgs/MoveBase.action
    geometry_msgs/PoseStamped	target_pose
    ---
    ---
    geometry_msgs/PoseStamped	base_position	
```

# Ros 常用工具

```cpp
	ROS中的工具就是帮助我们完成一系列的操作,使得我们的工作更加轻松高效。ROS工具的功能大概有以下几个方向:仿真、调试、可视化。本节课我们要学习的Gazebo就是实现了仿真的功能,而调试与可视化由Rviz、rqt来实现
```

## Gazebo

```cpp
无论是在topic通信还是在service通信中,我们的demo都是在Gazebo中实现。
    
    Gazebo是一个机器人仿真工具,模拟器,也是一个独立的开源机器人仿真平台。当今市面上还有其他的仿真工具例如V—Rep、Webots等等。但是Gazebo不仅开源,也是是兼容ROS最好的仿真工具。
    
    Gazebo的功能很强大,最大的优点是对ROS的支持很好,因为Gazebo和ROS都由OSRF(Open Source	Robotics Foundation)开源机器人组织来维护,Gazebo支持很多开源的物理引擎比如最典型的ODE。可以进行机器人的运动学、动力学仿真,能够模拟机器人常用的传感器(如激光雷达、摄像头、IMU等),也可以加载自定义的环境和场景。
    
    仿真不仅仅只是做出一个很酷的3D场景,更重要的是给机器人一个逼近现实的虚拟物理环境,比如光照条件、物理距离等等。设定好具体的参数,让机器人完成我们设定的目标任务。比如一些有危险因素的测试,就可以让机器人在仿真的环境中去完成,例如无人车在交通环境复杂的交通要道的效果,我们就可以在仿真的环境下测试各种情况无人车的反应与效果,如车辆的性能、驾驶的策略、车流人流的行为模式等,又或者各种不可控因素如雨雪天气,突发事故,车辆故障等,从而收集结果参数指标信息等等,只有更大程度的逼近现实,才能得出车辆的真实效果。直到无人车在仿真条件下做到万无一失,才能放心的投放到真实环境中去使用,这即避免了危险因素对实验者的威胁,也节约了时间和资源,这就是仿真的意义。
    
    通常一些不依赖于具体硬件的算法和场景都可以在Gazebo上仿真,例如图像识别、传感器数据融合处理、路径规划、SLAM等任务完全可以在Gazebo上仿真实现,大大减轻了对硬件的依赖。
    
** 实例演示 **
  	和我们前面的实例测试一样,我们打开教材的模拟场景,输入 roslaunch robot_sim_demo robot_spawn_launch	
   
** 操作说明 **
    - 平移:鼠标左键
	- 旋转:鼠标滚轮中键
	- 放缩:鼠标滚轮
	- 界面左侧是控制面板
	- 导入模型就在控制面板的insert,可以直接拖入模拟空间,也可以按需自制模型拖入。
```

## Rviz

```cpp
	本节课介绍的是我们在ROS开发中非常常用的一个工具,基本上的调试和开发都离不开这个工具——RViz(the	Robit	Visualization	tool)机器人可视化工具,可视化的作用是直观的,它极大的方便了监控和调试等操作。
        
** 实例演示 **
  	和我们前面的实例测试一样,我们打开教材的模拟场景,输入 roslaunch robot_sim_demo robot_spawn_launch	,之后在命令行打开新的终端直接输入rviz	打开工具,和Gazebo一样,也会显示出一个3D环境,不过操作上有所不同;
   
** 操作说明 **
   - 平移:鼠标滚轮中键
   - 旋转:鼠标左键
   - 放缩:鼠标滚轮
   - 左侧控制面板,可以添加插件
       RViz的插件种类繁多功能强大,非常适合我们开发调试ROS程序。
       
** rviz vs gazebo **
       虽然从界面上来看,RViz和Gazebo非常相似,但实际上两者有着很大的不同,Gazebo实现的是仿真,提供一个虚拟的世界,RViz实现的是可视化,呈现接收到的信息。左侧的插件相当于是一个个的subscriber,RViz接收信息,并且显示。所以RViz和Gazebo有本质的差异。
```

## Rqt

```cpp
rqt是一个基于qt开发的可视化工具,拥有扩展性好、灵活易用、跨平台等特点,主要作用和RViz一致都是可视化,但是和RViz相比,rqt要高级一个层次。
    
**rqt命令**
    - rqt_graph	:显示通信架构
	- rqt_plot	:绘制曲线
	- rqt_console	:查看日志
        
** rqt_graph **
   rqt_graph是来显示通信架构,也就是我们上一章所讲的内容节点、主题等等,当前有哪些Node和topic在运行,消息的流向是怎样,都能通过这个语句显示出来。此命令由于能显示系统的全貌,所以非常的常用;
	注意在椭圆形的代表节点,矩形代表topic。

** rqt_plot **
    rqt_plot将一些参数,尤其是动态参数以曲线的形式绘制出来。当我们在开发时查看机器人的原始数据,我们就能利用rqt_plot将这些原始数据用曲线绘制出来,非常的直观,利于我们分析数据。
    
** rqt_console **
    rqt_console里存在一些过滤器,我们可以利用它方便的查到我们需要的日志。
    输入命令语句 	rqt_console	 ,显示日志的输出,配合 	rqt_logger_level	 查看日志的级别。
```

## Rosbag

```cpp
	rosbag是一个这是一套用于记录和回放ROS主题的工具;它旨在提高性能,并避免消息的反序列化和重新排序。rosbag	package提供了命令行工具和代码API,可以用C++或者python来编写包。
        
** 命令 **
    命令				作用
	cheak	 		确定一个包是否可以在当前系统中进行,或者是否可以迁移。
	decompress		压缩一个或多个包文件。
	filter	 		解压一个或多个包文件。
	fix	 			在包文件中修复消息,以便在当前系统中播放。
	help	 		获取相关命令指示帮助信息
	info	 		总结一个或多个包文件的内容。
	play	 		以一种时间同步的方式回放一个或多个包文件的内容。
	record	 		用指定主题的内容记录一个包文件。
	reindex	 		重新索引一个或多个包文件。
```

Rosbridge

```cpp
	Rosbridge是一个用在ROS系统和其他系统之间的一个功能包,就像是它的名字一样,起到一个"桥梁"的作用,使得ros系统和其他系统能够进行交互.Rosbridge为非ROS程序提供了一个JSON	API,有许多与Rosbridge进行交互的前端,包括一个用于Web浏览器交互的WebSocket服务器。Rosbridge_suite是一个包含Rosbridge的元程序包,用于Rosbridge的各种前端程序包(如WebSocket程序包)和帮助程序包。
        
** 协议和实现 **
   Rosbridge主要包含两部分内容:协议(Potocol)和实现(Implementation)
       - 协议
           Rosbridge Protocol提供了非ROS程序与ROS通信的具体的格式规范,规范基于JSON格式,包括订阅topic,发布message,调用server,设置参数,压缩消息等等.例如订阅topic的格式，规范如下:
            {	"op":	"subscribe",
                    "topic":	"/cmd_vel",
                    "type":	"geometry_msgs/Twist"
            }
            此规范与所用的编程语言和传输方式无关,任何可以发送JSON格式的语音和传输方式都可以Rosbridge	protocol进行交流,并且与ROS进行交互.
       - 实现
            Rosbridge_suite元程序包是实现Rosbridge Protocol并提供WebSocket传输层的包的集合。
            这些软件包包括:
			1. Rosbridge_library	:	核心rosbridge软件包。Rosbridge_library负责获取JSON字符串并将命令发送到ROS,反过来接收处理ROS发过来的信息,将之转换为JSON字符串,并将结果转交给非ROS程序。
			2. rosapi	:	通过服务调用来访问某些ROS操作,这些服务通常为ROS客户端库保留的服务.这些操作包括获取和设置参数,获取主题列表等等。
			3. rosbridge_server	:	虽然Rosbridge_library提供JSON到ROS转换,但它将传输层留给其他人。Rosbridge_server提供了一个WebSocket连接,所以浏览器可以与ROS“交谈”。Roslibjs是一个浏览器的JavaScript库,可以通过rosbridge_server与ROS进行交流。
```

## moveit!

```cpp
	它融合了研究者在运动规划、操纵、3D感知、运动学、控制和导航方面的最新进展,为操作者提供了一个易于使用的平台,使用它可以开发先进的机器人应用,也被广泛应用于工业,商业,研发和其他领域。由于以上特性,moveit!一跃成为在机器人上最广泛使用的开源操作软件
        
        moveit!的使用通过为用户提供接口来调用它,包括C++、Python、GUI三种接口。ROS中的 	move_group	 节点充当整合器,整合多个独立组件,提供ROS风格的Action和service。 	move_group	 通过ROS	topic和action与机器人通讯,获取机器人的位置、节点等状
态,获取数据再传递给机器人的控制器。
	move_group	 节点获取到节点状态信息或者机器人变换信息时候,会通过控制器的接口去处理这些信息,比如进行坐标转换、规划场景、3D感知。另外, 	move_group	 的结构比较容易扩展,不仅具有独立的能力如抓放,运动规划,也可扩展自公共类,但实际作为独立的插件运行。
```

# roscpp编程

```
	ROS为机器人开发者们提供了不同语言的编程接口,比如C++接口叫做roscpp,Python接口叫做rospy,Java接口叫做rosjava。尽管语言不通,但这些接口都可以用来创建topic、service、param,实现ROS的通信功能。Clinet	Lirary有点类似开发中的Helper	Class,把一些常用的基本功能做了封装。
目前ROS支持的Clinet	Library包括:
Client 			Library 介绍
roscpp 			ROS的C++库,是目前最广泛应用的ROS客户端库,执行效率高
rospy 			ROS的Python库,开发效率高,通常用在对运行时间没有太大要求的场合,例如配置、初始化等操作
roslisp 		ROS的LISP库
roscs 			Mono/.NET.库,可用任何Mono/.NET语言,包括C#,Iron	Python,	IronRuby等
rosgo 			ROS Go语言库
rosjava 		ROS	Java语言库
rosnodejs 		Javascript客户端库

	roscpp位于 	/opt/ros/kinetic	 之下,用C++实现了ROS通信。在ROS中,C++的代码是通过catkin这个编译系统(扩展的CMake)来进行编译构建的。所以简单地理解,你也可以把roscpp就当作为一个C++的库,我们创建一个CMake工程,在其中include了roscpp等ROS的libraries,这样就可以在工程中使用ROS提供的函数了。
通常我们要调用ROS的C++接口,首先就需要 	#include	<ros/ros.h>	 。

roscpp的主要部分包括:
ros::init()	:		解析传入的ROS参数,创建node第一步需要用到的函数
ros::NodeHandle	:	和topic、service、param等交互的公共接口
ros::master	:		包含从master查询信息的函数
ros::this_node:		包含查询这个进程(node)的函数
ros::service:		包含查询服务的函数
ros::param:			包含查询参数服务器的函数,而不需要用到NodeHandle
ros::names:			包含处理ROS图资源名称的函数
具体可见:http://docs.ros.org/api/roscpp/html/index.html

以上功能可以分为以下几类:
Initialization	and	Shutdown	初始与关闭
Topics							话题
Services						服务
Parameter	Server				参数服务器
Timers							定时器
NodeHandles						节点句柄
Callbacks	and	Spinning		回调和自旋(或者翻译叫轮询?)
Logging							日志
Names and Node Information		名称管理
Time							时钟
Exception						异常

** 节点初始、关闭以及NodeHandle **
	当执行一个ROS程序,就被加载到了内存中,就成为了一个进程,在ROS里叫做节点。每一个ROS的节点尽管功能不同,但都有必不可少的一些步骤,比如初始化、销毁,需要通行的场景通常都还需要节点的句柄;
	
** 节点初始化 **
	对于一个C++写的ROS程序,之所以它区别于普通C++程序,是因为代码中做了两层工作:
	1.	 调用了 	ros::init()	 函数,从而初始化节点的名称和其他信息,一般我们ROS程序一开始都会以这种方式开始。
	2.	 创建 	ros::NodeHandle	 对象,也就是节点的句柄,它可以用来创建Publisher、Subscriber以及做其他事情。
	句柄(Handle)这个概念可以理解为一个“把手”,你握住了门把手,就可以很容易把整扇门拉开,而不必关心门是什么样子。NodeHandle就是对节点资源的描述,有了它你就可以操作这个节点了,比如为程序提供服务、监听某个topic上的消息、访问和修改param等等。

** 关闭节点**
通常我们要关闭一个节点可以直接在终端上按 	Ctrl	 + 	C	 ,系统会自动触发SIGINT句柄来关闭这个进程。	你也可以通过调用 	ros::shutdown()	 来手动关闭节点,但通常我们很少这样做。
以下是一个节点初始化、关闭的例子。
#include<ros/ros.h>
int	main(int	argc,	char**	argv)
{
    ros::init(argc,	argv,	"your_node_name");	
    ros::NodeHandle	nh;
    //....节点功能
    //....
    ros::spin();//用于触发topic、service的响应队列
}

** NodeHandle ** 
	NodeHandle是Node的句柄,用来对当前节点进行各种操作。在ROS中,NodeHandle是一个定义好的类,通过 	include<ros/ros.h>	 ,我们可以创建这个类,以及使用它的成员函数。
	NodeHandle常用成员函数包括:
        //创建话题的publisher	
        ros::Publisher	advertise(const	string	&topic,	uint32_t	queue_size,	bool	latch=false);	
        //第一个参数为发布话题的名称
        //第二个是消息队列的最大长度,如果发布的消息超过这个长度而没有被接收,那么就的消息就会出队。通常设为一
        个较小的数即可。
        //第三个参数是是否锁存。某些话题并不是会以某个频率发布,比如/map这个topic,只有在初次订阅或者地图更新
        这两种情况下,/map才会发布消息。这里就用到了锁存。
        //创建话题的subscriber
        ros::Subscriber	subscribe(const	string	&topic,	uint32_t	queue_size,	void(*)(M));
        //第一个参数是订阅话题的名称
        //第二个参数是订阅队列的长度,如果受到的消息都没来得及处理,那么新消息入队,就消息就会出队
        //第三个参数是回调函数指针,指向回调函数来处理接收到的消息
        //创建服务的server,提供服务
        ros::ServiceServer	advertiseService(const	string	&service,	bool(*srv_func)(Mreq	&,	Mre
        s	&));	
        //第一个参数是service名称
        //第二个参数是服务函数的指针,指向服务函数。指向的函数应该有两个参数,分别接受请求和响应。
        //创建服务的client
        ros::ServiceClient	serviceClient(const	string	&service_name,	bool	persistent=false);	
        //第一个函数式service名称
        //第二个参数用于设置服务的连接是否持续,如果为true,client将会保持与远程主机的连接,这样后续的请求会
        快一些。通常我们设为flase
        //查询某个参数的值
        bool	getParam(const	string	&key,	std::string	&s);	
        bool	getParam	(const	std::string	&key,	double	&d)	const;
        bool	getParam	(const	std::string	&key,	int	&i)	const;
        //从参数服务器上获取key对应的值,已重载了多个类型
        //给参数赋值
        void	setParam	(const	std::string	&key,	const	std::string	&s)	const;
        void	setParam	(const	std::string	&key,	const	char	*s)	const;
        void	setParam	(const	std::string	&key,	int	i)	const;
        //给key对应的val赋值,重载了多个类型的val	
```

## Topic in roscpp

```cpp
1. 创建gps消息
	topic_demo/msg/gps.msg	
        string	state					#工作状态
        float32	x						#x坐标
        float32	y						#y坐标
    
2. 当你创建完了msg文件,记得修改 CMakeLists.txt和 package.xml, 从而让系统能够编译自定义消息。在 CMakeLists.txt中需要改动
    find_package(catkin	REQUIRED	COMPONENTS
        roscpp
        std_msgs
        message_generation			#需要添加的地方
    )
    add_message_files(FILES	gps.msg)		
    #catkin在cmake之上新增的命令,指定从哪个消息文件生成
    generate_messages(DEPENDENCIES	std_msgs)	
    #catkin新增的命令,用于生成消息
    #DEPENDENCIES后面指定生成msg需要依赖其他什么消息,由于gps.msg用到了flaot32这种ROS标准消息,因此需要再把std_msgs作为依赖
    
3. package.xml中需要的改动
    <build_depend>message_generation</build_depend>
	<run_depend>message_runtime</run_depend>
    
4. 编译完成之后会在 devel路径下生成 gps.msg 对应的头文件
    要在代码中使用自定义消息类型,只要 	#include<topic_demo/gps.h>,然后声明,按照对结构体操作的方式修改内容即可。
    
5. talker.cpp
    #include	<ros/ros.h>			
    #include	<topic_demo/gps.h>		//自定义msg产生的头文件
    int	main(int	argc,	char	**argv)
    {
            ros::init(argc,	argv,	"talker");		//用于解析ROS参数,第三个参数为本节点名
            ros::NodeHandle	nh;				//实例化句柄,初始化node
            topic_demo::gps	msg;		//自定义gps消息并初始化	
                ...
            ros::Publisher	pub	=	nh.advertise<topic_demo::gps>("gps_info",	1);	//创建publisher,往"gps_info"话题上发布消息
            ros::Rate	loop_rate(1.0);			//定义发布的频率,1HZ	
            while	(ros::ok())			//循环发布msg
            {
                    ...			//处理msg
                    pub.publish(msg);//以1Hz的频率发布msg
                    loop_rate.sleep();//根据前面的定义的loop_rate,设置1s的暂停
            }
            return	0;
    }

6. listener.cpp
    #include	<ros/ros.h>
    #include	<topic_demo/gps.h>
    #include	<std_msgs/Float32.h>
    void	gpsCallback(const	topic_demo::gps::ConstPtr	&msg)
    {		
                    std_msgs::Float32	distance;		//计算离原点(0,0)的距离
                    distance.data	=	sqrt(pow(msg->x,2)+pow(msg->y,2));
                    ROS_INFO("Listener:	Distance	to	origin	=	%f,	state:	%s",distance.data,msg->state.c_str());	//输出
    }
    int	main(int	argc,	char	**argv)
    {
            ros::init(argc,	argv,	"listener");
            ros::NodeHandle	n;
            ros::Subscriber	sub	=	n.subscribe("gps_info",	1,	gpsCallback);		//设置回调函数gpsCallback
            ros::spin();	//ros::spin()用于调用所有可触发的回调函数,将进入循环,不会返回,类似于在循环里反复调用spinOnce()	
            //而ros::spinOnce()只会去触发一次
            return	0;
    }
注意：千万不要认为,只要指定了回调函数,系统就回去自动触发,你必须 ros::spin()或者ros::spinOnce()才能真正使回调函数生效。当有spin函数执
行,就会去处理消息队列中队首的消息
    
7. CMakeLists.txt文件修改
    在 CMakeLists.txt添加以下内容,生成可执行文件
    add_executable(talker	src/talker.cpp)	#生成可执行文件talker
    add_dependencies(talker	topic_demo_generate_messages_cpp)
    #表明在编译talker前,必须先生编译完成自定义消息
    #必须添加add_dependencies,否则找不到自定义的msg产生的头文件
    #表明在编译talker前,必须先生编译完成自定义消息
    target_link_libraries(talker	${catkin_LIBRARIES})	#链接
    add_executable(listener	src/listener.cpp	)	#声称可执行文件listener
    add_dependencies(listener	topic_demo_generate_messages_cpp)
    target_link_libraries(listener	${catkin_LIBRARIES})#链接
    
8. spin具体处理的方法又可分为阻塞/非阻塞,单线程/多线程,在ROS函数接口层面我们有4种spin的方式:
	spin方法 							   阻塞 线程
	ros::spin()	 						阻塞 单线程
	ros::spinOnce()	 					非阻塞 单线程
	ros::MultiThreadedSpin()	 		阻塞 多线程
	ros::AsyncMultiThreadedSpin()	 	非阻塞 多线程
        									
        									Topic1_cb
   	Topic1 --|								Topic2_cb
			 |-------Node----单线程spin--->  Topic2_cb  
    Topic2 --|								Topic1_cb
        									Topic2_cb
        
            										 	Topic2_cb
   	Topic1 --|							   Topic1_cb    Topic2_cb
			 |-------Node----多线程spin---> Topic1_cb    Topic2_cb  
    Topic2 --|							   Topic1_cb	Topic2_cb
        												Topic2_cb
    我们常用的 	spin()	 、 	spinOnce()	 是单个线程逐个处理回调队列里的数据。有些场合需要用
到多线程分别处理,则可以用到 	MultiThreadedSpin()	 、 	AsyncMultiThreadedSpin()
```

## service in roscpp

```cpp
Service是一种请求-反馈的通信机制。请求的一方通常被称为客户端,提供服务的一方叫做服务器端。Service机制相比于Topic的不同之处在于:
    1.	 消息的传输是双向的,有反馈的,而不是单一的流向。
    2.	 消息往往不会以固定频率传输,不连续,而是在需要时才会向服务器发起请求。
        
1. 创建 	service_demo/Greeting.srv	 文件,内容包括:
    string	name								#短横线上边部分是服务请求的数据
    int32	age										
    ---																#短横线下面是服务回传的内容。
    string	feedback
        
2. srv格式的文件创建后,也需要修改 	CMakeLissts.txt	 ,在其中加入
      add_service_files(FILES	Greeting.srv)
	其余与添加msg的改动一样。然后进行 	catkin_make	 ,系统就会生成在代码中可用的Greeting类型。在代码中使用,只需要 	#include	<service_demo/Greeting.h>	 ,然后即可创建该类型的srv。     
        
3. 创建提供服务节点(server);	service_demo/srv/server.cpp	 内容如下:
    #include	<ros/ros.h>
    #include	<service_demo/Greeting.h>
    bool	handle_function(service_demo::Greeting::Request	&req,	service_demo::Greeting::Response&res){
        //显示请求信息
        ROS_INFO(“Request	from	%s	with	age	%d”,	req.name.c_str(),	req.age);
        //处理请求,结果写入response
        res.feedback	=	“Hi	”	+	req.name	+	“.	I’m	server!”;
        //返回true,正确处理了请求
        return	true;
    }
    int	main(int	argc,	char**	argv){
        ros::init(argc,	argv,	“greetings_server”);		//解析参数,命名节点
        ros::NodeHandle	nh;									//创建句柄,实例化node
        ros::ServiceServer	service	=	nh.advertiseService(“greetings”,	handle_function);		//写明服务的处理函数
        ros::spin();
        return	0;
    }
	在以上代码中,服务的处理操作都写在 	handle_function()	 中,它的输入参数就是Greeting的Request和Response两部分,而非整个Greeting对象。通常在处理函数中,我们对Requst数据进行需要的操作,将结果写入到Response中。在roscpp中,处理函数返回值是bool型,也就是服务是否成功执行。不要理解成输入Request,返回Response,在rospy中是这样的。
        
4. 创建服务请求节点(client); service_demo/srv/client.cpp	 内容如下:
    #include	"ros/ros.h"
    #include	"service_demo/Greeting.h"
    int	main(int	argc,	char	**argv)
    {
        ros::init(argc,	argv,	"greetings_client");//	初始化,节点命名为"greetings_client"
        ros::NodeHandle	nh;
        ros::ServiceClient	client	=	nh.serviceClient<service_demo::Greeting>("greetings");
        //	定义service客户端,service名字为“greetings”,service类型为Service_demo
        //	实例化srv,设置其request消息的内容,这里request包含两个变量,name和age,见Greeting.srv
        service_demo::Greeting	srv;
        srv.request.name	=	"HAN";
        srv.request.age	=	20;
        if	(client.call(srv))
        {
            //	注意我们的response部分中的内容只包含一个变量response,另,注意将其转变成字符串
            ROS_INFO("Response	from	server:	%s",	srv.response.feedback.c_str());
        }
        else
        {
            ROS_ERROR("Failed	to	call	service	Service_demo");
            return	1;
        }
        return	0;
    }    
	以上代码比较关键的地方有两处,一个是建立一个ServiceClient,另一个是开始调用服务。建立client需要用 	nh.serviceClient<service_demo::Greeting>("greetings")	 ,指明服务的类型和服务的名称。而调用时可以直接用 	client.call(srv)	 ,返回结果不是response,而是是否成
功调用远程服务。CMakeLists.txt	 和 	pacakge.xml	 修改方法和 	topic_demo	 修改方法类似,不再赘述。
```

## param in roscpp

```cpp
	严格来说,param并不能称作一种通信方式,因为它往往只是用来存储一些静态的设置,而不是动态变化的。所以关于param的操作非常轻巧,非常简单。	关于param的API,roscpp为我们提供了两套,一套是放在 	ros::param	 namespace下,另一套是在 	ros::NodeHandle	 下,这两套API的操作完全一样,用哪一个取决于你的习惯;

1. 	param_demo/param.cpp	 文件,内容包括:
    #include<ros/ros.h>
    int	main(int	argc,	char	**argv){
        ros::init(argc,	argv,	"param_demo");
        ros::NodeHandle	nh;
        int	parameter1,	parameter2,	parameter3,	parameter4,	parameter5;
        //Get	Param的三种方法
        //1	ros::param::get()获取参数“param1”的value,写入到parameter1上
        bool	ifget1	=	ros::param::get("param1",	parameter1);
        //2	ros::NodeHandle::getParam()获取参数,与1作用相同
        bool	ifget2	=	nh.getParam("param2",parameter2);
        //3	ros::NodeHandle::param()类似于1和2
        //但如果get不到指定的param,它可以给param指定一个默认值(如33333)
        nh.param("param3",	parameter3,	33333);
        if(ifget1)	//param是否取得
            ...
            //Set	Param
            //1	ros::param::set()设置参数
            parameter4	=	4;
        ros::param::set("param4",	parameter4);
        //2	ros::NodeHandle::setParam()设置参数
        parameter5	=	5;
        nh.setParam("param5",parameter5);
        //Check	Param
        //1	ros::NodeHandle::hasParam()
        bool	ifparam5	=	nh.hasParam("param5");
        //2	ros::param::has()
        bool	ifparam6	=	ros::param::has("param6");
        //Delete	Param
        //1	ros::NodeHandle::deleteParam()
        bool	ifdeleted5	=	nh.deleteParam("param5");
        //2	ros::param::del()
        bool	ifdeleted6	=	ros::param::del("param6");
                    ...
    }

2. 	param_demo中的launch文件
    实际项目中我们对参数进行设置,尤其是添加参数,一般都不是在程序中,而是在launch文件中。因为launch文件可以方便的修改参数,而写成代码之后,修改参数必须重新编译;
	param_demo/launch/param_demo_cpp.launch	 内容为:
    <launch>
                    <!--param参数配置-->
                    <param	name="param1"	value="1"	/>
                    <param	name="param2"	value="2"	/>
                    <!--rosparam参数配置-->
                    <rosparam>			
                                    param3:	3
                                    param4:	4
                                    param5:	5
                    </rosparam>
                    <!--以上写法将参数转成YAML文件加载,注意param前面必须为空格,不能用Tab,否则YAML解析错误-->
                    <!--rosparam	file="$(find	robot_sim_demo)/config/xbot2_control.yaml"	command="load"	/-->
                    <node	pkg="param_demo"	type="param_demo"	name="param_demo"	output="screen"	/>
    </launch>
    通过和两个标签我们设置好了5个param,从而在之前的代码中进行增删改查的操作。
                                        
3. 命名空间对param的影响
    在实际的项目中,实例化句柄时,经常会看到两种不同的写法	ros::NodeHandle	n; ros::NodeHandle	nh("~");`	这两种写法有什么不同呢?以本教学报的name_demo为例。在本节launch文件夹的demo.launch定义两个参数,一个全局serial	他的数值是5,一个是局部的serial,他的数值是10.
    <launch>
		<!--全局参数serial-->
		<param	name="serial"	value="5"	/>
		<node	name="name_demo"	pkg="name_demo"	type="name_demo"
			output="screen">
				<!--局部参数serial-->
				<param	name="serial"	value="10"	/>
		</node>
	</launch>
        
    在name_demo.cpp中,我们分别尝试了,利用全局命名空间句柄提取全局的param和局部的param,以及在局部命名空间下的句柄提取全局的param和局部的param,详细的代码如下:
#include	<ros/ros.h>
int	main(int	argc,	char*	argv[])
{
    int	serial_number	=	-1;//serial_number初始化
    ros::init(argc,	argv,	"name_demo");//node初始化
    /*创建命名空间*/
    //n	是全局命名空间
    ros::NodeHandle	n;
    //nh	是局部命名空间
    ros::NodeHandle	nh("~");
    /*全局命名空间下的Param*/
    ROS_INFO("global	namespace");
    //提取全局命名空间下的参数serial
    n.getParam("serial",	serial_number);
    ROS_INFO("global_Serial	was	%d",	serial_number);
    //提取局部命名空间下的参数serial
    n.getParam("name_demo/serial",	serial_number);//在全局命名空间下,要提取局部命名空间下的参数,需要添加node name
    ROS_INFO("global_to_local_Serial	was	%d",	serial_number);
    /*局部命名空间下的Param*/
    ROS_INFO("local	namespace");
    //提取局部命名空间下的参数serial
    nh.getParam("serial",	serial_number);
    ROS_INFO("local_Serial	was	%d",	serial_number);
    //提取全局命名空间下的参数serial
    nh.getParam("/serial",	serial_number);//在局部命名空间下,要提取全局命名空间下的参数,需要
    添加“/”
        ROS_INFO("local_to_global_Serial	was	%d",	serial_number);
    ros::spin();
    return	0;
}

结果：
[	INFO]	[1525095241.802257811]:	global	namespace
[	INFO]	[1525095241.803512501]:	global_Serial	was	5
[	INFO]	[1525095241.804515959]:	global_to_local_Serial	was	10
[	INFO]	[1525095241.804550167]:	local	namespace
[	INFO]	[1525095241.805126562]:	local_Serial	was	10
[	INFO]	[1525095241.806137701]:	local_to_global_Serial	was	5    
```

## 时钟定时器

```cpp
roscpp里有两种时间的表示方法,一种是时刻(ros::Time),一种是时长(ros::Duration)
    Time/Duration都由秒和纳秒组成。	要使用Time和Duration,需要 #include<ros/time.h>和 #include<ros/duration.h>
    
ros::Time	begin	=	ros::Time::now();	//获取当前时间
ros::Time	at_some_time1(5,20000000);		//5.2s (第一个数字是s 第二个数字是ns)
ros::Time	at_some_time2(5.2)	//同上,重载了float类型和两个uint类型的构造函数
ros::Duration	one_hour(60*60,0);	//1h
double	secs1	=	at_some_time1.toSec();//将Time转为double型时间
double	secs2	=	one_hour.toSec();//将Duration转为double型时间

	Time和Duration表示的概念并不相同,Time指的是某个时刻,而Duration指的是某个时段,尽管他们的数据结构都相同,但是用在不同的场景下。	ROS为我们重载了Time、Duration类型之间的加减运算,比如:
ros::Time	t1	=	ros::Time::now()	-	ros::Duration(5.5);	//t1是5.5s前的时刻,Time加减Duratio
n返回都是Time
ros::Time	t2	=	ros::Time::now()	+	ros::Duration(3.3);//t2是当前时刻往后推3.3s的时刻
ros::Duration	d1	=	t2	-	t1;//从t1到t2的时长,两个Time相减返回Duration类型
ros::Duration	d2	=	d1	-	ros::Duration(0,300);//两个Duration相减,还是Duration
	注意：以上是Time、Duration之间的加减运算,要注意没有Time+Time的做法。

** sleep **
通常在机器人任务执行中可能有需要等待的场景,这时就要用到sleep功能,roscpp中提供了
两种sleep的方法:
    ros::Duration(0.5).sleep();	//用Duration对象的sleep方法休眠

    ros::Rate	r(10);	//10HZ
    while(ros::ok())
    {
                    r.sleep();					
                    //定义好sleep的频率,Rate对象会自动让整个循环以10hz休眠,即使有任务执行占用了时间
    }

** timer **
    void callback1(const	ros::TimerEvent&)
    {
    	ROS_INFO("Callback	1	triggered");
    }
    void callback2(const	ros::TimerEvent&)
    {
        ROS_INFO("Callback	2	triggered");
    }
    int	main(int	argc,	char	**argv)
    {
        ros::init(argc,	argv,	"talker");
        ros::NodeHandle	n;
        ros::Timer	timer1	=	n.createTimer(ros::Duration(0.1),	callback1);		//timer1每0.1s触发一次callback1函数
        ros::Timer	timer2	=	n.createTimer(ros::Duration(1.0),	callback2);		//timer2每1.0s触发一次callback2函数
        ros::spin();		//千万别忘了spin,只有spin了才能真正去触发回调函数
        return	0;
    }
```

## 日志和异常

```cpp
** log **	
	ROS为开发者和用户提供了一套日志记录和输出系统,这套系统的实现方式是基于topic,也就是每个节点都会把一些日志信息发到一个统一的topic上去,这个topic就是 	/rosout	 。rosout	 本身也是一个node,它专门负责进行日志的记录。我们在启动master的时候,系统就会附带启动rosout。在roscpp中进行日志的输出,需要先 	include	<ros/console.h>	 ,这个头文件包括了五个级别的日志输出接口,分别是:
    - DEBUG
    - INFO
    - WARN
    - ERROR
    - FATAL	
        用法非常简单:
            ROS_DEBUG("The	velocity	is	%f",	vel);
            ROS_WARN("Warn:	the	use	is	deprecated.");
            ROS_FATAL("Cannot	start	this	node.");

** Exception **
    roscpp中有两种异常类型,当有以下两种错误时,就会抛出异常:
        ros::InvalidNodeNameException
        当无效的基础名称传给ros::init(),通常是名称中有/,就会触发
        ros::InvalidNameExcaption
        当无效名称传给了roscpp
```

#rospy

```cpp
//暂时用不到，后续学习
```

# TF与URDF

## TF 介绍

```cpp
TF(TransFrom)坐标转换；
    
    当机器人的"眼睛"获取一组数据,关于物体的坐标方位,但是相对于机器人手臂来说,这个坐标只是相对于机器人头部的传感器,并不直接适用于机器
人手臂执行,那么物体相对于头部和手臂之间的坐标转换,就是TF.
    
    坐标变换包括了位置和姿态两个方面的变换，ROS中的tf是一个可以让用户随时记录多个坐标系的软件包。tf保持缓存的树形结构中的坐标系之间的关系,并且允许用户在任何期望的时间点在任何两个坐标系之间转换点,矢量等.
    
    上图是我们常用的robot_sim_demo运行起来的tf tree结构,每一个圆圈代表一个frame,对应着机器人上的一个link,任意的两个frame之间都必须是联通的,如果出现某一环节的断裂,就会引发error系统报错.所以完整的tf	tree不能有任何断层的地方,这样我们才能查清楚任意两个frame之间的关系.仔细观察上图,我们发现每两个frame之间都有一个broadcaster,这就是为了使得两个frame之间能够正确连通,中间都会有一个Node来发布消息来broadcaster.如果缺少Node来发布消息维护连通,那么这两个frame之间的连接就会断掉.broadcaster就是一个publisher,如果两个frame之间发生了相对运动,broadcaster就会发布相关消息. 
    
    了解了在每个frame之间都会有broadcaster来发布消息维系坐标转换.那么这个消息到底是什么样子的呢?这个消息TransformStampde.msg,它就是处理两个frame之间一小段tf的数据格式.TransformStamped.msg的格式规范如下:
    std_mags/Header	header
        uint32	seq
        time	stamp
        string	frame_id
    string	child_frame_id
    geometry_msgs/Transform	transform
        geometry_msgs/Vector3	translation
            float64	x
            float64	y
            float64	z
        geometry_msgs/Quaternion	rotation
            float64	x
            float64	y
            flaot64	z
            float64	w
	观察标准的格式规范,首先header定义了序号,时间以及frame的名称.接着还写了child_frame,这两个frame之间要做那种变换就是由geometry_msgs/Transform来定义.Vector3三维向量表示平移,Quaternion四元数表示旋转.像下图TF树中的两个frame之间的消息,就是由这种格式来定义的.odom就是frame_id,baselink_footprint就是child_frame_id.
        
** TF树的数据类型 **
	1. tf/tfMessage.msg
	2. tf2_msgs/TFMessage.msg
  	这里TF的数据类型有两个,主要的原因是版本的迭代。自ROS	Hydro以来,tf第一代已被“弃用”,转而支持tf2。tf2相比tf更加简单高效。此外也添加了一些新的功能。官网建议新工作直接使用tf2,因为它有一个更清洁的界面,和更好的使用体验。如何查看自己使用的TF是哪一个版本,使用命令 	rostopic info /tf即可。      

** 格式定义 **
tf/tfMessage.msg或tf2_msgs/TFMessage标准格式规范如下:
	geometry_msgs/TransformStamped[]	transforms
    std_msgs/Header	header
        uint32	seq
        time	stamp
        string	frame_id
    string	child_frame_id
    geometry_msgs/Transform	transform
        geometry_msgs/Vector3	translation
            float64	x
            float64	y
            float64	z
        geometry_msgs/Quaternion	rotation
            float64	x
            float64	y
            flaot64	z
            float64	w
如上,一个TransformStamped数组就是一个TF tree。
```

## tf in c++

```cpp
** 数据类型 **
    C++中给我们提供了很多TF的数据类型,如下表:
    名称 						数据类型
    向量 						tf::Vector3
    点 						 tf::Point
    四元数 					tf::Quaternion
    3*3矩阵(旋转矩阵) 		 tf::Matrix3x3
    位姿 						tf::pose
    变换 						tf::Transform
    带时间戳的以上类型 		   tf::Stamped
    带时间戳的变换 			 tf::StampedTransform
        
** 数据转换 **
   		   |_____________\			四元素         /__________________
           |			 /		tf::Quaternion q  \					|	
          \|/													   \|/
        旋转矩阵                                                  欧垃角度
        tf::3X3Matix m	<-------------------------------->tfScalar(Double)r,p,y
        
例如常见的四元数、旋转矩阵、欧拉角这三种数据之间的转换。tf in roscpp给了我们解决该问题的函数。详细源码在我们教学课程的代码包中。首先在tf中与数据转化的数据都类型都包含在 #include<tf/tf.h>头文件中,我们将与数据转换相关API都存在tf_demo中的coordinate_transformation.cpp当中;

** example **
第1部分定义空间点和空间向量    
编号   	函数名称 														函数功能
1.1   	tfScalar::tfDot(const Vector3 &v1,const	Vector3&v2) 		计算两个向量的点积
1.2   	tfScalar length() 											计算向量的模
1.3 	Vector3	&normalize() 										求与已知向量同方向的单位向量
1.4 	tfScalar::tfAngle(const Vector3	&v1,const Vector3&v2) 		计算两个向量的夹角
1.5 	tfScale::tfDistance(const Vector3 &v1, const Vector3 &v2) 	计算两个向量的距离
1.6 	tfScale::tfCross(const	Vector3	&v1,const Vector3&v2) 		计算两个向量的乘积
    
示例代码:
#include	<ros/ros.h>
#include	<tf/tf.h>
//退出用:ctrl+z
int	main(int	argc,	char**	argv){
    //初始化
    ros::init(argc,	argv,	"coordinate_transformation");
    ros::NodeHandle	node;
    tf::Vector3	v1(1,1,1);
    tf::Vector3	v2(1,0,1);
    //第1部分,定义空间点和空间向量
    std::cout<<"第1部分,定义空间点和空间向量"<<std::endl;
    //1.1	计算两个向量的点积
    std::cout<<"向量v1:"<<"("<<v1[0]<<","<<v1[1]<<","<<v1[2]<<"),";
    std::cout<<"向量v2:"<<"("<<v2[0]<<","<<v2[1]<<","<<v2[2]<<")"<<std::endl;
    std::cout<<"两个向量的点积:"<<tfDot(v1,v2)<<std::endl;
    //1.2	计算向量的模
    std::cout<<"向量v2的模值:"<<v2.length()<<std::endl;
    //1.3	求与已知向量同方向的单位向量
    tf::Vector3	v3;
    v3=v2.normalize();
    std::cout<<"与向量v2的同方向的单位向量v3:"<<"("<<v3[0]<<","<<v3[1]<<","<<v3[2]<<")"<<std::
    endl;
    //1.4	计算两个向量的夹角
    std::cout<<"两个向量的夹角(弧度):"<<tfAngle(v1,v2)<<std::endl;
    //1.5	计算两个向量的距离
    std::cout<<"两个向量的距离:"<<tfDistance2(v1,v2)<<std::endl;
    //1.6	计算两个向量的乘积
    tf::Vector3	v4;
    v4=tfCross(v1,v2);
    std::cout<<"两个向量的乘积v4:"<<"("<<v4[0]<<","<<v4[1]<<","<<v4[2]<<")"<<std::endl;```
    return	0;
} 

** example2 **
第2部分定义四元数
编号 			函数名称															 函数功能
2.1 	setRPY(const tfScalar& yaw,	const stScalar &pitch,const	tfScalar &roll) 由欧拉角计算四元数
2.2 	Vector3	getAxis() 														由四元数得到旋转轴
2.3 	setRotation(const Vector3 &axis, const tfScalar&angle) 					已知旋转轴和旋转角估计四元数
    
示例代码:
#include	<ros/ros.h>
#include	<tf/tf.h>
//退出用:ctrl+z
int	main(int	argc,	char**	argv){
    //初始化
    ros::init(argc,	argv,	"coordinate_transformation");
    ros::NodeHandle	node;
    std::cout<<"第2部分,定义四元数"<<std::endl;
    //2.1	由欧拉角计算四元数
    tfScalar yaw,pitch,roll;
    yaw=0;pitch=0;roll=0;
    std::cout<<"欧拉角rpy("<<roll<<","<<pitch<<","<<yaw<<")";
    tf::Quaternion	q;
    q.setRPY(yaw,pitch,roll);
    std::cout<<",转化到四元数q:"<<"("<<q[3]<<","<<q[0]<<","<<q[1]<<","<<q[2]<<")"<<std::end
    l;
    //2.2	由四元数得到旋转轴
    tf::Vector3	v5;
    v5=q.getAxis();
    std::cout<<"四元数q的旋转轴v5"<<"("<<v5[0]<<","<<v5[1]<<","<<v5[2]<<")"<<std::endl;
    //2.3	由旋转轴和旋转角来估计四元数
    tf::Quaternion	q2;
    q2.setRotation(v5,1.570796);
    std::cout<<"旋转轴v5和旋转角度90度,转化到四元数q2:"<<"("<<q2[3]<<","<<q2[0]<<","<<q2[1]<<","<<q2[2]<<")"<<std::endl;
    return	0;
}

** example3 **
第3部分定义旋转矩阵
编号 函数名称 													函数功能
3.1 setRotaion(const Quaternion	&q) 							通过四元数得到旋转矩阵
3.2 getEulerYPR(tfScalar &yaw, tfScalar	&pitch,	tfScalar&roll) 	由旋转矩阵求欧拉角    
    
示例代码:
#include	<ros/ros.h>
#include	<tf/tf.h>
//退出用:ctrl+z
int	main(int	argc,	char**	argv){
    //初始化
    ros::init(argc,	argv,	"coordinate_transformation");
    ros::NodeHandle	node;
    //第3部分,定义旋转矩阵
    std::cout<<"第3部分,定义旋转矩阵"<<std::endl;
    //3.1	由旋转轴和旋转角来估计四元数
    tf::Quaternion	q2(1,0,0,0);
    tf::Matrix3x3	Matrix;
    tf::Vector3	v6,v7,v8;
    Matrix.setRotation(q2);
    v6=Matrix[0];
    v7=Matrix[1];
    v8=Matrix[2];
    std::cout<<"四元数q2对应的旋转矩阵M:"<<v6[0]<<","<<v6[1]<<","<<v6[2]<<std::endl;
    std::cout<<" "<<v7[0]<<","<<v7[1]<<","<<v7[2]<<std::endl;
    std::cout<<" "<<v8[0]<<","<<v8[1]<<","<<v8[2]<<std::endl;
    //3.2	通过旋转矩阵求欧拉角
    tfScalar	m_yaw,m_pitch,m_roll;
    Matrix.getEulerYPR(m_yaw,m_pitch,m_roll);
    std::cout<<"由旋转矩阵M,得到欧拉角rpy("<<m_roll<<","<<m_pitch<<","<<m_yaw<<")"<<std::endl;
    return	0;
};

** example4 **
常见的欧拉角与四元素的互换Euler2Quaternion.cpp 
 #include	<ros/ros.h>
#include	<tf/tf.h>
//退出用:ctrl+z
int	main(int	argc,	char**	argv){
    //初始化
    ros::init(argc,	argv,	"Euler2Quaternion");
    ros::NodeHandle	node;
    geometry_msgs::Quaternion	q;
    double	roll,pitch,yaw;
    while(ros::ok())
    {
        //输入一个相对原点的位置
        std::cout<<"输入的欧拉角:roll,pitch,yaw:";
        std::cin>>roll>>pitch>>yaw;
        //输入欧拉角,转化成四元数在终端输出
        q=tf::createQuaternionMsgFromRollPitchYaw(roll,pitch,yaw);
        //ROS_INFO("输出的四元数为:w=%d,x=%d,y=%d,z=%d",q.w,q.x,q.y,q.z);
        std::cout<<"输出的四元数为:w="<<q.w<<",x="<<q.x<<",y="<<q.y<<",z="<<q.z<<std::endl;
        ros::spinOnce();
    }
    return	0;
};  

//Quaternion2Euler.cpp
#include	<ros/ros.h>
#include	"nav_msgs/Odometry.h"
#include	<tf/tf.h>
//退出用:ctrl+z
int	main(int	argc,	char**	argv){
    //初始化
    ros::init(argc,	argv,	"Quaternion2Euler");
    ros::NodeHandle	node;
    nav_msgs::Odometry	position;
    tf::Quaternion	RQ2;		
    double	roll,pitch,yaw;
    while(ros::ok())
    {
        //输入一个相对原点的位置
        std::cout<<"输入的四元数:w,x,y,z:";
        std::cin>>position.pose.pose.orientation.w>>position.pose.pose.orientation.x>>positi
            on.pose.pose.orientation.y>>position.pose.pose.orientation.z;
        //输入四元数,转化成欧拉角数在终端输出
        tf::quaternionMsgToTF(position.pose.pose.orientation,RQ2);		
        //	tf::Vector3	m_vector3;	方法2
        //	m_vector3=RQ2.getAxis();
        tf::Matrix3x3(RQ2).getRPY(roll,pitch,yaw);		
        std::cout<<"输出的欧拉角为:roll="<<roll<<",pitch="<<pitch<<",yaw="<<yaw<<std::endl;
        //std::cout<<"输出欧拉角为:roll="<<m_vector3[0]<<",pitch="<<m_vector3[1]<<",yaw="<<m_
        vector3[2]<<std::endl;
        ros::spinOnce();
    }
    return	0;
};				
```

## TF类

```cpp
1. tf::TransformBroadcaster类
    transformBroadcaster()
    void	sendTransform(const	StampedTransform	&transform)
    void	sendTransform(const	std::vector<StampedTransform>	&transforms)
    void	sendTransform(const	geometry_msgs::TransformStamped	&transform)
    void	sendTransform(const	std::vector<geometry_msgs::TransformStamped>	&transforms)
    
    这个类在前面讲TF树的时候提到过,这个broadcaster就是一个publisher,而sendTransform的作用是来封装publish的函数。在实际的使用中,我们需要在某个Node中构建tf::TransformBroadcaster类,然后调用sendTransform(),将transform发布到 	/tf	 的一段transform上。 	/tf	 里的transform为我们重载了多种不同的函数类型。在我们的tf_demo教学包当中提供了相关的示例代码tf.broadcaster.cpp,具体如下:
#include	<ros/ros.h>
#include	<tf/transform_broadcaster.h>
#include	<tf/tf.h>
//退出用:ctrl+z
int	main(int	argc,	char**	argv){
    //初始化
    ros::init(argc,	argv,	"tf_broadcaster");
    ros::NodeHandle	node;
    static	tf::TransformBroadcaster	br;
    tf::Transform	transform;
    //geometry_msgs::Quaternion	qw;
    tf::Quaternion	q;
    //定义初始坐标和角度
    double	roll=0,pitch=0,yaw=0,x=1.0,y=2.0,z=3.0;
    ros::Rate	rate(1);
    while(ros::ok())
    {
        yaw+=0.1;//每经过一秒开始一次变换
        //输入欧拉角,转化成四元数在终端输出
        q.setRPY(roll,pitch,yaw);
        //qw=tf::createQuaternionMsgFromRollPitchYaw(roll,pitch,yaw);方法2
        transform.setOrigin(tf::Vector3(x,y,z));
        transform.setRotation(q);
        std::cout<<"发布tf变换:sendTransform函数"<<std::endl;
        br.sendTransform(tf::StampedTransform(transform,ros::Time::now(),"base_link","link1"));
        std::cout<<"输出的四元数为:w="<<q[3]<<",x="<<q[0]<<",y="<<q[1]<<",z="<<q[2]<<std::endl;
        //std::cout<<"输出的四元数为:w="<<qw.w<<",x="<<qw.x<<",y="<<qw.y<<",z="<<qw.z<<std::endl;
        rate.sleep();
        ros::spinOnce();
    }
    return	0;
};

2. tf::TransformListener类
void	lookupTranform(const std::string &target_frame,const std::string &source_frame,const ros::Time&time,StampedTransform				&transform)const
bool	canTransform()
bool	waitForTransform()const
    
    上一个类是向 	/tf	 上发的类,那么这一个就是从 	/tf	 上接收的类。首先看lookuptransform()函数,第一个参数是目标坐标系,第二个参数为源坐标系,也即是得到从源坐标系到目标坐标系之间的转换关系,第三个参数为查询时刻,第四个参数为存储转换关系的位置。值得注意,第三个参数通常用 	ros::Time(0)	 ,这个表示为最新的坐标转换关系,而 	ros::time::now	 则会因为收发延迟的原因,而不能正确获取当前最新的坐标转换关系。canTransform()是用来判断两个transform之间是否连通,waitForTransform()const是用来等待某两个transform之间的连通,在我们的tf_demo教学包当中提供了相关的示例代码
	tf_listerner.cpp,具体如下:
#include	<ros/ros.h>
#include	<tf/transform_listener.h>
#include	<geometry_msgs/Twist.h>
int	main(int	argc,	char**	argv){
    ros::init(argc,	argv,	"tf_listener");
    ros::NodeHandle	node;
    tf::TransformListener	listener;
    //1.	阻塞直到frame相通
    std::cout<<"1.	阻塞直到frame相通"<<std::endl;
    listener.waitForTransform("/base_link","link1",ros::Time(0),ros::Duration(4.0));
    ros::Rate	rate(1);
    while	(node.ok()){
        tf::StampedTransform	transform;
        try{
            //2.	监听对应的tf,返回平移和旋转
            std::cout<<"2.	监听对应的tf,返回平移和旋转"<<std::endl;
            listener.lookupTransform("/base_link",	"/link1", ros::Time(0),	transform);
            //ros::Time(0)表示最近的一帧坐标变换,不能写成ros::Time::now()
        }
        catch	(tf::TransformException	&ex)	{
            ROS_ERROR("%s",ex.what());
            ros::Duration(1.0).sleep();
            continue;
        }
        std::cout<<"输出的位置坐标:x="<<transform.getOrigin().x()<<",y="<<transform.getOrigin().
            y()<<",z="<<transform.getOrigin().z()<<std::endl;
        std::cout<<"输出的旋转四元数:w="<<transform.getRotation().getW()<<",x="<<transform.getRotation().getX()<<
            ",y="<<transform.getRotation().getY()<<",z="<<transform.getRotation().getZ()<<std::endl;
        rate.sleep();
    }
    return	0;
};
```

## 统一机器人描述格式URDF

```cpp
	统一机器人描述格式,URDF使用XML格式描述机器人文件;URDF组件,是由不同的功能包和组件组成:
	在URDF当中,当你想要描述一个机器人的时候,例如小车的base_link和右轮right,两个link之间需要joint来连接;

** 制作URDF模型 **
    1. 添加模型
    我们以构建一个小车为例子,为大家讲解这部分的内容:(相关的示例代码可以从我们的tf_demo中找到),我们的想法是,首先构建base_link作为小车的父坐标系,然后在
base_link基础上,再构建左轮,右轮	和雷达	link.最后不同的link之间通过joint来连接。参考
	代码如下:
	<robot name="mycar">
    	<link name="base_link"/>
        <link name="right"/>
        <link name="left"/>
        <link name="rplidar"/>
        
        <joint name="right_joint" type="continuous">
        	<parent link="base_link">
        	<child link="right">
        </joint>
        
        <joint name="left_joint" type="continuous">
        	<parent link="base_link">
        	<child link="left">
        </joint>
        
        <joint name="rplidar_joint" type="fixed">
        	<parent link="base_link">
        	<child link="rplidar">
        </joint>
    </robot>
    技巧:	sudo apt-get install liburdfdom-tools,安装完毕后,执行检查check_urdfmy_car.urdf如果一切正常,就会有如下显示:
    随后打开新的终端,输入roslaunch	urdf_demo	display_urdf_link_joint.launch,回车之后,发现所有的link和joint都在一起了
        
    2. 添加机器人link之间的相对位置关系
    在基础模型之上,我们需要为机器人之间link来设相对位置和朝向的关系,URDF中通过<origin>来描述这种关系  
    <robot name="mycar">
    	<link name="base_link"/>
        <link name="right"/>
        <link name="left"/>
        <link name="rplidar"/>
        
        <joint name="right_joint" type="continuous">
        	<parent link="base_link">
        	<child link="right">
        	<origin rpy="1.57075 0 0" xyz="0 0.2 0.07"/>
        </joint>
        
        <joint name="left_joint" type="continuous">
        	<parent link="base_link">
        	<child link="left">
        	<origin rpy="-1.57075 0 0" xyz="0 0.2 0.07"/>
        </joint>
        
        <joint name="rplidar_joint" type="fixed">
        	<parent link="base_link">
        	<child link="rplidar">
        	<origin xyz="0.2 0 0.12"/>
        </joint>
    </robot>
        
    3. 添加模型的尺寸,形状和颜色等
    在已经设置好模型的link基础上,添加模型的形状(例如圆柱或长方体),相对于link的位置,颜色等。其中形状用<geometry>来描述,颜色用<color>来描述。
    <robot name="mycar">
    	<link name="base_link">
        	<visual>
        		<geometry>
        			<cylinder length=".06" radius="0.27"></cylinder>
        		</geometry>
        		<origin rpy="0 0 0" xyz="0 0 0.1"/>
        		<material name="white">
        			<color rgba="1 1 1 1">
        		</material>
        	</visual>
        </link>
        
        <link name="right">
        	<visual>
        		<geometry>
        			<cylinder length="0.04" radius="0.07"></cylinder>
        		</geometry>
        		<origin rpy="0 0 0" xyz="0 0 0"/>
        		<material name="black">
        			<color rgba="0 0 0 1">
        		</material>
        	</visual>
        </link>
        
        <link name="left">
        	<visual>
        		<geometry>
        			<cylinder length="0.04" radius="0.07"></cylinder>
        		</geometry>
        		<origin rpy="0 0 0" xyz="0 0 0"/>
        		<material name="black">
        			<color rgba="0 0 0 1">
        		</material>
        	</visual>
        </link>
    </robot>   
        
4. 显示URFD模型
    想要在rviz中显示出我们制作好的小车的URDF模型,可以写一个launch文件,参考如下:
<launch>
    <arg name="model" default="(find tf_demo)/urdf/mycar.urdf">
    <param name="robot_description" command="$(find xacro)/xacro.py $(arg model)" />
    <param name="use_gui" value="false" />
    <node name="joint_state_publisher" pkg="joint_state_publisher" type="joint_state_publisher" />
    <node name="robot_state_publisher" pkg="robot_state_publisher" type="robot_state_publisher" />
    <node pkg="rviz" type="rviz" name="rviz" args="-d $(find tf_demo/urdf/mycar.rviz)" output="screen" />
</launch>
    除了launch文件中的前3句,导入我们制作小车的URDF模型外,还需要添加joint_state_publisher和robot_state_publisjer这两个节点
```

## xacro模型

```cpp
	什么是Xacro? 我们可以把它理解成为针对URDF的扩展性和配置性而设计的宏语言(macro language)。有了Xacro,我们就可以像编程一样来写URDF文件。XACRO格式提供了一些更高级的方式来组织编辑机器人描述. 主要提供了三种方式来使得整个描述文件变得简单。
        
1. Constants
Usage:
<xacro:property name="WIDTH" value="2.0"/>
    
2. Macros
Usage:
<xacro:macro name="default_origin">
<origin xyz="0 0 0" rpy="0 0 0"/>
</xacro:macro>
<xacro:default_origin />
    
3. Include
	很多模型都是已宏的形式进行定义, 并以最小集团分成很多个文件. 而最终的机器人描述就变得非常简单了. 下面摘录一个ur5的描述文件. 从中可以看出来xacro的强大优势. 在最后的示例中我们还能够看到, urdf文件也是能够直接导入进来的.
Usage:
<?xml version="1.0"?>
<robot xmlns:xacro="http://www.ros.org/wiki/xacro" name="ur5" >
<!-- common stuff -->
    <xacro:include filename="$(find ur_description)/urdf/ur5/common.gazebo.xacro" />
    <!-- ur5 -->
    <xacro:include filename="$(find ur_description)/urdf/ur5/ur5.urdf.xacro" />
    <!-- arm -->
    <xacro:ur5_robot prefix="" joint_limited="false"/>
    <link name="world" />
    <joint name="world_joint" type="fixed">
        <parent link="world" />
        <child link = "base_link" />
        <origin xyz="0.0 0.0 0.0" rpy="0.0 0.0 0.0" />
    </joint>
</robot>
include类似于C语言中的include, 先将该文件扩展到包含的位置. 但包含进来的文件很有可能只是一个参数宏的定义. 并没有被调用.
        
        
** 制作gazebo模型 **
   在已经制作好的xcaro模型的基础上,添加gazebo模型的组建,看起已经变得十分的具有可操作性。
1. 对于二轮差动模型通过添加libgazebo_ros_diff_drive.so插件对小车左右轮的控制。
<gazebo>
    <plugin name="differential_drive_controller" filename="libgazebo_ros_diff_drive.so">
        <robotNamespace>/</robotNamespace>
        <alwaysOn>true</alwaysOn>
        <legacyMode>false</legacyMode>
        <updateRate>50</updateRate>
        <leftJoint>mybot_left_wheel_hinge</leftJoint>
        <rightJoint>mybot_right_wheel_hinge</rightJoint>
        <wheelSeparation>${chassisWidth+wheelWidth}</wheelSeparation>
        <wheelDiameter>${2*wheelRadius}</wheelDiameter>
        <torque>20</torque>
        <commandTopic>mybot_cmd_vel</commandTopic>
        <odometryTopic>mybot_odom</odometryTopic>
        <odometryFrame>odom</odometryFrame>
        <robotBaseFrame>mybot_link</robotBaseFrame>
    </plugin>
</gazebo>
        
2.通过添加libgazebo_ros_p3d.so来计算里程
<gazebo>
    <plugin name="ground_truth" filename="libgazebo_ros_p3d.so">
        <frameName>map</frameName>
        <bodyName>mybot_chassis</bodyName>
        <topicName>odom</topicName>
        <updateRate>30.0</updateRate>
    </plugin>
</gazebo>
        
3. 最后,对gazebo模型中小车左右轮相关PID等参数进行设置
<gazebo reference="mybot_chassis">
	<material>Gazebo/Orange</material>
</gazebo>
<gazebo reference="caster_wheel">
    <mu1>0.0</mu1>
    <mu2>0.0</mu2>
	<material>Gazebo/Red</material>
</gazebo>
<gazebo reference="right_wheel">
    <mu1 value="1.0"/>
    <mu2 value="1.0"/>
    <kp value="10000000.0" />
    <kd value="1.0" />
    <fdir1 value="1 0 0"/>
    <material>Gazebo/Black</material>
</gazebo>
<gazebo reference="left_wheel">
    <mu1 value="1.0"/>
    <mu2 value="1.0"/>
    <kp value="10000000.0" />
    <kd value="1.0" />
    <fdir1 value="1 0 0"/>
    <material>Gazebo/Black</material>
</gazebo>
     
```

## 扩展(旋转矩阵，欧拉角，四元素)

```cpp
```

# SLAM

```cpp
	我们常见的几个研究的问题包括:建图(Mapping)、定位(Localization)和路径规划(Path	Planning),如果机器人带有机械臂,那么运动规划(Motion	Planning)也是重要的一个环节。而同步定位与建图(SLAM)问题位于定位和建图的交集部分。
    SLAM需要机器人在未知的环境中逐步建立起地图,然后根据地区确定自身位置,从而进一步定位。
        
    这一章我们来看ROS中SLAM的一些功能包,也就是一些常用的SLAM算法,例如Gmapping、Karto、Hector、Cartographer等算法。这一章我们不会去关注算法背后的数学原理,而是更注重工程实现上的方法,告诉你SLAM算法包是如何工作的,怎样快速的搭建起SLAM算法;

** 地图 **
	ROS中的地图很好理解,就是一张普通的灰度图像,通常为pgm格式。这张图像上的黑色像素表示障碍物,白色像素表示可行区域,灰色是未探索的区域。
    在SLAM建图的过程中,你可以在RViz里看到一张地图被逐渐建立起来的过程,类似于一块块拼图被拼接成一张完整的地图。这张地图对于我们定位、路径规划都是不可缺少的信息。事实上,地图在ROS中是以Topic的形式维护和呈现的,这个Topic名称就叫做 /map,它的消息类型是 nav_msgs/OccupancyGrid。
    
** nav_msgs/OccupancyGrid **
	然后我们来看一下地图的OccupancyGrid类型是如何定义的,你可以通过rosmsg show nav_msgs/OccupancyGrid来查看消息,或者直接rosed	nav_msgs OccupancyGrid.msg来查看srv文件;
	std_msgs/Header	header	#消息的报头
        uint32	seq
        time	stamp
        string	frame_id	#地图消息绑定在TF的哪个frame上,一般为map
    nav_msgs/MapMetaData	info	#地图相关信息
    	time	map_load_time	#加载时间
    	float32	resolution	#分辨率	单位:m/pixel
        uint32	width	#宽	单位:pixel
        uint32	height	#高	单位:pixel
        geometry_msgs/Pose	origin	#原点
        	geometry_msgs/Point	position
                float64	x
                float64	y
                float64	z
            geometry_msgs/Quaternion	orientation
                float64	x
                float64	y
                float64	z
                float64	w
    int8[]	data	#地图具体信息	
            
	这个srv文件定义了/map话题的数据结构,包含了三个主要的部分:header,info和data。header是消息的报头,保存了序号、时间戳、frame等通用信息,info是地图的配置信息,它反映了地图的属性,data是真正存储这张地图数据的部分,它是一个可变长数组, int8后面加了[],你可以理解为一个类似于vector的容器,它存储的内容有width*height个int8型的数据,也就是这张地图上每个像素;
```

## Gmapping

```cpp
	Gmapping算法是目前基于激光雷达和里程计方案里面比较可靠和成熟的一个算法,它基于粒子滤波,采用RBPF的方法效果稳定,许多基于ROS的机器人都跑的gmapping_slam。这个软件包位于ros-perception组织中的slam_gmapping仓库中。其中的 slam_gmapping是一个metapackage,它依赖了 gmapping ,而算法具体实现都在 gmapping软件包中,该软件包中的 slam_gmapping程序就是我们在ROS中运行的SLAM节点。如果你感兴趣,可以阅读一下gmapping的源代码。
        
	gmapping在ROS上运行的方法很简单
	rosrun gmapping slam_gmapping
        
	但由于gmapping算法中需要设置的参数很多,这种启动单个节点的效率很低。所以往往我们会把gmapping的启动写到launch文件中,同时把gmapping需要的一些参数也提前设置好,写进launch文件或yaml文件。具体可参考教学软包中的 slam_sim_demo中的 gmapping_demo.launch和 robot_gmapping.launch.xml	文件。
        
** Gmapping	SLAM计算图 **
	gmapping的作用是根据激光雷达和里程计(Odometry)的信息,对环境地图进行构建,并且对自身状态进行估计。因此它得输入应当包括激光雷达和里程计的数据,而输出应当有自身位置和地图;
												/tf
	(tf2_msg)									/slam_gmapping/entropy
						  slam_gmapping    		/map_metadata (nav_msgs/MapMetaData)
	  /scan										/map (nav_msgs/OccupancyGrid)
 （sensor_msgs/LaserScan）
                                                    
	位于中心的是我们运行的slam_gmapping节点,这个节点负责整个gmapping	SLAM的工作。它的输入需要有两个:
	输入：
     - /tf以及 /tf_static:坐标变换,类型为第一代的tf/tfMessage或第二代
                的 tf2_msgs/TFMessage其中一定得提供的有两个tf,一个
                是 base_frame与 laser_frame之间的tf,即机器人底盘和激光雷达之间的变换;一个
                是 base_frame与 odom_frame之间的tf,即底盘和里程计原点之间的坐标变
                换。 odom_frame可以理解为里程计原点所在的坐标系。
	- /scan:激光雷达数据,类型为 sensor_msgs/LaserScan
        
    /scan很好理解,Gmapping SLAM所必须的激光雷达数据,而 /tf是一个比较容易忽视的细节。尽管 /tf这个Topic听起来很简单,但它维护了整个ROS三维世界里的转换关系,而 slam_gmapping要从中读取的数据是 base_frame与 laser_frame间的tf,只有这样才能够把周围障碍物变换到机器人坐标系下,更重要的是 	base_frame与 odom_frame之间的tf,这个tf反映了里程计(电机的光电码盘、视觉里程计、IMU)的监测数据,也就是机器人里程计测得走了多少距离,它会把这段变换发布到 odom_frame和 laser_frame	之间。因此slam_gmapping会从 /tf中获得机器人里程计的数据。
        
    输出：
    - /tf:主要是输出 map_frame和odom_frame之间的变换
	- /slam_gmapping/entropy: std_msgs/Float64类型,反映了机器人位姿估计的分散程度
	- /map:	slam_gmapping建立的地图
	- /map_metadata	:地图的相关信息
        
    输出的 /tf	里又一个很重要的信息,就是 map_frame和 odom_frame	之间的变换,这其实就是对机器人的定位。通过连通 map_frame和odom_frame,这样 map_frame与 base_frame甚至与 laser_frame都连通了。这样便实现了机器人在地图上的定位。同时,输出的Topic里还有 /map,在上一节我们介绍了地图的类型,在SLAM场景中,地图是作为SLAM的结果被不断地更新和发布； 
        
** 里程计误差及修正 **
	目前ROS中常用的里程计广义上包括车轮上的光电码盘、惯性导航元件(IMU)、视觉里程计,你可以只用其中的一个作为odom,也可以选择多个进行数据融合,融合结果作为
odom。通常来说,实际ROS项目中的里程计会发布两个Topic: 
	1. /odom:类型为 nav_msgs/Odometry,反映里程计估测的机器人位置、方向、线速度、角速度信息。
	2. /tf:	主要是输出 odom_frame 和 base_frame之间的tf。这段tf反映了机器人的位置和方向变换,数值与 /odom	中的相同。
	
    由于以上三种里程计都是对机器人的位姿进行估计,存在着累计误差,因此当运动时间较长时,odom_frame和base_frame	之间变换的真实值与估计值的误差会越来越大。你可能会想,能否用激光雷达数据来修正 odom_frame和 base_frame的tf。事实上gmapping不是这么做的,里程计估计的是多少,odom_frame 和 base_frame	 的tf就显示多少,永远不会去修正这段tf。gmapping的做法是把里程计误差的修正发布到 map_frame和 odom_frame之间的tf上,也就是把误差补偿在了地图坐标系和里程计原点坐标系之间。通过这种方式来修正定位。
    这样 map_frame和 base_frame,甚至和 laser_frame之间就连通了,实现了机器人在地图上的定位。
        
** 服务 **
slam_gmapping也提供了一个服务:
	/dynamic_map:其srv类型为nav_msgs/GetMap,用于获取当前的地图。

该srv定义如下:nav_msgs/GetMap.srv
#	Get	the	map	as	a	nav_msgs/OccupancyGrid
---
nav_msgs/OccupancyGrid	map
    
可见该服务的请求为空,即不需要传入参数,它会直接反馈当前地图。
    
** 参数 **
	slam_gmapping需要的参数很多,这里以 slam_sim_demo教学包中的 gmapping_demo的参数为例,注释了一些比较重要的参数,具体请查看 	ROS-Academy-for-Beginners/slam_sim_demo/launch/include/robot_gmapping.launch.xml；
        
<node	pkg="gmapping"	type="slam_gmapping"	name="slam_gmapping"	output="screen">
    <param	name="base_frame"	value="$(arg	base_frame)"/>	<!--底盘坐标系-->
    <param	name="odom_frame"	value="$(arg	odom_frame)"/>	<!--里程计坐标系-->
    <param	name="map_update_interval"	value="1.0"/>	<!--更新时间(s),每多久更新一次地图,不是频率-->
    <param	name="maxUrange"	value="20.0"/>	<!--激光雷达最大可用距离,在此之外的数据截断不用-->
    <param	name="maxRange"	value="25.0"/>	<!--激光雷达最大距离-->
    <param	name="sigma"	value="0.05"/>
    <param	name="kernelSize"	value="1"/>
    <param	name="lstep"	value="0.05"/>
    <param	name="astep"	value="0.05"/>
    <param	name="iterations"	value="5"/>
    <param	name="lsigma"	value="0.075"/>
    <param	name="ogain"	value="3.0"/>
    <param	name="lskip"	value="0"/>
    <param	name="minimumScore"	value="200"/>
    <param	name="srr"	value="0.01"/>
    <param	name="srt"	value="0.02"/>
    <param	name="str"	value="0.01"/>
    <param	name="stt"	value="0.02"/>
    <param	name="linearUpdate"	value="0.5"/>
    <param	name="angularUpdate"	value="0.436"/>
    <param	name="temporalUpdate"	value="-1.0"/>
    <param	name="resampleThreshold"	value="0.5"/>
    <param	name="particles"	value="80"/>
    <param	name="xmin"	value="-25.0"/>
    <param	name="ymin"	value="-25.0"/>
    <param	name="xmax"	value="25.0"/>
    <param	name="ymax"	value="25.0"/>
    <param	name="delta"	value="0.05"/>
    <param	name="llsamplerange"	value="0.01"/>
    <param	name="llsamplestep"	value="0.01"/>
    <param	name="lasamplerange"	value="0.005"/>
    <param	name="lasamplestep"	value="0.005"/>
    <remap	from="scan"	to="$(arg	scan_topic)"/>
</node>      
```

## karto slam

```cpp
Karto SLAM和Gmapping SLAM在工作方式上非常类似,如下图所示
	输入的Topic同样是 /tf	和 /scan,其中 /tf里要连通odom_frame与 base_frame ,还有laser_frame。这里和Gmapping完全一样。唯一不同的地方是输出,slam_karto的输出少相比slam_gmapping了一个位姿估计的分散程度.
    
** 服务 **
	与Gmapping相同,提供/dynamic_map服务
    
** 参数 ** 
	这里以 ROS-Academy-for-Beginners中的 karto_slam为例,选取了它的参数文件 slam_sim_demo/param/karto_params.yaml,关键位置做了注释:

    #	General	Parameters
    use_scan_matching:	true
    use_scan_barycenter:	true
    minimum_travel_distance:	0.2	
    minimum_travel_heading:	0.174							#in	radians
    scan_buffer_size:	70
    scan_buffer_maximum_scan_distance:	20.0
    link_match_minimum_response_fine:	0.8
    link_scan_maximum_distance:	10.0
    loop_search_maximum_distance:	4.0
    do_loop_closing:	true
    loop_match_minimum_chain_size:	10
    loop_match_maximum_variance_coarse:	0.4					#	gets	squared	later
    loop_match_minimum_response_coarse:	0.8
    loop_match_minimum_response_fine:	0.8
    #	Correlation	Parameters	-	Correlation	Parameters
    correlation_search_space_dimension:	0.3
    correlation_search_space_resolution:	0.01
    correlation_search_space_smear_deviation:	0.03
    #	Correlation	Parameters	-	Loop	Closure	Parameters
    loop_search_space_dimension:	8.0
    loop_search_space_resolution:	0.05
    loop_search_space_smear_deviation:	0.03
    #	Scan	Matcher	Parameters
    distance_variance_penalty:	0.3						#	gets	squared	later
    angle_variance_penalty:	0.349						#	in	degrees	(gets	converted	to	radians	t
    hen	squared)
    fine_search_angle_offset:	0.00349			#	in	degrees	(gets	converted	to	radians)
    coarse_search_angle_offset:	0.349	#	in	degrees	(gets	converted	to	radians)
    coarse_angle_resolution:	0.0349							#	in	degrees	(gets	converted	to	radians)
    minimum_angle_penalty:	0.9
    minimum_distance_penalty:	0.5
    use_response_expansion:	false	
```

## hector slam

```cpp
	Hector SLAM算法不同于前面两种算法,Hector只需要激光雷达数据,而不需要里程计数据。这种算法比较适合手持式的激光雷达,并且对激光雷达的扫描频率有一定要求。Hector算法的效果不如Gmapping、Karto,因为它仅用到激光雷达信息。这样建图与定位的依据就不如多传感器结合的效果好。但Hector适合手持移动或者本身就没有里程计的机器人使用。
        
   	(tf2_msg)												/poseupdate (geometry_msgs/PoseWithCovarianceStamped)
/syscommand(std_msgs/string)								/slam_out_pose (PoseStampd)				  																hector_mapping   		/map_metadata (nav_msgs/MapMetaData)
	  /scan													/map (nav_msgs/OccupancyGrid)
 （sensor_msgs/LaserScan）
        
	位于中心的节点叫作hector_mapping	,它的输入和其他SLAM框架类似,都包括了 /tf 和 /scan,另外Hector还订阅一个 /syscommand	Topic,这是一个字符串型的Topic,当接收到 reset	消息时,地图和机器人的位置都会初始化到最初最初的位置。在输出的Topic方面,hector多了一个 /poseupdate和/slam_out_pose,前者是具有协方差的机器人位姿估计,后者是没有协方差的位姿估计。
        
** 服务 **
	与Gmapping相同,提供 	/dynamic_map查询地图服务
        
** 参数 **
    以 ROS-Academy-for-Beginners	中的 hector_slam为例,选取了它的launch文件 slam_sim_demo/launch/hector_demo.launch为例,关键位置做了注释:

	<node pkg="hector_mapping" type="hector_mapping" name="hector_height_mapping" output="screen">
        <param	name="scan_topic"	value="scan"	/>
        <param	name="base_frame"	value="base_link"	/>
        <param	name="odom_frame"	value="odom"	/>
        <param	name="output_timing"	value="false"/>
        <param	name="advertise_map_service"	value="true"/>
        <param	name="use_tf_scan_transformation"	value="true"/>
        <param	name="use_tf_pose_start_estimate"	value="false"/>
        <param	name="pub_map_odom_transform"	value="true"/>
        <param	name="map_with_known_poses"	value="false"/>
        <param	name="map_pub_period"	value="1"/>
        <param	name="update_factor_free"	value="0.45"/>
        <param	name="map_update_distance_thresh"	value="0.1"/>
        <param	name="map_update_angle_thresh"	value="0.05"/>
        <param	name="map_resolution"	value="0.05"/>
        <param	name="map_size"	value="1024"/>
        <param	name="map_start_x"	value="0"/>
        <param	name="map_start_y"	value="0"/>
    </node>
```

# Navigation

```cpp
	Navigation是机器人最基本的功能之一,ROS为我们提供了一整套Navigation的解决方案,包括全局与局部的路径规划、代价地图、异常行为恢复、地图服务器等等,这些开源工具包极大地减少了我们开发的工作量,任何一套移动机器人硬件平台经过这套方案就可以快速部署实现;
```

## Navigation stack

```cpp
	Navigation	Stack是一个ROS的metapackage,里面包含了ROS在路径规划、定位、地图、异常行为恢复等方面的package,其中运行的算法都堪称经典。Navigation	Stack的主要作用就是路径规划,通常是输入各传感器的数据,输出速度。一般我们的ROS都预装了Navigation
        
包名 功能
amcl 						定位
fake_localization 			定位
map_server 					提供地图
move_base 					路径规划节点
nav_core 					路径规划的接口类,包括base_local_planner、base_global_planner和recovery_behavior三个接口
base_local_planner 			实现了Trajectory Rollout和DWA两种局部规划算法
dwa_local_planner 			重新实现了DWA局部规划算法
parrot_planner 				实现了较简单的全局规划算法
navfn 						实现了Dijkstra和A*全局规划算法
global_planner 				重新实现了Dijkstra和A*全局规划算法
clear_costmap_recovery 		实现了清除代价地图的恢复行为
rotate_recovery 			实现了旋转的恢复行为
move_slow_and_clear 		实现了缓慢移动的恢复行为
costmap_2d 					二维代价地图
voxel_grid 					三维小方块(体素?)
robot_pose_ekf 				机器人位姿的卡尔曼滤波
	这么多package,你可能会觉得很乱,不过担心,在使用中其实还是比较简单的,我们接下来会对常用的主要功能进行介绍。
        
** Navigation工作框架 **
	上图中位于导航功能正中心的是move_base节点,可以理解为一个强大的路径规划器,在实际的导航任务中,你只需要启动这一个node,并且给他提供数据,就可以规划出路径和速度。	move_base之所以能做到路径规划,是因为它包含了很多的插件,像图中的白色圆圈 global_planner、local_planner、global_costmap、local_costmap 、recovery_behaviors。这些插件用于负责一些更细微的任务:全局规划、局部规划、全局地图、局部地图、恢复行为。而每一个插件其实也都是一个package,放在Navigation Stack里。关于move_base我们后面会进一步介绍,先来看看move_base外围有哪些输入输出。
        
    输入：
        /tf	 :提要提供的tf包括 map_frame、odom_frame、base_frame	以及机器人各关节之间的完成的一棵tf树。
		/odom:里程计信息
		/scan或 /pointcloud:传感器的输入信息,最常用的是激光雷达(sensor_msgs/LaserScan类型),也有用点云数据(sensor_msgs/PointCloud)的。
		/map:地图,可以由SLAM程序来提供,也可以由 map_server来指定已知地图。
		
        以上四个Topic是必须持续提供给导航系统的,下面一个是可随时发布的topic:
			move_base_simple/goal :目标点位置。
                
    输出：
      	/cmd_vel: geometry_msgs/Twist类型,为每一时刻规划的速度信息。         
```

## move_base

```cpp
	move_base算得上是Navigation中的核心节点,之所以称之为核心,是因为它在导航的任务中处于支配地位,其他的一些package都是它的插件;
	
	move_base要运行起来,需要选择好插件,包括三种插件: base_local_planner、base_global_planner和 recovery_behavior,这三种插件都得指定,否则系统会指定默认值。Navigation为我们提供了不少候选的插件,可以在配置move_base时选择。
        
1. base_local_planner插件:
    - base_local_planner:	实现了TrajectoryRollout和DWA两种局部规划算法
    - dwa_local_planner:	实现了DWA局部规划算法,可以看作是base_local_planner的改进版本
        
2. base_global_planner插件:
	- parrot_planner:	实现了较简单的全局规划算法
    - navfn:	实现了Dijkstra和A*全局规划算法
    - global_planner:	重新实现了Dijkstra和A*全局规划算法,可以看作navfn的改进版

3. recovery_behavior插件:
    - clear_costmap_recovery:	实现了清除代价地图的恢复行为
    - rotate_recovery:	实现了旋转的恢复行为
    - move_slow_and_clear:	实现了缓慢移动的恢复行为
        
	除了以上三个需要指定的插件外,还有一个costmap插件,该插件默认已经选择好,无法更改。
        
	以上所有的插件都是继承于nav_core里的接口, nav_core属于一个接口package,它只定义了三种插件的规范,也可以说定义了三种接口类,然后分别由以上的插件来继承和实现这些接口。因此如果你要研究路径规划算法,不妨研究一下nav_core定义的路径规划工作流程,然后仿照 dwa_local_planner或其他插件来实现。
    除了以上三个需要指定的插件外,还有一个costmap插件,该插件默认已经选择好,默认即为costmap_2d,不可更改,但costmap_2d提供了不同的Layer可以供我们设置;

	在这里插件的概念并不是我们抽象的描述,而是在ROS里catkin编译系统能够认出的,并且与其他节点能够耦合的C++库,插件是可以动态加载的类,也就是说插件不需要提前链接到ROS的程序上,只需在运行时加载插件就可以调用其中的功能;

** 插件选择(参数) **
   既然我们知道了move_base具体的一些插件,那如何来选择呢?其实非常简单。在move_base的参数设置里可以选择插件。move_base的参数包括以下内容: 
参数 						默认值 								  		功能
~base_global_planner 	navfn/NavfnROS 							 		设置全局规划器
~base_local_planner 	base_local_planner/TrajectoryPlannerROS  		设置局部规划器
~recovery_behaviors     [{name:	conservative_reset,	type:
                        clear_costmap_recovery/ClearCostmapRecovery},
                        {name:	rotate_recovery,	type:
                        rotate_recovery/RotateRecovery},	{name:
                        aggressive_reset,	type:
                        clear_costmap_recovery/ClearCostmapRecovery}] 	设置恢复行为 
    
	除了这三个选择插件的参数,还有控制频率、误差等等参数。	具体请看http://wiki.ros.org/move_base介绍。在ROS-Academy-for-Beginners的代码中的 navigation_sim_demo	例子中,由于要配置的参数太多,通常会将配置写在一个yaml文件中,我们用 param/move_base_params.yaml来保存以上参数。而关于一些具体插件,比如 dwa_local_planner则也会创建一个文件 param/dwa_local_planner.yaml来保存它的设置；

** Topic与Service **
move_base包含的Service包括:
    - make_plan:	nav_msgs/GetPlan类型,请求为一个目标点,响应为规划的轨迹,但不执行该轨迹。
    - clear_unknown_space:	std_srvs/Empty类型,允许用户清除未知区域地图。
    - clear_costmaps:	std_srvs/Empty类型,允许用户清除代价地图上的障碍物。 
        
** costmap **
   costmap是Navigation Stack里的代价地图,它其实也是move_base插件,本质上是C++的动态链接库,用过catkin_make之后生成.so文件,然后move_base在启动时会通过动态加载的方式调用其中的函数。    
        
   之前我们在介绍SLAM时讲过ROS里的地图的概念,地图就是 /map这个topic,它也是一张图片,一个像素代表了实际的一块面积,用灰度值来表示障碍物存在的可能性。然而在实际的导航任务中,光有一张地图是不够的,机器人需要能动态的把障碍物加入,或者清楚已经不存在的障碍物,有些时候还要在地图上标出危险区域,为路径规划提供更有用的信息。因为导航的需要,所以出现了代价地图。你可以将代价地图理解为,在 /map 之上新加的另外几层地图,不仅包含了原始地图信息,还加入了其他辅助信息。

    代价地图有一下特点:	
	1.首先,代价地图有两张,一张是 local_costmap,一张是global_costmap,分别用于局部路径规划器和全局路径规划器,而这两个costmap都默认
并且只能选择costmap_2d作为插件。	
    2.无论是 local_costmap还是 global_costmap,都可以配置他们的Layer,可以选择多个层次。costmap的Layer包括以下几种:
		- Static	Map	Layer:静态地图层,通常都是SLAM建立完成的静态地图。
		- Obstacle	Map	Layer:障碍地图层,用于动态的记录传感器感知到的障碍物信息。
		- Inflation	Layer:膨胀层,在以上两层地图上进行膨胀(向外扩张),以避免机器人的外壳会撞上障碍物。
		- Other	Layers:你还可以通过插件的形式自己实现costmap,目前已有 Social CostmapLayer、Range Sensor Layer等开源插件。可以同时选择多个Layer并存。
            
** 地图插件的选择 **
    与move_base插件的配置类似,costmap配置也同样用yaml	来保存,其本质是维护在参数服务器上。由于costmap通常分为local和global的coastmap,我们习惯把两个代价地图分开。以ROS-Academy-for-Beginners为例,配置写在了param文件夹下的global_costmap_params.yaml和local_costmap_params.yaml里。
    1. global_costmap_params.yaml:
	global_costmap:
			global_frame:	/map
			robot_base_frame:	/base_footprint
			update_frequency:	2.0
			publish_frequency:	0.5
			static_map:	true
			rolling_window:	false
			transform_tolerance:	0.5
			plugins:
					-	{name:	static_layer,				type:	"costmap_2d::StaticLayer"}
					-	{name:	voxel_layer,				type:	"costmap_2d::VoxelLayer"}
					-	{name:	inflation_layer,			type:	"costmap_2d::InflationLayer"}

	2. local_costmap_params.yaml
     local_costmap:
			global_frame:	/map
			robot_base_frame:	/base_footprint
			update_frequency:	5.0
			publish_frequency:	2.0
			static_map:	false
			rolling_window:	true
			width:	4.0
			height:	4.0
			resolution:	0.05
			origin_x:	5.0
			origin_y:	0
			transform_tolerance:	0.5
			plugins:
				-	{name:	voxel_layer,				type:	"costmap_2d::VoxelLayer"}
				-	{name:	inflation_layer,			type:	"costmap_2d::InflationLayer"}
	在plugins一项中可以设置Layer的种类,可以多层叠加。在本例中,考虑到局部地图并不需要静态地图,而只考虑传感器感知到的障碍物,因此可以删去StaticLayer。
```

## map_server

```cpp
	在某些固定场景下,我们已经知道了地图(无论通过SLAM还是测量),这样机器人每次启动最好就能直接加载已知地图,而不是每次开机都重建。在这种情况下,就需要有一个节点来发布 /map,提供场景信息了。
        
** map_server **
	map_server是一个和地图相关的功能包,它可以将已知地图发布出来,供导航和其他功能使用,也可以保存SLAM建立的地图。
        
    要让map_server发布 /map,需要输入给它两个文件:
        1. 地图文件,通常为pgm格式;
        2. 地图的描述文件,通常为yaml格式
            
    例如在ROS-Academy-for-Beginners里,我们提供了软件博物馆的地图文件,见slam_sim_demo/maps下:
		1. Software_Museum.pgm	
		2. Software_Museum.yaml
            image:	Software_Museum.pgm		#指定地图文件
            resolution:	0.050000				#地图的分辨率	单位为m/pixel
            origin:	[-25.000000,	-25.000000,	0.000000]			#地图的原点
            negate:	0				#0代表	白色为空闲	黑色为占据
            occupied_thresh:	0.65		#当占据的概率大于0.65认为被占据
            free_thresh:	0.196					#当占据的概率小于0.196认为无障碍
        其中占据的概率	occ	=	(255-color_avg)/255.0,	color_avg为RGB三个通道的平均值。
   有了以上两个文件,你可以通过指令来加载这张地图,map_server相关命令如下:  
        map_server命令 												作用
        rosrun map_server map_server Software_Museum.yaml		 	加载自定义的地图
        rosrun map_server map_saver -f mymap	 					保存当前地图为mymap.pgn和mymap.yaml
            
** **
当我运行  rosrun map_server map_server	***.yaml时,会有以下的通信接口: 
1. topic
    通常我们是在launch文件中加载map_server,发布地图。而map_server发布的消息包括:
        - /map_metadata:	发布地图的描述信息
        - /map:	发布锁存的地图消息
2. Service
    amcl的服务只有一个:
		- static_map:	用于请求和响应当前的静态地图。
3. param
amcl有一个参数需要设置,就是发布地图的frame。
	~frame_id:	string类型,默认为map。	绑定发布的地图与tf中的哪个frame,通常就是map。
	有两个概念不要搞混淆,map既是一个topic,也是一个frame,前者是topic通信方式中的一个话题,信息交互的频道,后者是tf中的一个坐标系,map_frame需要和其他的frame相连通。            
```

## acml

```cpp
	Adaptive Mentcarto Localization(AMCL),蒙特卡洛自适应定位是一种很常用的定位算法,它通过比较检测到的障碍物和已知地图来进行定位。AMCL上的通信架构如上图所示,与之前SLAM的框架很像,最主要的区别是 /map作为了输入,而不是输出,因为AMCL算法只负责定位,而不管建图。	
        										
   	(tf2_msg)												/amcl_pose (geometry_msgs/PoseWithCovarianceStamped)
	/map(nav_msgs/OccupancyGrid)  	    amcl	   			/tf 
	  /scan													/particlecloudpoint (geometry_msgs/PoseAray)
 （sensor_msgs/LaserScan） 
        
  同时还有一点需要注意,AMCl定位会对里程计误差进行修正,修正的方法是把里程计误差加到 map_frame	和odom_frame之间,而 odom_frame和 base_frame之间是里程计的测量值,这个测量值并不会被修正。这一工程实现与之前gmapping、karto的做法是相同的。
```

# Navigation工具包说明

```cpp
```

