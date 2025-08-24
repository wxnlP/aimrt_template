#include <cstring>

#include "aimrt_pkg_c_interface/pkg_macro.h"
#include "chn_subscriber_module/chn_subscriber_module.h"

static std::tuple<std::string_view, std::function<aimrt::ModuleBase*()>>
    aimrt_module_register_array[]{
        {"ChnSubscriberModule",
         []() -> aimrt::ModuleBase* {
           return new example::chn_protocols::chn_subscriber_module::ChnSubscriberModule();
         }},
    };

AIMRT_PKG_MAIN(aimrt_module_register_array)
