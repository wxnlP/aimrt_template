## 目录

- `install/linux/bin/cfg`
  - `*.yaml`：部署文件，选择执行器、`rpc`后端等。
- `module`
  - `ros2_client_module`：基于自定义`ros2`协议的客户端模块。
  - `ros2_server_module`：基于自定义`ros2`协议的服务端模块。
  - `pb_client_module`：基于自定义`protobuf`协议的客户端模块。
  - `pb_server_module`：基于自定义`protobuf`协议的服务端模块。
- `../protocols`
  - `pb`：自定义`protobuf`消息接口类型。
  - `ros2`：自定义`ros2`消息接口类型。

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

```bash
./start_pb_rpc.sh
```

## RPC

`RPC(Remote procedure call)`是一种远程过程调用，基于请求-回复模型。`RPC`同`Channel`一样，逻辑实现与部署运行分离，可以使用`ros2 srv`和`protobuf`两种协议通信，经由不同的后端发送。

> 特别的，关于`ros2`后端的一些使用问题，在[channel](https://github.com/wxnlP/aimrt_template/tree/main/WorkSpaceExample/src/channel)中已经介绍，若未阅读学习建议先暂停后续内容。

同时，`rpc`子项目使用自定义的接口，若想使用原生接口可以参考[channel](https://github.com/wxnlP/aimrt_template/tree/main/WorkSpaceExample/src/channel)，去查看源码、添加依赖。

> 建议先去看[protocols](https://github.com/wxnlP/aimrt_template/tree/main/WorkSpaceExample/src/protocols)的介绍，可以更加理解下面的内容。

### Ros2 Service

- **逻辑实现阶段**

链接依赖，这里与`channel`中很不一样，因为`AimRT`提供了一个封装的`CMake`方法，使得链接依赖时更方便。

> `example::protocols`命名空间是特意加的，为了与`protobuf`保持格式一致。若你使用过`aimrt_cli`生成协议，你会发现它提供的编译脚本并没有命名空间。

```cmake
target_link_libraries(
  ${CUR_TARGET_NAME}
  PRIVATE yaml-cpp::yaml-cpp
  PUBLIC aimrt::interface::aimrt_module_cpp_interface
         aimrt::interface::aimrt_module_protobuf_interface
         example::protocols::test_msgs_aimrt_rpc_gencode)
```

编译时添加`ros`插件和依赖：

```bash
cmake -B build -DAIMRT_BUILD_WITH_ROS2=ON -DAIMRT_BUILD_ROS2_PLUGIN=ON
```

- **部署运行阶段**

使用`AimRT`官方提供的`local`后端：

```yaml
aimrt:
  log:
    core_lvl: INFO # Trace/Debug/Info/Warn/Error/Fatal/Off
    backends:
      - type: console
  module:
    pkgs:
      - path: ./librpc_pb_pkg.so
        disable_modules: []
    modules:
      - name: PbClientModule
        log_lvl: INFO
      - name: PbServerModule
        log_lvl: INFO
  executor:
    executors:
      - name: work_thread_pool
        type: asio_thread
        options:
          thread_num: 4
      - name: timeout_handle
        type: time_wheel
        options:
          bind_executor: work_thread_pool
  rpc:
    backends:
      - type: local
        options:
          timeout_executor: timeout_handle
    clients_options:
      - func_name: "(.*.)"
        enable_backends: [local]
        enable_filters: []
    servers_options:
      - func_name: "(.*.)"
        enable_backends: [local]
        enable_filters: []

# Module custom configuration
PbClientModule:
  service_name: rpc_pb/001
  rpc_frq: 0.2

PbServerModule:
  service_name: rpc_pb/001
```

使用`ros2`插件提供的后端，可以看做是一个`ros2`节点：

```yaml
aimrt:
  log:
    core_lvl: INFO # Trace/Debug/Info/Warn/Error/Fatal/Off
    backends:
      - type: console
  module:
    pkgs:
      - path: ./librpc_ros2_pkg.so
        disable_modules: []
    modules:
      - name: Ros2ClientModule
        log_lvl: INFO
      - name: Ros2ServerModule
        log_lvl: INFO
  plugin:
    plugins:
      - name: ros2_plugin
        path: ./libaimrt_ros2_plugin.so
        options:
          node_name: rpc_ros2
          executor_type: MultiThreaded
          executor_thread_num: 2
  executor:
    executors:
      - name: work_thread_pool
        type: asio_thread
        options:
          thread_num: 4
      - name: timeout_handle
        type: time_wheel
        options:
          bind_executor: work_thread_pool
  rpc:
    backends:
      - type: local
        options:
          timeout_executor: timeout_handle
      - type: ros2
        options:
          clients_options:
            - func_name: "(.*)"
              qos:
                history: keep_last
                depth: 10
                reliability: reliable
                durability: volatile
                deadline: -1
                lifespan: -1
                liveliness: automatic
                liveliness_lease_duration: -1
          servers_options:
            - func_name: "(.*)"
              qos:
                history: keep_last
                depth: 10
                reliability: reliable
                durability: volatile
                deadline: -1
                lifespan: -1
                liveliness: automatic
                liveliness_lease_duration: -1
    clients_options:
      - func_name: "(.*.)"
        enable_backends: [ros2]
        enable_filters: []
    servers_options:
      - func_name: "(.*.)"
        enable_backends: [ros2]
        enable_filters: []
  

# Module custom configuration
Ros2ClientModule:
  service_name: rpc_ros2/info
  rpc_frq: 0.2

Ros2ServerModule:
  service_name: rpc_ros2/info

```

**需要注意**

在使用`ros2`后端配置`rpc`时，`func_name`得到的服务名称和我们实际设置的`aimrt rpc`名称不太一样，`AimRT`默认会把`:`及之前的 `<msg_type>`去掉并且将非数字、字母和`/`的字符以其对应的`HEX`编码展示，并加上`_`作为前缀。

例如，我们当前设置的话题名称`rpc_ros2/info`，会被替换为`rpc_5Fros2/info`。同时，官方也提供了一个`remapping_rule`的替换规则，详见[ROS2 插件](https://docs.aimrt.org/tutorials/plugins/ros2_plugin.html#ros2-rpc)。

### Protobuf

> `rpc`使用`protobuf`的方式与`channel`几乎一致。

- **逻辑实现阶段**

链接依赖：

```cmake
target_link_libraries(
  ${CUR_TARGET_NAME}
  PRIVATE yaml-cpp::yaml-cpp
  PUBLIC aimrt::interface::aimrt_module_cpp_interface
         aimrt::interface::aimrt_module_protobuf_interface
         example::protocols::[消息包名])
```

编译时添加`protobuf`依赖：

```bash
cmake -B build -DAIMRT_BUILD_WITH_PROTOBUF=ON -DAIMRT_BUILD_PROTOCOLS=ON
```

- **部署运行阶段**

部署方式与`ros2`协议一致，这样正是`AimRT`的便利之处，逻辑实现与部署运行分离，可以直接复用`ros2`协议的部署文件。