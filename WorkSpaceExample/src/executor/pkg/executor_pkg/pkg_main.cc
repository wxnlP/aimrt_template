#include "aimrt_pkg_c_interface/pkg_macro.h"
#include "executor_module/executor_module.h"
#include "executor_co_module/executor_co_module.h"

using namespace example::executor::executor_module;
using namespace example::executor::executor_co_module;

static std::tuple<std::string_view, std::function<aimrt::ModuleBase*()>> aimrt_module_register_array[]{
    {"ExecutorModule", []() -> aimrt::ModuleBase* { return new ExecutorModule(); }},
    {"ExecutorCoModule", []() -> aimrt::ModuleBase* { return new ExecutorCoModule(); }}
};

AIMRT_PKG_MAIN(aimrt_module_register_array);