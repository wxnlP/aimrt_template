#include <cstring>

#include "aimrt_pkg_c_interface/pkg_macro.h"
#include "pb_sub_module/pb_sub_module.h"
#include "ros2_sub_module/ros2_sub_module.h"

static std::tuple<std::string_view, std::function<aimrt::ModuleBase*()>>
    aimrt_module_register_array[]{
        {"PbSubModule",
         []() -> aimrt::ModuleBase* {
           return new example::chn_protocols::pb_sub_module::PbSubModule();
         }},
        {"Ros2SubModule",
         []() -> aimrt::ModuleBase* {
           return new example::chn_protocols::ros2_sub_module::Ros2SubModule();
         }},
    };

AIMRT_PKG_MAIN(aimrt_module_register_array)
