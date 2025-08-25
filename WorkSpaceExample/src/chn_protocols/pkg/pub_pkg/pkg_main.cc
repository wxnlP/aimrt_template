#include <cstring>

#include "aimrt_pkg_c_interface/pkg_macro.h"
#include "pb_pub_module/pb_pub_module.h"
#include "ros2_pub_module/ros2_pub_module.h"

static std::tuple<std::string_view, std::function<aimrt::ModuleBase*()>>
    aimrt_module_register_array[]{
        {"PbPubModule",
         []() -> aimrt::ModuleBase* {
           return new example::chn_protocols::pb_pub_module::PbPubModule();
         }},
        {"Ros2PubModule",
         []() -> aimrt::ModuleBase* {
           return new example::chn_protocols::ros2_pub_module::Ros2PubModule();
         }},
    };

AIMRT_PKG_MAIN(aimrt_module_register_array)
