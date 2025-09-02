## 目录

- `install/linux/bin/cfg`
  - `*.yaml`：部署文件，选择执行器。
- `module`
  - `executor_module`：基本执行器，线程安全执行器。
  - `executor_time_module`：时间调度执行器。
  - `executor_co_module`：协程执行器。

## 快速开始

提供两种方法，使用我的脚本`pkg_start.sh`或使用`xxx.sh`，各有优劣。

### pkg_start

启动`executor_module`：

```bash
./pkg_start.sh executor_pkg.yaml
```

启动`executor_time_module`：

```bash
./pkg_start.sh executor_time_pkg.yaml
```

启动`executor_co_module`：

```bash
./pkg_start.sh executor_co_pkg.yaml
```

### bash

进入`build`目录：

```bash
cd build
```

启动`executor_module`：

```bash
./start_executor_pkg.sh
```

启动`executor_time_module`：

```
./start_executor_time_pkg.sh
```

启动`executor_co_module`：

```
./start_executor_co_pkg.sh
```

## Executor

执行器是一个很早就有的概念，它表示一个可以执行逻辑代码的抽象概念，一个执行器可以是一个线程池、可以是一个协程/纤程，可以是 CPU、GPU、甚至是远端的一个服务器。

### 逻辑实现阶段

`AimRT`提供两种执行器：

- 基础执行器
  - 线程安全执行器：可以保证 **投递到其中的任务不会同时运行** ，使用`ThreadSafe`方法判断。
  - 时间调度执行器：支持按时间调度接口`ExecuteAt`、`ExecuteAfter`等。
- 协程执行器

### 部署运行阶段

参考：[aimrt.executor](https://docs.aimrt.org/tutorials/cfg/executor.html)

示例：

```yaml
aimrt:
  log: # log配置
    core_lvl: INFO # 内核日志等级
    backends: # 日志后端
      - type: console # 控制台日志
        options:
          pattern: "[%c:%f][%l][%t][%n][%G:%R @%F]%v"
      - type: rotate_file
  module:
    pkgs:
    - path: ./libexecutor_pkg.so
      enable_modules: [ExecutorModule]
    modules:
    - name: ExecutorModule
      log_lvl: INFO
  executor:
    executors:
      - name: work_executor
        type: asio_thread
        options:
          thread_num: 2 # asio_thread线程>1，不保证线程安全
      - name: thread_safe_executor
        type: asio_strand # 保证线程安全
        options:
          bind_asio_thread_executor_name: work_executor
      - name: time_schedule_executor
        type: asio_thread
        options:
          thread_num: 2
```

