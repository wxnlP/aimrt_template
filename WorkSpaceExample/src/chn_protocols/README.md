## 目录

- `install/linux/bin/cfg`
  - `*.yaml`：部署文件，选择执行器、`channel`后端等。
- `module`
  - `ros2_pub_module`：基于自定义`ros2`协议的发布者模块。
  - `ros2_sub_module`：基于自定义`ros2`协议的订阅者模块。
  - `pb_pub_module`：基于自定义`protobuf`协议的发布者模块。
  - `pb_sub module`：基于自定义`protobuf`协议的订阅者模块。
- `protocols`
  - `pb`：自定义`protobuf`消息接口类型。
  - `ros2`：自定义`ros2`消息接口类型。
  
## 快速开始

提供两种方法，是我的的脚本`pkg_start.sh`或使用`xxx.sh`，各有优劣。

### pkg_start

更新环境变量：

```bash
source ./setup.sh
```

启动`ros2`协议发布者：

```bash
./pkg_start.sh pub_ros2_col_cfg.yaml
```

启动`ros2`协议订阅者：

```bash
./pkg_start.sh sub_ros2_col_cfg.yaml
```

启动`protobuf`协议发布者：

```bash
./pkg_start.sh pub_pb_col_cfg.yaml
```

启动`protobuf`协议订阅者：

```bash
./pkg_start.sh sub_pb_col_cfg.yaml
```

启动`ros2`协议发布者+订阅者：

```bash
./pkg_start.sh ros2_col_cfg.yaml
```

启动`protobuf`协议发布者+订阅者：

```bash
./pkg_start.sh pb_col_cfg.yaml
```

### bash

更新环境变量：

```bash
source ./setup.sh
```

进入`build`目录：

```
cd build
```

启动`ros2`协议发布者：

```bash
./start_pub_ros2_col.sh
```

启动`ros2`协议订阅者：

```bash
./start_sub_ros2_col.sh
```

启动`protobuf`协议发布者：

```bash
./start_pub_pb_col.sh
```

启动`protobuf`协议订阅者：

```bash
./start_sub_pb_col.sh
```

启动`ros2`协议发布者+订阅者：

```bash
./start_ros2_col.sh
```

启动`protobuf`协议发布者+订阅者：

```bash
./start_pb_col.sh
```

## Protocols

### Potobuf

新建子项目同级目录`protocols/pb/test`，新建文件`info.proto`存放消息接口，内容如下：

```protobuf
syntax = "proto3";


// 命名空间
package example.protocols.test;

message Info {
    uint64 time_stamp = 1;
    string info = 2;
}
```

`test`目录下新建`CMakeLists.txt`文件：

- 前三段是标准的获取父级命名空间和设置目标的配置。
- 第四段使用`AimRT`封装的`CMake`语法生成目标，并起一个别名。
- 最后是安装文件。（可选）

```cmake
# Get the current folder name
string(REGEX REPLACE ".*/\(.*\)" "\\1" CUR_DIR ${CMAKE_CURRENT_SOURCE_DIR})

# Get namespace
get_namespace(CUR_SUPERIOR_NAMESPACE)
string(REPLACE "::" "_" CUR_SUPERIOR_NAMESPACE_UNDERLINE ${CUR_SUPERIOR_NAMESPACE})

# Set target name
set(CUR_TARGET_NAME ${CUR_SUPERIOR_NAMESPACE_UNDERLINE}_${CUR_DIR})
set(CUR_TARGET_ALIAS_NAME ${CUR_SUPERIOR_NAMESPACE}::${CUR_DIR})

# Generate 
add_protobuf_gencode_target_for_proto_path(
    TARGET_NAME     ${CUR_TARGET_NAME}_pb_gencode
    PROTO_PATH      ${CMAKE_CURRENT_SOURCE_DIR}
    GENCODE_PATH    ${CMAKE_CURRENT_BINARY_DIR}
)
add_library(${CUR_TARGET_ALIAS_NAME}_pb_gencode ALIAS ${CUR_TARGET_NAME}_pb_gencode)

# Set installation
install(
  DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
  DESTINATION "share"
  FILES_MATCHING
  PATTERN "*.proto")

set_property(TARGET ${CUR_TARGET_NAME}_pb_gencode PROPERTY EXPORT_NAME ${CUR_TARGET_ALIAS_NAME}_pb_gencode)
install(
  TARGETS ${CUR_TARGET_NAME}_pb_gencode
  EXPORT ${INSTALL_CONFIG_NAME}
  ARCHIVE DESTINATION lib
          FILE_SET HEADERS
          DESTINATION include/${CUR_TARGET_NAME}_pb_gencode)
```

`protocols`目录下新建`CMakeLists.txt`文件：

```cmake
# Set namespace
set_namespace()

add_subdirectory(pb/test)
```

最后将`protocols`目录包含进编译文件就好了，使用接口时包含`example::protocols::test_pb_gencode`依赖。

### ROS2

新建子项目同级目录`protocols/ros2`，在该目录下新建`ros2`功能包：

```bash
ros2 pkg create test_msgs --build-type ament_cmake --dependencies rosidl_default_generators --license Apache-2.0
```

> 更多`ros2`内容推荐：[ROS2 - 小李的知识库](https://tonmoon.top/study/ROS2/4-工作空间与功能包/)

`test_msgs`下新建`msg/Info.msg`添加如下内容：

```
uint64 time_stamp
string info
```

修改`CMakeLists.txt`：

```cmake
cmake_minimum_required(VERSION 3.8)

# Get the current folder name
string(REGEX REPLACE ".*/\(.*\)" "\\1" CUR_DIR ${CMAKE_CURRENT_SOURCE_DIR})

# Get namespace
get_namespace(CUR_SUPERIOR_NAMESPACE)
string(REPLACE "::" "_" CUR_SUPERIOR_NAMESPACE_UNDERLINE ${CUR_SUPERIOR_NAMESPACE})

# Set target name
set(CUR_TARGET_NAME ${CUR_DIR})

project(${CUR_TARGET_NAME})

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rosidl_default_generators REQUIRED)

set(CUR_BUILD_SHARED_LIBS ${BUILD_SHARED_LIBS})
set(BUILD_SHARED_LIBS ON)

# 注册消息接口
rosidl_generate_interfaces(
  ${PROJECT_NAME}
  "msg/Info.msg" 
)
set(BUILD_SHARED_LIBS ${CUR_BUILD_SHARED_LIBS})


if(NOT TARGET ${CUR_TARGET_NAME}::${CUR_TARGET_NAME}__rosidl_typesupport_cpp)
  add_library(${CUR_TARGET_NAME}::${CUR_TARGET_NAME}__rosidl_typesupport_cpp ALIAS ${CUR_TARGET_NAME}__rosidl_typesupport_cpp)
endif()

if(NOT TARGET ${CUR_TARGET_NAME}::${CUR_TARGET_NAME}__rosidl_typesupport_fastrtps_cpp)
  add_library(${CUR_TARGET_NAME}::${CUR_TARGET_NAME}__rosidl_typesupport_fastrtps_cpp ALIAS ${CUR_TARGET_NAME}__rosidl_typesupport_fastrtps_cpp)
endif()

ament_export_dependencies(rosidl_default_runtime)
ament_package()

```

`protocols`目录下`CMakeLists.txt`文件添加依赖：

```cmake
# Set namespace
set_namespace()

add_subdirectory(pb/test)
add_subdirectory(ros2/test_msgs)
```

使用接口时包含如下依赖：

```cmake
test_msgs::test_msgs__rosidl_generator_cpp
test_msgs::test_msgs__rosidl_typesupport_cpp
test_msgs::test_msgs__rosidl_typesupport_fastrtps_cpp
test_msgs::test_msgs__rosidl_typesupport_introspection_cpp
```

==特别注意==

使用自定义`ros2`包时要把其添加到环境变量：

```bash
source ./build/install/share/test_msgs/local_setup.zsh
```

