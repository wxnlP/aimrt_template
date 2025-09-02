## 目录

- `install/linux/bin/cfg`
  - `*.yaml`：部署文件，选择执行器。
- `module`
  - `parameter_module`：参数模块。

## 快速开始

提供两种方法，使用我的脚本`pkg_start.sh`或使用`xxx.sh`，各有优劣。

### pkg_start

启动

```bash
./pkg_start.sh parameter_pkg.yaml
```

### bash

进入`build`目录：

```bash
cd build
```

启动

```bash
./start_parameter_pkg.sh
```

## Parameter

参考：[Parameter](https://docs.aimrt.org/tutorials/interface_cpp/parameter.html)