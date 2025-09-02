## 目录

- `install/linux/bin/cfg`
  - `*.yaml`：部署文件，选择执行器、`rpc`后端等。
- `module`
  - `ros2_client_module`：基于自定义`ros2`协议的客户端模块。
  - `ros2_server_module`：基于自定义`ros2`协议的服务端模块。
  - `pb_client_module`：基于自定义`protobuf`协议的客户端模块。
  - `pb_server_module`：基于自定义`protobuf`协议的服务端模块。
- `../protocols`
  - `pb/test/info_rpc.proto`：自定义`protobuf`消息接口类型。
  - `ros2/test_msgs/srv/RpcInfo.srv`：自定义`ros2`消息接口类型。

## 快速开始

提供两种方法，是我的的脚本`pkg_start.sh`或使用`xxx.sh`，各有优劣。

### pkg_start

更新环境变量：

```bash
source ./setup.sh
```

启动`ros2`协议客户端和服务端（`ros2`后端）：

```bash
./pkg_start.sh ros2_rpc_cfg.yaml
```

启动`protobuf`协议客户端和服务端（`local`后端）：

```
./pkg_start.sh pb_rpc_cfg.yaml
```

### bash

更新环境变量：

```bash
source ./setup.sh
```

进入`build`目录：

```bash
cd build
```

启动`ros2`协议客户端和服务端（`ros2`后端）：

```bash
./start_ros2_rpc.sh
```

启动`protobuf`协议客户端和服务端（`local`后端）：

```
./start_pb_rpc.sh
```

## Protocols

**请在完成`chn_protocols`学习后，再来看本部分内容。**

### Protobuf

新建子项目同级目录`protocols/pb/test`，新建文件`info_rpc.proto`存放消息接口，内容如下：

```protobuf
syntax = "proto3";

// 命名空间
package example.protocols.test;

message RpcInfoReq {
    uint64 time_stamp = 1;
    string info = 2;
}

message RpcInfoRes {
    uint64 req_stamp = 1;
    uint64 res_stamp = 2;
    string info = 3;
}

service InfoRpc {
    rpc GetRpcInfo (RpcInfoReq) returns (RpcInfoRes);
}
```

需要注意几个参数：

- `InfoRpc`：服务名称，作为一些生成的类的前缀。
- `GetRpcInfo`：核心`rpc`调用函数，客户端使用。
- `RpcInfoReq`和`RpcInfoRes`：`rpc`的请求和响应的数据结构名称。

在`chn_protocols`基础上，需要将`info_rpc.proto`加入编译，同样使用`AimRT`官方提供的`CMake`方法：

```cmake
add_protobuf_aimrt_rpc_gencode_target_for_proto_files(
  TARGET_NAME       ${CUR_TARGET_NAME}_aimrt_rpc_gencode
  PROTO_FILES       ${CMAKE_CURRENT_SOURCE_DIR}/info_rpc.proto
  GENCODE_PATH      ${CMAKE_CURRENT_BINARY_DIR}
  DEP_PROTO_TARGETS ${CUR_TARGET_NAME}_pb_gencode
)
add_library(${CUR_TARGET_ALIAS_NAME}_aimrt_rpc_gencode ALIAS ${CUR_TARGET_NAME}_aimrt_rpc_gencode)

set_property(TARGET ${CUR_TARGET_NAME}_aimrt_rpc_gencode PROPERTY EXPORT_NAME ${CUR_TARGET_ALIAS_NAME}_aimrt_rpc_gencode)
install(
  TARGETS ${CUR_TARGET_NAME}_aimrt_rpc_gencode
  EXPORT ${INSTALL_CONFIG_NAME}
  ARCHIVE DESTINATION lib
          FILE_SET HEADERS
          DESTINATION include/${CUR_TARGET_NAME}_aimrt_rpc_gencode)
```

需要注意，`add_protobuf_aimrt_rpc_gencode_target_for_proto_files`依赖于`add_protobuf_gencode_target_for_proto_path`，后者是对当前目录所有`.proto`文件作用，而后者进对单个指定`rpc`文件作用。

在模块中使用时，只需要添加如下依赖：

```cmake
example::protocols::test_aimrt_rpc_gencode
```

### Ros2

`test_msgs`下新建`msg/RpcInfo.srv`添加如下内容：

```
uint64 time_stamp
string info
---
uint64 req_stamp
uint64 res_stamp
string info
```

加入编译：

```cmake
# 注册消息接口
rosidl_generate_interfaces(
  ${CUR_PACKAGE_NAME}
  "msg/Info.msg"
  "srv/RpcInfo.srv"
)

add_ros2_aimrt_rpc_gencode_target_for_one_file(
  TARGET_NAME ${CUR_PACKAGE_NAME}_aimrt_rpc_gencode
  PACKAGE_NAME ${CUR_PACKAGE_NAME}
  PROTO_FILE ${CMAKE_CURRENT_SOURCE_DIR}/srv/RpcInfo.srv
  GENCODE_PATH ${CMAKE_CURRENT_BINARY_DIR}
  DEP_PROTO_TARGETS
    ${CUR_PACKAGE_NAME}::${CUR_PACKAGE_NAME}__rosidl_generator_cpp
    ${CUR_PACKAGE_NAME}::${CUR_PACKAGE_NAME}__rosidl_typesupport_cpp
    ${CUR_PACKAGE_NAME}::${CUR_PACKAGE_NAME}__rosidl_typesupport_fastrtps_cpp
    ${CUR_PACKAGE_NAME}::${CUR_PACKAGE_NAME}__rosidl_typesupport_introspection_cpp
)

add_library(${CUR_TARGET_ALIAS_NAME}_aimrt_rpc_gencode ALIAS ${CUR_TARGET_NAME}_aimrt_rpc_gencode)
```

在模块中使用时，只需要添加如下依赖：

```cmake
example::protocols::test_msgs_aimrt_rpc_gencode
```

