#!/bin/bash
if [ $# -eq 0 ]; then
    echo "err 未提供参数！"
    exit 1
elif [ $# -lt 2 ]; then
    echo "err 至少需要2个参数！"
    exit 1
fi

PJ_NAME=${1#./}
WK_SPACE=${2#./}

echo "开始工作空间转移......"

if [[ "$3" != "-1" && "$3" != "-a" ]]; then
    mkdir -p ${WK_SPACE}/src/${PJ_NAME}
    echo "完成工程目录创建."
else
    echo "跳过工程目录创建!"
fi

if [[ "$3" != "-2" && "$3" != "-a" ]]; then
    mv ${PJ_NAME}/src/* ${WK_SPACE}/src/${PJ_NAME}/
    rm -rf ${PJ_NAME}
    echo "完成文件转移."
else
    echo "跳过文件转移!"
fi

cd ${WK_SPACE}/src/${PJ_NAME}/
sed -i "2s/set_root_namespace(\"${PJ_NAME}\")/set_namespace()/" CMakeLists.txt
cd ../
sed -i "\$a add_subdirectory(${PJ_NAME})" CMakeLists.txt

echo "完成CMake配置修改."

echo "完成工作空间转移."
