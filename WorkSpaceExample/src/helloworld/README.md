## 目录

- `install/linux/bin/cfg`
  - `*.yaml`：部署文件，选择执行器。
- `module`
  - `helloworld_module`：参数模块。

## 快速开始

提供两种方法，使用我的脚本`pkg_start.sh`和`app_start.sh`或使用`xxx.sh`，各有优劣。

### pkg_start

`pkg`模式启动

```bash
./pkg_start.sh helloworld_pkg.yaml
```
`app`模式启动

```bash
./app_start.sh helloworld_app helloworld_app.yaml
```

### bash

进入`build`目录：

```bash
cd build
```

`pkg`模式启动

```bash
./start_helloworld_pkg.sh
```

`app`模式启动

```bash
./start_helloworld_app.sh
```

## Helloworld

参考：
[ModuleBase](https://docs.aimrt.org/tutorials/interface_cpp/module_base.html)
[CoreRef](https://docs.aimrt.org/tutorials/interface_cpp/core_ref.html)
[Logger](https://docs.aimrt.org/tutorials/interface_cpp/logger.html)
