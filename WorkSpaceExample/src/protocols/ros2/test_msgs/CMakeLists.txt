cmake_minimum_required(VERSION 3.8)

# Get the current folder name
string(REGEX REPLACE ".*/\(.*\)" "\\1" CUR_DIR ${CMAKE_CURRENT_SOURCE_DIR})

# Get namespace
get_namespace(CUR_SUPERIOR_NAMESPACE)
string(REPLACE "::" "_" CUR_SUPERIOR_NAMESPACE_UNDERLINE ${CUR_SUPERIOR_NAMESPACE})

# Set target name
set(CUR_TARGET_NAME ${CUR_DIR})

project(${CUR_TARGET_NAME})

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rosidl_default_generators REQUIRED)

set(CUR_BUILD_SHARED_LIBS ${BUILD_SHARED_LIBS})
set(BUILD_SHARED_LIBS ON)

# 注册消息接口
rosidl_generate_interfaces(
  ${PROJECT_NAME}
  "msg/Info.msg" 
)
set(BUILD_SHARED_LIBS ${CUR_BUILD_SHARED_LIBS})


if(NOT TARGET ${CUR_TARGET_NAME}::${CUR_TARGET_NAME}__rosidl_typesupport_cpp)
  add_library(${CUR_TARGET_NAME}::${CUR_TARGET_NAME}__rosidl_typesupport_cpp ALIAS ${CUR_TARGET_NAME}__rosidl_typesupport_cpp)
endif()

if(NOT TARGET ${CUR_TARGET_NAME}::${CUR_TARGET_NAME}__rosidl_typesupport_fastrtps_cpp)
  add_library(${CUR_TARGET_NAME}::${CUR_TARGET_NAME}__rosidl_typesupport_fastrtps_cpp ALIAS ${CUR_TARGET_NAME}__rosidl_typesupport_fastrtps_cpp)
endif()

ament_export_dependencies(rosidl_default_runtime)
ament_package()
