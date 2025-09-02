## 目录

- `helloworld`
  - `Aimrt `基础框架与`Log`接口的使用，支持使用`app`和`pkg`两种模式启动。
  - 由于可执行文件与动态库名称与官方案例一样，不能和官方示例一起编译。
- `executor`
  - `executor`执行器接口使用，包括线程安全、时间调度执行器以及协程。
  - 协程不太好理解，建议先去了解一下`cpp`协程的概念。
- `parameter`
  - `parameter`参数接口使用，功能不够强大，配合插件使用功能会更强大一些。
  - 参数插件的使用待更新。
- `channel`
  - `channel`通道接口使用，包括`ros2`、`protobuf`两种协议的原生消息类型，`local`、`ros2`两种后端。
  - 预计`ros2`部分会多一些，`AimRT`没有命令行工具，所以借助`ros2`后端可以使用`ros2`的工具会很方便。
  - 注意，`ros2`后端使用需要基于`Aimrt`官方提供的`ros2`插件。
- `protocols`
  - 自定义的消息接口，包括`msg`和`srv`，分别用于`channel`和`rpc`两种通信方式。
  - 需要熟悉接口的定义规则，以及编译结构搭建。

- `chn_protocols`

  - 使用自定义`msg`消息接口，包括`ros2`、`protobuf`两种协议的自定义消息类型，`local`、`ros2`两种后端。

  

