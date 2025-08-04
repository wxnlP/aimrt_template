#include "aimrt_pkg_c_interface/pkg_macro.h"
#include "helloworld_module/helloworld_module.h"

using namespace aimrt::helloworld::helloworld_module;

static std::tuple<std::string_view, std::function<aimrt::ModuleBase*()>> aimrt_module_register_array[]{
    {"HelloWorldModule", []() -> aimrt::ModuleBase* { return new HelloWorldModule(); }}
};

AIMRT_PKG_MAIN(aimrt_module_register_array);