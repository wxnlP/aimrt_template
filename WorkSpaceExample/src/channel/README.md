## 目录

- `install/linux/bin/cfg`
  - `*.yaml`：部署文件，选择执行器、`channel`后端等。
- `module`
  - `ros2_publisher_module`：基于`ros2`协议的发布者模块。
  - `ros2_subscriber_module`：基于`ros2`协议的订阅者模块。
  - `pb_publisher_module`：基于`protobuf`协议的发布者模块。
  - `pb_subscriber module`：基于`protobuf`协议的订阅者模块。
- `protocols`
  - `pb`：自定义`protobuf`消息接口类型。
  - `ros2`：自定义`ros2`消息接口类型。

> 这里的协议（Protocol）的含义其实是`Module`之间通信的数据格式，用来描述数据的字段信息以及序列化、反序列化方式。通常由一种`IDL`( Interface description language )描述，然后由某种工具转换为各个语言的代码。
>
> `AimRT`目前官方支持两种`IDL`：
>
> - `Protobuf`
> - `ROS2`

## 快速开始

提供两种方法，是我的的脚步`pkg_start.sh`或使用`xxx.sh`，各有优劣。

### pkg_start

启动`ros2`协议发布者：

```bash
./pkg_start.sh pub_ros2_pkg_cfg.yaml
```

启动`ros2`协议订阅者：

```bash
./pkg_start.sh sub_ros2_pkg_cfg.yaml
```

启动`protobuf`协议发布者：

```bash
./pkg_start.sh pub_pb_pkg_cfg.yaml
```

启动`protobuf`协议订阅者：

```bash
./pkg_start.sh sub_pb_pkg_cfg.yaml
```

启动`ros2`协议发布者+订阅者：

```bash
./pkg_start.sh ros2_pkg_cfg.yaml
```

启动`protobuf`协议发布者+订阅者：

```bash
./pkg_start.sh pb_pkg_cfg.yaml
```

### bash

进入`build`目录：

```
cd build
```

启动`ros2`协议发布者：

```bash
./start_pub_ros2_pkg.sh
```

启动`ros2`协议订阅者：

```bash
./start_sub_ros2_pkg.sh
```

启动`protobuf`协议发布者：

```bash
./start_pub_pb_pkg.sh
```

启动`protobuf`协议订阅者：

```bash
./start_sub_pb_pkg.sh
```

启动`ros2`协议发布者+订阅者：

```bash
./start_ros2_pkg.sh
```

启动`protobuf`协议发布者+订阅者：

```bash
./start_pb_pkg.sh
```

## Channel

`AimRT`中最核心的设计理念之一就是 **逻辑实现与部署运行分离** ，使用`channel`的流程也是分逻辑实现（协议）和部署运行（后端）两个阶段。官方主要支持的协议为`protobuf`和`ros2`，官方支持的后端是`local`后端，其他后端如`ros2`、`mqtt`均有插件提供。

有几点需要注意：

1. `local`后端仅支持 **单进程内** 的模块通信，即不能开两个终端分别运行发布者和订阅者。
2. `protobuf`似乎只支持`local`后端，直接使用

### ROS2 Message

`ros2`协议的消息接口在源码的`protocols/ros2`目录下：

![](https://tonmoon.obs.cn-east-3.myhuaweicloud.com/img/tonmoon/20250824130917372.png)

`AimRT`的`ros2`消息接口使用与在`ros2`中一致，所以这里不介绍，仅介绍与`AimRT`相关的编译部署配置。

- **逻辑实现阶段**

使用`ros2`原生消息接口时，需要在模块中添加如下依赖：

```cmake
find_package([消息包名] REQUIRED)

target_link_libraries(
  ${CUR_TARGET_NAME}
  PRIVATE yaml-cpp::yaml-cpp
  PUBLIC aimrt::interface::aimrt_module_cpp_interface
  		 aimrt::interface::aimrt_module_ros2_interface
         [消息包名]::[消息包名]__rosidl_generator_cpp
         [消息包名]::[消息包名]__rosidl_typesupport_cpp
         [消息包名]::[消息包名]__rosidl_typesupport_fastrtps_cpp
         [消息包名]::[消息包名]__rosidl_typesupport_introspection_cpp)
```

例如，需要使用`std_msgs`消息接口：

```cmake
find_package(std_msgs REQUIRED)

# Set link libraries of target
target_link_libraries(
  ${CUR_TARGET_NAME}
  PRIVATE yaml-cpp::yaml-cpp
  PUBLIC aimrt::interface::aimrt_module_cpp_interface
         aimrt::interface::aimrt_module_ros2_interface
         std_msgs::std_msgs__rosidl_generator_cpp
         std_msgs::std_msgs__rosidl_typesupport_cpp
         std_msgs::std_msgs__rosidl_typesupport_fastrtps_cpp
         std_msgs::std_msgs__rosidl_typesupport_introspection_cpp
         )
```

编译时添加`ros`插件和依赖：

```bash
cmake -B build -DAIMRT_BUILD_WITH_ROS2=ON -DAIMRT_BUILD_ROS2_PLUGIN=ON
```

- **部署运行阶段**

使用`AimRT`官方提供的`local`后端：

```yaml
aimrt:
 executor: # 执行器
    executors:
      - name: pub_executor
        type: asio_thread
        options:
          thread_num: 2
 channel: # 数据通道
    backends:
      - type: local # 官方支持local后端
        options:
          subscriber_use_inline_executor: false
          subscriber_executor: pub_executor
    pub_topics_options:
      - topic_name: "(.*)" # 正则表达式
        enable_backends: [local]
        enable_filters: []
    sub_topics_options:
      - topic_name: "(.*)" # 正则表达式
        enable_backends: [local]
        enable_filters: []
          
```

使用`ros2`插件提供的后端，可以看做是一个`ros2`节点：

```yaml
aimrt:
 plugin:
    plugins:
      - name: ros2_plugin
        path: ./libaimrt_ros2_plugin.so
        options:
          node_name: aimrt_test_node
          executor_type: MultiThreaded
          executor_thread_num: 2
 executor: # 执行器
    executors:
      - name: pub_executor
        type: asio_thread
        options:
          thread_num: 2
 channel:
    backends:
      - type: local # 官方支持local后端
        options:
          subscriber_use_inline_executor: false
          subscriber_executor: pub_executor
      - type: ros2 # 官方ros2插件支持ros2后端
        options:
          pub_topics_options:
            - topic_name: "(.*)"
              qos:
                history: keep_last
                depth: 10
                reliability: reliable
                durability: volatile
                deadline: -1
                lifespan: -1
                liveliness: automatic
                liveliness_lease_duration: -1
    pub_topics_options:
      - topic_name: "(.*)" # 正则表达式
        enable_backends: [ros2]
        enable_filters: []
    sub_topics_options:
      - topic_name: "(.*)" # 正则表达式
        enable_backends: [ros2]
        enable_filters: []
```

### Protobuf

`protobuf`协议的消息接口在源码的`protocols/protobuf`目录下：

![](https://tonmoon.obs.cn-east-3.myhuaweicloud.com/img/tonmoon/20250824131329529.png)

`protobuf`消息接口的使用与`ros2`消息接口略有不同，主要体现在头文件包含、赋值方面。

- **逻辑实现阶段**

使用官方提供的`protobuf`协议，需要添加如下依赖：

```cmake
target_link_libraries(
  ${CUR_TARGET_NAME}
  PRIVATE yaml-cpp::yaml-cpp
  PUBLIC aimrt::interface::aimrt_module_cpp_interface
         aimrt::interface::aimrt_module_protobuf_interface
         aimrt::protocols::[消息包名])
```

以`common/header.proto`为例，它的消息包名为<kbd>common_pb_gencode</kbd>。

```cmake
target_link_libraries(
  ${CUR_TARGET_NAME}
  PRIVATE yaml-cpp::yaml-cpp
  PUBLIC aimrt::interface::aimrt_module_cpp_interface
         aimrt::interface::aimrt_module_protobuf_interface
         aimrt::protocols::common_pb_gencode)
```

这个消息包名称是创建这个消息接口的人自定义的，在`AimRT`中的命名规范是`xxx_pb_gencode`，`xxx`为`protobuf`文件所在文件夹的名称。所以使用`common/header.proto`就需要包含`aimrt::protocols::common_pb_gencode`依赖。具体的依赖名称，可以查看`AimRT`源码的编译文件，`add_protobuf_gencode_target_for_proto_path`下的 **TARGET_NAME** 选项：

> 除了`add_protobuf_gencode_target_for_proto_path`还有`add_protobuf_gencode_target_for_one_proto_file`，均是`AimRT`官方封装的语法。

![image-20250824154237446](https://tonmoon.obs.cn-east-3.myhuaweicloud.com/img/tonmoon/image-20250824154237446.png)

`protobuf`官方有提供工具将`xxx.proto`文件转化为`xxx.cc`和`xxx.h`文件，所以我们在使用`protobuf`协议时需要包含这些头文件。

例如，使用`common/header.proto`定义的消息接口就需要包含`header.pb.h`。

此外，使用消息接口是以函数的形式，这与`ros2`不同。函数的命名方式为`set_[参数名称]`，使用如下图所示：

```cpp
#include "header.pb.h"

aimrt::protocols::common::Header msg;
msg.set_frame_id("base_link");
msg.set_sequence_num(20250823);
msg.set_time_stamp(std::time(0));
```

编译时添加`protobuf`依赖：

```bash
cmake -B build -DAIMRT_BUILD_WITH_PROTOBUF=ON -DAIMRT_BUILD_PROTOCOLS=ON
```

- **部署运行阶段**

部署方式与`ros2`协议一致，这样正是`AimRT`的便利之处，逻辑实现与部署运行分离，可以直接复用`ros2`协议的部署文件。

### ROS2后端说明

使用`protobuf`协议部署`ros2`后端时可能会梦见如下错误：

![](https://tonmoon.obs.cn-east-3.myhuaweicloud.com/img/tonmoon/20250824134053972.png)

总结下来就是`ros2`后端找不到`ros2_plugin_proto`生成的相关动态库，但是查看`build`目录会发现这些动态库都在。

![](https://tonmoon.obs.cn-east-3.myhuaweicloud.com/img/tonmoon/20250824134332083.png)

将`build`目录添加到`ros2`后端的动态库搜索目录就好了，可以选择在工作空间根目录运行下面内容。

```bash
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$PWD/build
```

但是每次新开终端都需要刷新环境变量麻烦，可以选择使用绝对路径，并把内容添加到`.bashrc`文件。

```bash
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/home/lzh/AimRtWorks/WorkSpaceExample/build
```

其实两种办法都不够优雅，但苦于目前 **未找到更好的方法** 。

关于`ros2`后端配合`ros2`协议或非`ros2`协议，官方文档有进行一些说明，大概意思就是打通`AimRT`与`ROS2`的兼容，原文如下：

![](https://tonmoon.obs.cn-east-3.myhuaweicloud.com/img/tonmoon/20250824152425931.png)

虽然这样看似不错，但是`protobuf`协议强行使用`ros2`后端又很多不便：

1. 无法完全正常使用`ros2`命令行，比如`ros2 echo`无法使用。
2. 需要注意环境变量配置。

就目前的使用体验，`ros2`协议配合`ros2`后端做`ros2`生态兼容，`protobuf`协议配合`local`后端做进程内高效通信。其他后端目前还未尝试，或许`ros2`后端、`protobuf`协议有它们独特的使用场景是我未发现的，所以这些 **仅代表个人现阶段的观点** 。

## Protocol

这里介绍自定义`protocol`并在`AimRT`中使用。

### ROS2 Message





### Protobuf