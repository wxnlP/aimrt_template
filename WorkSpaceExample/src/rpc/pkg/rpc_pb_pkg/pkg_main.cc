#include <cstring>

#include "aimrt_pkg_c_interface/pkg_macro.h"
#include "pb_client_module/pb_client_module.h"
#include "pb_server_module/pb_server_module.h"

static std::tuple<std::string_view, std::function<aimrt::ModuleBase*()>>
    aimrt_module_register_array[]{
        {"PbClientModule",
         []() -> aimrt::ModuleBase* {
           return new example::rpc::pb_client_module::PbClientModule();
         }},
        {"PbServerModule",
         []() -> aimrt::ModuleBase* {
           return new example::rpc::pb_server_module::PbServerModule();
         }},
    };

AIMRT_PKG_MAIN(aimrt_module_register_array)
