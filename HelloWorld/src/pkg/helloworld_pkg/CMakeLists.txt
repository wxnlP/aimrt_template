# Set target name
set(CUR_TARGET_NAME helloworld_pkg)

$
# Add target
add_library(${CUR_TARGET_NAME} SHARED)

# Set source file of target
target_sources(${CUR_TARGET_NAME} PRIVATE 
  pkg_main.cc
)

# Set link libraries of target
target_link_libraries(
  ${CUR_TARGET_NAME}
  PRIVATE aimrt::interface::aimrt_pkg_c_interface
          helloworld::helloworld_module
)
