# 获取当前文件夹的名称
string(REGEX REPLACE ".*/\(.*\)" "\\1" CUR_DIR ${CMAKE_CURRENT_SOURCE_DIR})

# 获取当前目录的父级命名空间（不含当前）
get_namespace(CUR_SUPERIOR_NAMESPACE)
# 将命名空间的 "::" 换成 "_"
string(REPLACE "::" "_" CUR_SUPERIOR_NAMESPACE_UNDERLINE ${CUR_SUPERIOR_NAMESPACE})

# 设置可执行文件名称，"_"和"::"两种形式
set(CUR_TARGET_NAME ${CUR_SUPERIOR_NAMESPACE_UNDERLINE}_${CUR_DIR})
set(CUR_TARGET_ALIAS_NAME ${CUR_SUPERIOR_NAMESPACE}::${CUR_DIR})

# 获取源文件集合
file(GLOB_RECURSE src *.cc)

# 实体可执行文件
add_executable(${CUR_TARGET_NAME})
# 为可执行文件起别名
add_executable(${CUR_TARGET_ALIAS_NAME} ALIAS ${CUR_TARGET_NAME})


target_sources(${CUR_TARGET_NAME} PRIVATE
    ${src}
)

target_include_directories(${CUR_TARGET_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(
    ${CUR_TARGET_NAME}
    PRIVATE aimrt::runtime::core
            ${CUR_SUPERIOR_NAMESPACE}::helloworld_module
)

# 重命名可执行文件的输出名称
set_target_properties(${CUR_TARGET_NAME} PROPERTIES OUTPUT_NAME ${CUR_DIR})