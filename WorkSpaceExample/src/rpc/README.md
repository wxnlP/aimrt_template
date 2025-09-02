## 目录

- `install/linux/bin/cfg`
  - `*.yaml`：部署文件，选择执行器、`rpc`后端等。
- `module`
  - `ros2_client_module`：基于自定义`ros2`协议的客户端模块。
  - `ros2_server_module`：基于自定义`ros2`协议的服务端模块。
  - `pb_client_module`：基于自定义`protobuf`协议的客户端模块。
  - `pb_server_module`：基于自定义`protobuf`协议的服务端模块。

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
