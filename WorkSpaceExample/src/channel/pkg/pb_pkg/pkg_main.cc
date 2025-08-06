#include <cstring>

#include "aimrt_pkg_c_interface/pkg_macro.h"
#include "pb_publisher_module/pb_publisher_module.h"
#include "pb_subscriber_module/pb_subscriber_module.h"

static std::tuple<std::string_view, std::function<aimrt::ModuleBase*()>>
    aimrt_module_register_array[]{
        {"PbPublisherModule",
         []() -> aimrt::ModuleBase* {
           return new example::channel::pb_publisher_module::PbPublisherModule();
         }},
        {"PbSubscriberModule",
         []() -> aimrt::ModuleBase* {
           return new example::channel::pb_subscriber_module::PbSubscriberModule();
         }},
    };

AIMRT_PKG_MAIN(aimrt_module_register_array)
