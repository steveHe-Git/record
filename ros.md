# ros安装

> 设置编码

```cpp
sudo locale-gen en_US en_US.UTF-8
sudo update-locale LC_ALL=en_US.UTF-8 LANG=en_US.UTF-8
export LANG=en_US.UTF-8
```

> 设置软件源

```cpp
sudo apt update && sudo apt install curl gnupg2 lsb-release
curl -s https://raw.githubusercontent.com/ros/rosdistro/master/ros.asc | sudo apt-key add -

sudo sh -c 'echo "deb [arch=$(dpkg --print-architecture)] http://packages.ros.org/ros2/ubuntu $(lsb_release -cs) main" > /etc/apt/sources.list.d/ros2-latest.list'
```

> ### 安装ROS2

```cpp
sudo apt update
sudo apt install ros-foxy-desktop
```

> 设置环境变量

```cpp
source /opt/ros/foxy/setup.bash
```

> 安装自动补全工具

```cpp
sudo apt install python3-argcomplete
```
>ROS2的RWM默认使用的DDS是Fast RTPS，Cyclone DDS默认也安装好了，还可以这样安装RTI Connext

```cpp
sudo apt update
sudo apt install ros-foxy-rmw-connext-cpp    #for RTI Connext (requires license agreement)
    
安装之后通过环境变量即可切换DDS：
RMW_IMPLEMENTATION=rmw_connext_cpp
```

> example

```cpp
source /opt/ros/foxy/setup.bash
ros2 run demo_nodes_cpp talker

source /opt/ros/foxy/setup.bash
ros2 run demo_nodes_py listener    
```

> uninstall

```cpp
sudo apt remove ros-foxy-* && sudo apt autoremove
```

