#include "aimrt_pkg_c_interface/pkg_macro.h"
#include "parameter_module/parameter_module.h"

using namespace example::parameter::parameter_module;

static std::tuple<std::string_view, std::function<aimrt::ModuleBase*()>> aimrt_module_register_array[]{
    {"ParameterModule", []() -> aimrt::ModuleBase* { return new ParameterModule(); }}
};

AIMRT_PKG_MAIN(aimrt_module_register_array);