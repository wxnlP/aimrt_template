# Get the current folder name
string(REGEX REPLACE ".*/\(.*\)" "\\1" CUR_DIR ${CMAKE_CURRENT_SOURCE_DIR})

# 设置命名空间
set_namespace()

# 设置子构建目录
add_subdirectory(module/executor_module)
add_subdirectory(module/executor_co_module)
add_subdirectory(module/executor_time_module)
add_subdirectory(pkg/executor_pkg)

set(CUR_INSTALL_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/install/linux)

# 获取当前目录的父级命名空间（不含当前）
get_namespace(CUR_SUPERIOR_NAMESPACE)
# 将命名空间的 "::" 换成 "_"
string(REPLACE "::" "_" CUR_SUPERIOR_NAMESPACE_UNDERLINE ${CUR_SUPERIOR_NAMESPACE})
# 做资源文件拷贝
add_custom_target(
  ${CUR_SUPERIOR_NAMESPACE_UNDERLINE}_${CUR_DIR}_build_all ALL
  COMMAND ${CMAKE_COMMAND} -E copy_directory ${CUR_INSTALL_SOURCE_DIR} ${CMAKE_BINARY_DIR}
  DEPENDS aimrt::runtime::main
          # ${CUR_SUPERIOR_NAMESPACE}::${CUR_DIR}::executor_app
          ${CUR_SUPERIOR_NAMESPACE}::${CUR_DIR}::executor_pkg)
