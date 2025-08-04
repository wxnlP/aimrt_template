# 定义一个静态库目标
add_library(helloworld_module STATIC)
# 建立了一个带 namespace 的别名
add_library(helloworld::helloworld_module ALIAS helloworld_module)

target_sources(helloworld_module PRIVATE
    helloworld_module.cc
)

target_include_directories(helloworld_module PUBLIC
    # /src/module/
    ${CMAKE_CURRENT_SOURCE_DIR}/..
)

target_link_libraries(
    helloworld_module
    PRIVATE yaml-cpp::yaml-cpp
    PRIVATE aimrt::common::util
    PUBLIC aimrt::interface::aimrt_module_cpp_interface
)
