# Set namespace
set_namespace()



# module
add_subdirectory(module/pb_pub_module)
add_subdirectory(module/pb_sub_module)
add_subdirectory(module/ros2_pub_module)
add_subdirectory(module/ros2_sub_module)

# pkg
add_subdirectory(pkg/pb_col_pkg)
add_subdirectory(pkg/ros2_col_pkg)
add_subdirectory(pkg/pub_pkg)
add_subdirectory(pkg/sub_pkg)

# install
if(CMAKE_SYSTEM_NAME MATCHES "Linux")
  set(CUR_INSTALL_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/install/linux)
elseif(CMAKE_SYSTEM_NAME MATCHES "Windows")
  set(CUR_INSTALL_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/install/win)
else()
  message(FATAL_ERROR "Unsupport os")
endif()

string(REGEX REPLACE ".*/\(.*\)" "\\1" CUR_DIR ${CMAKE_CURRENT_SOURCE_DIR})
# 获取当前目录的父级命名空间（不含当前）
get_namespace(CUR_SUPERIOR_NAMESPACE)
# 将命名空间的 "::" 换成 "_"
string(REPLACE "::" "_" CUR_SUPERIOR_NAMESPACE_UNDERLINE ${CUR_SUPERIOR_NAMESPACE})

# build all
add_custom_target(
  chn_protocols_build_all ALL
  COMMAND ${CMAKE_COMMAND} -E copy_directory ${CUR_INSTALL_SOURCE_DIR}/bin ${CMAKE_BINARY_DIR}
  DEPENDS aimrt::runtime::main
          ${CUR_SUPERIOR_NAMESPACE}::${CUR_DIR}::pb_col_pkg
          ${CUR_SUPERIOR_NAMESPACE}::${CUR_DIR}::ros2_col_pkg
          ${CUR_SUPERIOR_NAMESPACE}::${CUR_DIR}::pub_pkg
          ${CUR_SUPERIOR_NAMESPACE}::${CUR_DIR}::sub_pkg)


