#include <cstring>

#include "aimrt_pkg_c_interface/pkg_macro.h"
#include "ros2_client_module/ros2_client_module.h"
#include "ros2_server_module/ros2_server_module.h"

static std::tuple<std::string_view, std::function<aimrt::ModuleBase*()>>
    aimrt_module_register_array[]{
        {"Ros2ClientModule",
         []() -> aimrt::ModuleBase* {
           return new example::rpc::ros2_client_module::Ros2ClientModule();
         }},
        {"Ros2ServerModule",
         []() -> aimrt::ModuleBase* {
           return new example::rpc::ros2_server_module::Ros2ServerModule();
         }},
    };

AIMRT_PKG_MAIN(aimrt_module_register_array)
