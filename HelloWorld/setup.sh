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

