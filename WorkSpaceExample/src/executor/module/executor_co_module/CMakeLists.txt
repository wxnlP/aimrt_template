# 获取当前文件夹的名称
string(REGEX REPLACE ".*/\(.*\)" "\\1" CUR_DIR ${CMAKE_CURRENT_SOURCE_DIR})

# 获取当前目录的父级命名空间（不含当前）
get_namespace(CUR_SUPERIOR_NAMESPACE)
# 将命名空间的 "::" 换成 "_"
string(REPLACE "::" "_" CUR_SUPERIOR_NAMESPACE_UNDERLINE ${CUR_SUPERIOR_NAMESPACE})

# 设置静态库名称，"_"和"::"两种形式
set(CUR_TARGET_NAME ${CUR_SUPERIOR_NAMESPACE_UNDERLINE}_${CUR_DIR})
set(CUR_TARGET_ALIAS_NAME ${CUR_SUPERIOR_NAMESPACE}::${CUR_DIR})


# 定义一个静态库目标
add_library(${CUR_TARGET_NAME} STATIC)
# 建立了一个带 namespace 的别名
add_library(${CUR_TARGET_ALIAS_NAME} ALIAS ${CUR_TARGET_NAME})

target_sources(${CUR_TARGET_NAME} PRIVATE
    executor_co_module.cc
)

target_include_directories(${CUR_TARGET_NAME} PUBLIC
    # /src/module/
    ${CMAKE_CURRENT_SOURCE_DIR}/..
)

target_link_libraries(
    ${CUR_TARGET_NAME}
    PRIVATE yaml-cpp::yaml-cpp
    PRIVATE aimrt::common::util
    PUBLIC aimrt::interface::aimrt_module_cpp_interface
)
