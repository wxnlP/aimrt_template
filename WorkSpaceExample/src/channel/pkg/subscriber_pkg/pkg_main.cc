#include <cstring>

#include "aimrt_pkg_c_interface/pkg_macro.h"
#include "pb_subscriber_module/pb_subscriber_module.h"
#include "ros2_subscriber_module/ros2_subscriber_module.h"

static std::tuple<std::string_view, std::function<aimrt::ModuleBase*()>>
    aimrt_module_register_array[]{
        {"PbSubscriberModule",
         []() -> aimrt::ModuleBase* {
           return new example::channel::pb_subscriber_module::PbSubscriberModule();
         }},
        {"Ros2SubscriberModule",
         []() -> aimrt::ModuleBase* {
           return new example::channel::ros2_subscriber_module::Ros2SubscriberModule();
         }},
    };

AIMRT_PKG_MAIN(aimrt_module_register_array)
