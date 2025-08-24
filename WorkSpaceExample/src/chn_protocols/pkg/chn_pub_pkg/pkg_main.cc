#include <cstring>

#include "aimrt_pkg_c_interface/pkg_macro.h"
#include "chn_publisher_module/chn_publisher_module.h"

static std::tuple<std::string_view, std::function<aimrt::ModuleBase*()>>
    aimrt_module_register_array[]{
        {"ChnPublisherModule",
         []() -> aimrt::ModuleBase* {
           return new example::chn_protocols::chn_publisher_module::ChnPublisherModule();
         }},
    };

AIMRT_PKG_MAIN(aimrt_module_register_array)
