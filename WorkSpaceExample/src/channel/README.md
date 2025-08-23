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
> - `ROS2 msg/srv`

## Channel

`AimRT`中最核心的设计理念之一就是 **逻辑实现与部署运行分离** ，使用`channel`的流程也是分逻辑实现和部署运行两个阶段。

### ros2

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

