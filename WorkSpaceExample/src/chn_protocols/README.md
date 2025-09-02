## 目录

- `install/linux/bin/cfg`
  - `*.yaml`：部署文件，选择执行器、`channel`后端等。
- `module`
  - `ros2_pub_module`：基于自定义`ros2`协议的发布者模块。
  - `ros2_sub_module`：基于自定义`ros2`协议的订阅者模块。
  - `pb_pub_module`：基于自定义`protobuf`协议的发布者模块。
  - `pb_sub_module`：基于自定义`protobuf`协议的订阅者模块。
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

```bash
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
