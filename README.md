## 目录说明

```
├── HelloWorld --------- // 单个简单项目管理实践（无命名空间）
│   ├── app_start.sh --- // app模式启动脚本
│   ├── build.sh ------- // 编译构建脚本
│   ├── pkg_start.sh --- // pkg模式启动脚本
│   ├── setup.sh ------- // 临时环境变量配置脚本（减少启动指令的路径含量）
├── WorkSpaceExample --- // 参考官方源码的多个项目融合实践（有命名空间）
│   ├── app_start.sh --- // app模式启动脚本
│   ├── build.sh ------- // 编译构建脚本
│   ├── pkg_start.sh --- // pkg模式启动脚本
└── README.md ---------- // 说明文档
```

**WorkSpaceExample 案例还不是最终版，比如只编译部分子项目/只不编译部分子项目还没有做，后续会持续更新。** 

请根据[官方文档](https://docs.aimrt.org/tutorials/quick_start/build_from_source_ubuntu.html)完成环境依赖安装，并进行`Aimrt`源码编译构建后再使用本仓库。

## HelloWorld

本案例融合了`app`模式和`pkg`模式，启动时可以根据`yaml`配置文件和可执行程序自由选择（这就是逻辑和部署分离）。

- `app`模式的`CMake`构建环境搭建参考了`AimRT`官方文档的[快速开始](https://docs.aimrt.org/tutorials/quick_start/helloworld_cpp.html)；
- `pkg`模式的`CMake`构建环境搭建参考了`AimRT`源码中的[helloworld示例](https://github.com/AimRT/AimRT/tree/main/src/examples/cpp/helloworld)，并取消了`CMake`构建脚本中的命名空间使用，方便新手阅读学习。

### 如何使用

`AimRT`的`app`模式需要手动注册模块，核心程序在`src/app/helloworld_app/main.cc`。

**进入工程根目录** ，运行编译构建脚本，编译构建后，`build`根目录下会生成`helloworld_app`可执行文件：

```shell
./build.sh
```

运行临时环境变量配置脚本，激活环境变量`CONFIG_FILE_PATH`：

> 可以选择手动输入`yaml`文件完整路径，则忽略该脚本。

```shell
./setup.sh
```

使用`app`模式执行：

> 可以选择自己到`build`目录下执行`helloworld_app`可执行文件，后面加`yaml`文件路径。

```shell
# ./app_start.sh <可执行文件> <yaml文件路径>
./app_start.sh helloworld_app $CONFIG_FILE_PATH/helloworld_app.yaml
```

使用`pkg`模式执行：

>  可以选择自己到`build`目录下执行`aimrt_main`可执行文件，后面加`yaml`文件路径。

```shell
# ./pkg_start.sh <yaml文件路径>
./pkg_start.sh $CONFIG_FILE_PATH/helloworld_app.yaml
```

### 脚本解读

`build.sh`

```bash
#!/bin/bash

# exit on error and print each command
set -e

if [ -d ./build/install ]; then
    rm -rf ./build/install
fi

# 使用Ninja加速编译
cmake -B build -G Ninja $@

cmake --build build --config Release --parallel $(nproc)
```

编译构建脚本就是将`CMake`的构建和编译指令合到一起了，这与直接使用`CMake`指令快速验证效果是一样的。

---

`setup.sh`

```shell
#!/bin/bash
PROJRCT_PATH="$(pwd)"
AIMRT_PATH="$(dirname $(pwd))"

# 项目的构建目录
PROJECT_BUILD_PATH="${PROJRCT_PATH}/build"
# 项目的cfg文件目录
CONFIG_FILE_PATH="${PROJRCT_PATH}/src/install/linux/cfg"
# aimrt_main目录
# AIMRT_MAIN_FILE="${PROJRCT_PATH}/build/_deps/aimrt-build/src/runtime/main/aimrt_main"
# 将aimrt_main拷贝到当前项目的构建目录
# cp ${AIMRT_MAIN_FILE} ${PROJECT_BUILD_PATH}

# 定义aimrt_main临时环境变量（可选）
# export AIMRT_MAIN="${PROJECT_BUILD_PATH}/aimrt_main"
```

为了优雅的启动`AimRT`，这个脚本做了很多改动，不过后来基本都没用上，在搞清楚一些`CMake`配置后便只保留了`CONFIG_FILE_PATH`环境变量，在终端`$CONFIG_FILE_PATH`便可以替换大串路径。

> 注意，在 **多个项目融合实践** 中这个脚本已经彻底被取代，转而使用`CMake`程序将`yaml`拷贝到`build`目录，使得路径简单且固定（这一点和官方源码一致）。

---

`app_start.sh`

```shell
#!/bin/bash
PROJRCT_PATH="$(pwd)"
cd ${PROJRCT_PATH}/build/

./$1 $2
```

`pkg_start.sh`

```shell
#!/bin/bash
PROJRCT_PATH="$(pwd)"
cd ${PROJRCT_PATH}/build/


./aimrt_main --cfg_file_path="$1"
```

两个启动脚本使用的思想一致：切换到`build`目录，然后启动对于的可执行文件，`yaml`文件路径通过终端参数传递。

> `$1`为第一个参数，`$0`是`.sh`文件本身，依次类推。

## WorkSpaceExample

本案例可以理解为是多项目融合实践，因为它可以管理多个单独项目，一起编译或单独编译（单独编译还没做，不过官方源码给了很好的示例）。

> 官方并没有此称呼，这是个人随便起的😂。官方文档示例中，无论手动搭建或使用`CLI`自动搭建，结果均是一个单独的项目，类似上面的`HelloWorld`示例（差别仅`CMake`构建脚本更复杂规范）。不过，`AimRT`的源码中的`Example`是使用这种方法搭建的，这也是我这样搭建开发环境的原因之一。

本案例唯一参考[AimRT源码](https://github.com/AimRT/AimRT)。

### 如何使用

与`HelloWorld`使用方法一样，不同点在于不再需要`setup.sh`。

### CMake命名空间

`WorkSpaceExample`的目录如下：

```
├── CMakeLists.txt
├── app_start.sh
├── build.sh
├── cmake
│   └── GetAimRT.cmake
├── pkg_start.sh
└── src
    ├── CMakeLists.txt
    ├── executor
    └── helloworld
        ├── CMakeLists.txt
        ├── app
        ├── install
        ├── module
        └── pkg
```

`src`为父级命名空间，固定为`aimrt`：

```cmake
set_root_namespace("aimrt")
```

`src`下每一个子项目如`helloworld`，均设置一个以文件夹命名的子级命名空间：

```cmake
set_namespace()
```

子项目`helloworld`下的`CMakeLists.txt`及其下级`CMakeLists.txt`开始使用命名空间，具体如下：

- 获取当前文件夹名称

```cmake
string(REGEX REPLACE ".*/\(.*\)" "\\1" CUR_DIR ${CMAKE_CURRENT_SOURCE_DIR})
```

- 获取当前目录的父级命名空间（不含当前）

```cmake
get_namespace(CUR_SUPERIOR_NAMESPACE)
```

- 将命名空间的 "::" 换成 "_"

```cmake
string(REPLACE "::" "_" CUR_SUPERIOR_NAMESPACE_UNDERLINE ${CUR_SUPERIOR_NAMESPACE})
```

> 重点关注三个变量`CUR_DIR`、`CUR_SUPERIOR_NAMESPACE`、`CUR_SUPERIOR_NAMESPACE_UNDERLINE`，若是在`helloworld`根目录下的`CMakeLists.txt`，则：
>
> - `${CUR_DIR}=helloworld`
>
> - `${CUR_SUPERIOR_NAMESPACE}=aimrt::helloworld` 
>
> - `${CUR_SUPERIOR_NAMESPACE_UNDERLINE}=aimrt_helloworld`

在子项目下的文件夹`app`、`pkg`、`module`，分别需要编译成可执行文件、动态库、静态库。因此基于命名空间会进一步做以下操作（以可执行文件为例）：

```cmake
# 设置静态库/动态库/可执行文件名称，"_"和"::"两种形式
set(CUR_TARGET_NAME ${CUR_SUPERIOR_NAMESPACE_UNDERLINE}_${CUR_DIR})
set(CUR_TARGET_ALIAS_NAME ${CUR_SUPERIOR_NAMESPACE}::${CUR_DIR})

# 定义一个静态库/动态库/可执行文件目标
add_library(${CUR_TARGET_NAME} SHARED)
# 建立一个静态库/动态库/可执行文件的别名
add_library(${CUR_TARGET_ALIAS_NAME} ALIAS ${CUR_TARGET_NAME})

# 重命名可执行文件/动态库的输出名称
set_target_properties(${CUR_TARGET_NAME} PROPERTIES OUTPUT_NAME ${CUR_DIR})
```

### 使用的CMake技巧

在工作空间根目录的`CMakeLists.txt`中，修改默认的库文件、存档文件的存放位置，这样`aimrt_main`等可执行文件、pkg模式生成的`.so`都会放在构建目录的根位置。

```cmake
# 修改默认的库文件、存档文件的存放位置
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
```

在每个子项目的`CMakeLists.txt`中，做资源文件（主要是`yaml`文件）的拷贝，这样`yaml`文件也会放在构建目录的根位置。

```cmake
add_custom_target(
  ${CUR_SUPERIOR_NAMESPACE_UNDERLINE}_${CUR_DIR}_build_all ALL
  COMMAND ${CMAKE_COMMAND} -E copy_directory ${CUR_INSTALL_SOURCE_DIR} ${CMAKE_BINARY_DIR}
  DEPENDS aimrt::runtime::main
          ${CUR_SUPERIOR_NAMESPACE}::${CUR_DIR}::helloworld_app
          ${CUR_SUPERIOR_NAMESPACE}::${CUR_DIR}::helloworld_pkg)
```

### 脚本解读

与`HelloWorld`的脚本区别不大：

- 取消`setup.sh`
- 修改启动脚本的`yaml`文件路径