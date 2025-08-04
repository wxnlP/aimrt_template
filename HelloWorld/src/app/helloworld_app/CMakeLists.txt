# 定义一个变量src，包含当前目录下的.cc文件
file(GLOB src ${CMAKE_CURRENT_SOURCE_DIR}/*.cc)

add_executable(helloworld_app)

target_sources(helloworld_app PRIVATE
    ${src}
)

target_include_directories(helloworld_app PRIVATE
    # /src/app/helloworld_app/
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(
    helloworld_app
    PRIVATE aimrt::runtime::core
    helloworld::helloworld_module
)