# vnc远程登录设置分辨率

```
登录上去可以设置分辨率，分辨率越高，表示需要带宽高，也就越卡
xrandr --fb 1024x768
```

# ros小车工程启动操作

```
#启动底层的驱动包
roslaunch xtark_driver xtark_driver.launch

#打印话题列表
rostopic list

#打印其中一个话题的实时数据
rostopic echo voltage

有些话题通过文本方式可能不方便观察数据变化,可以通过 ROS 提供的
rqt_plot 工具图形化显示数据,例如电池电压数据
rosrun rqt_plot rqt_plot
在”Topic”文本框输入“/”即可出现所有话题
列表,选择需要显示的话题,点击“+”添加按钮可加入到波形中

#打印话题信息
rostopic info /usb_cam/image_raw
图像话题的消息类型是 sensor_msgs/Image,这
是 ROS 定义的一种摄像头原始图像的消息类型,可以使用以下命令查看该图像消息的
详细定义

#消息可视化
rosmsg show sensor_msgs/Image

#查看图像(压缩过后)的传输速度
rostopic bw image_raw/compressed

#查看图像
roslaunch xtark_driver xtark_camera.launch 启动摄像头节点
rosrun rqt_image_view rqt_image_view （图像化显示图像）
选择中/usb_cam/image_raw,即可看到摄像头未压缩的原始数据画面,可以
明显观察到原始数据图像卡顿,帧率较低。选择压缩后的图像后,图像显示流畅。

#基于web显示摄像头
roslaunch xtark_driver xtark_camera.launch
#输入网址显示图像
http://192.168.12.1:8080/stream?topic=/camera/image_raw
#机器人搭载的摄像头支持多种分辨率格式,可以启动时设置参数来控制摄像头的
分辨率
roslaunch xtark_driver xtark_camera.launch resolution:=480p

#摄像头标定
ROS 官方提供了用于双目和单目摄像头标定的功能包——camera_calibration；
首先使用以下命令安装摄像头标定功能包 camera_calibration
roslaunch xtark_driver xtark_camera.launch
rosrun camera_calibration cameracalibrator.py --size 5x7 --square 0.03 image:=/camera/image_raw camera:=
可以得到畸变矩阵

#机器人移动控制
1. 键盘
roslaunch xtark_driver xtark_driver.launch
roslaunch xtark_ctl xtark_keyboard.launch

2. 遥控器
roslaunch xtark_driver xtark_driver.launch
roslaunch xtark_ctl xtark_joy.launch
遥控拨到on模式，如果此时是绿灯，长按MODE进入红灯常量，按住左1联合键进行转向；

3. app
后续补充

#机器人直线行驶校准
#底盘驱动
roslaunch xtark_driver xtark_driver.launch
#键盘控制行走
roslaunch xtark_ctl xtark_keyboard.launch
#图形化界面
rosrun rqt_reconfigure rqt_reconfigure
ServoBias 参数为小车舵机的零偏角参数,调节 ServoBias 参数,可以看到转向
轮舵机跟随转动。按键盘“I”控制小车前进,观察小车是否走直线,如果不走直线,
修改 ServoBias 参数直到小车走直线为止,记录下此时 ServoBias 参数数值

#里程计线速度校准
roslaunch xtark_driver xtark_calibrate_linear.launch
rosrun rqt_reconfigure rqt_reconfigure

#电机转速 PID 参数调节
roslaunch xtark_driver xtark_driver.launch
rosrun rqt_plot rqt_plot
选择一组电机速度数据,例如 a 电机,将 aset(期望速度) 和 avel(实际速度) 两个数据添加到示波器,
示波器实时显示两个数据
roslaunch xtark_ctl xtark_keyboard.launch
使用键盘操控机器人前进、后退等操作,可观察电机 A 曲线的变化,可看到电机
实际速度对设定值的跟随效果。
- 配置调好的PID的参数
进入 xtark_driver/config
文件夹,修改 xtark_params.yaml 配置文件的 PID 参数

#机器人 URDF 模型导入
```

# ROS 基本操作

> 查看环境变量中的ROS相关信息

```
printenv |grep ROS
```

> 创建ros的工作空间

```
# step 1
mkdir -p ~/catkin_ws/src #创建工作空间文件夹
cd ~/catkin_ws/          #切换到文件夹下
catkin_make              #初始化编译工作空间
# step 2
source devel/setup.bash  #source工作空间环境变量
						 #将当前工作空间环境变量加到ROS环境中，可以使用printenv |grep ROS进行查看
						 #source命令只对当前的终端有效
						 #需要永久有效需要加入到~/.bashrc
```

> ROS节点

```
概念： ros节点可以使用客户端库与其他节点通信，节点可以发布和接受一个话题，节点可以提供或使用某种服务

example:运行小乌龟
1. roscore
2. rosrun turtlesim turtlesim_node
3. rosrun turtlesim turtle_teleop_key
rosnode list   			    #查看节点列表
rosnode info 节点名字 		 #查看节点的信息
rostopic echo 话题名         # 显示话题的实时数据 
rosrun rqt_graph rqt_graph  #查看节点之间的通信链路
rostopic -h                 #话题help

vscode创建一个功能包的步骤
右键~/catkin_ws/src目录点击 Creat Catkin Package
1. 输入包名（ros_test）
2. 输入包的依赖 std_msgs roscpp(依赖库)
3. 编写CMakelists.txt
4. catkin_make编译完成 生成talker可执行文件
5. rosrun 包名 可执行文件名
```

> ros launch

```
launch 用来启动节点
执行launch会自动执行roscore；
launch启动节点的格式：
<launch>
	<node pkg="ros_test" name="listener" type="listener" output="screen" respawn="true"/>
	<node pkg="ros_test" name="talker" type="talker" output="screen" respawn="true"/>
</launch>

参数pkg表示的包的名称：
name表示的是
type表示的是当前节点的名字
output表示的是输出到屏幕
respawn表示的是如果程序意外中断，会继续重启

#启动launch
roslaunch 包名 对应的launch文件
roslaunch ros_test test.launch


#launch启动其他的launch文件
<launch>
	<include file="$(find ros_test)/launch/test.launch"/>
</launch>
```

> TF在ros中的基本作用和简单介绍

```cpp
TF库的目的是实现系统中的任何一个点在所有坐标系之间的坐标变换，也就是说，只要给定一个坐标系下一个点的坐标，就能获得这个点在其他坐标系的坐标

##1 监听tf变换
    接收并缓存系统中发布的所有坐标系变换，并从中查询所需要的参考系变换；    
##2. 广播tf变换
    向系统中广播坐标系之间的坐标变换关系，系统中更可能会存在多个不同部分的tf变换广播，每个广播都可以直接将坐标系变换关系直接插入tf树中，不需要在进行同步；
```

> ros常用的命令行工作

```cpp
1. roscore         #ros master启动
    roscore命令会运行主节点，主节点管理节点之间的消息通信中的连接信息
    主节点是使用ros时必须首先被运行的必要元素，ros节点由roscore命令驱动，并作为XNLRPC服务器运行。主节点接收多种消息的注册，如节点的名称、话题、服务名称、消息类型、URI地址和端口号，并再收到节点的请求时将此消息通知给其他节点。
    此外，会运行rosout，这个命令用于记录ROS中使用的ROS标准输出日志，例如DEBUG、INFO、WARN、ERROR、FATAL。它还运行一个管理参数服务器。当执行roscore时，将用户设置的ROS_MASTER_URI作为主URI， 并且驱动主节点。用户可以在~/.bashrc中设置ROS_MASTER_URI
    
2. roscd           #软件包路径切换工具
   roscd是rosbash命令集中的一部分，它允许你直接切换工作目录到某个软件包或者软件包集当中
    用法：
    roscd [本地包名称[/子目录]]
    
3.rospack         #软件包检索工具
    rospack是ros软件包管理工具，rospack允许获取软件包中的有关信息
    用法：
    rospack <command> [options][package]
实例功能：
    rospack list  #输出当前ROS所有软件包的信息
    rospack depends1 包名 #显示当前包的一级依赖
    rospack depends 包名 #显示当前包的所有依赖
    rospack profile #刷新当前ROS软件索引
    rospach -h #输出当前指令的帮助
    
4.rostopic        #ros话题命令行工具
    rostopic是ros话题工具，可以显示ros话题各种消息或发布话题
    用法：
    rostopic <command> [options][topic name]
实例功能：
    rostopic list        # 列出当前活动主题
    rostopic bw 话题名    #显示主题发布的频率，带宽
    rostopic delay 话题名 #显示话题的延时
    rostopic hz 话题名    #显示话题的发布的hz频率
    rostopic echo 话题名  #打印话题消息
    rostopic type 话题名  #显示话题消息类型
    rostopic pub [话题名][type][data...] #向话题发送指定类型的消息
    rostopic -h
    
5.rosnode         #ros节点命令行工作
    rosnode是ros节点工具，可以显示当前运行的ROS节点信息
    用法：
    rosnode <command> [options][node name]
实例功能：
    rosnode list          # 获取运行节点列表
    rosnode info 节点名    # 获取特定节点的信息
    rosnode ping 节点名    # 测试节点连通
    rosnode kill 节点名    # 终止节点
    rosnode -h
    
6.rosrun          #ros节点执行命令行工具
    rosrun可以直接使用包名运行一个包内节点
    用法：
    rosrun <包名> <executable>[参数]
    实例：
    rosrun ros_test talker #运行ros_test包内的 talker节点
    
7.rosbag          #ros数据包命令行工作
    rosbag将ROS系统运行过程中的数据录制到一个.bag文件中，然后可以通过回放数据来重现相似的运行过程。退出录制时按ctrl+c退出该命令，会在当前目录下生成以年份、日期和时间命名并以.bag作为后缀的文件
    用法：
    rosbag <command> [options][bagname]
实例功能：
    rosbag recode -a     #录制所有发布的话题，存储在.bag中
    rosbag record -O bagname.bag /cmd_vel /odom #录制特定话题到特定bag包中
    rosbag info <bag name> #查看bag文件信息
    rosbag play <bag name> #回放指定bag包
    rosbag play -r 2 <bag name> #以2hz频率回放bag包
    rosbag play -s 2 <bag name> #从2s某时间节点回放bag包
    rosbag -h #显示rosbag的所有的用法
```

> Rviz可视化工具

```
图形化显示工具
配置RVIZ显示插件
#运行导航模块
roslaunch xtark_nav xtark_nav.launch
#运行rviz
rosrun rviz rviz
#添加
点击add -> by topic ->通过话题添加对应的消息
点击file->save config as rviz文件
下次直接加载这个配置文件就行，不需要重复配置
```

> rqt 工具箱(GUI)

```
1. rqt_graph 工具是一个GUI工具，可用于可视化ROS计算图（节点话题之间的流向图）
rosrun rqt_graph rqt_graph
2. rqt_tf_tree 提供了一个GUI插件，用于可视化ROS-TF框架树(坐标变化的装换图)
rosrun rqt_tf_tree rqt_tf_tree
3. rqt_console 提供一个GUI插件来显示和过滤ROS消息
rosrun rqt_console rqt_console
4. rqt_plot 提供一个GUI插件，使用不同的绘图方式在2D界面中可视化数值
rosrun rqt_plot rqt_plot
5. rqt_reconfigure 提供一个基于dynamic_reconfigure框架的可视化参数修改工具，可以动态修改参数
rosrun rqt_reconfigure rqt_reconfigure
```

