#include <cstring>

#include "aimrt_pkg_c_interface/pkg_macro.h"
#include "pb_publisher_module/pb_publisher_module.h"
#include "ros2_publisher_module/ros2_publisher_module.h"

static std::tuple<std::string_view, std::function<aimrt::ModuleBase*()>>
    aimrt_module_register_array[]{
        {"PbPublisherModule",
         []() -> aimrt::ModuleBase* {
           return new example::channel::pb_publisher_module::PbPublisherModule();
         }},
        {"Ros2PublisherModule",
         []() -> aimrt::ModuleBase* {
           return new example::channel::ros2_publisher_module::Ros2PublisherModule();
         }},
    };

AIMRT_PKG_MAIN(aimrt_module_register_array)
