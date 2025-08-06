#include <cstring>

#include "aimrt_pkg_c_interface/pkg_macro.h"
#include "ros2_publisher_module/ros2_publisher_module.h"
#include "ros2_subscriber_module/ros2_subscriber_module.h"

static std::tuple<std::string_view, std::function<aimrt::ModuleBase*()>>
    aimrt_module_register_array[]{
        {"Ros2PublisherModule",
         []() -> aimrt::ModuleBase* {
           return new example::channel::ros2_publisher_module::Ros2PublisherModule();
         }},
        {"Ros2SubscriberModule",
         []() -> aimrt::ModuleBase* {
           return new example::channel::ros2_subscriber_module::Ros2SubscriberModule();
         }},
    };

AIMRT_PKG_MAIN(aimrt_module_register_array)
