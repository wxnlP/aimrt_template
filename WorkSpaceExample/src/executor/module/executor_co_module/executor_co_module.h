#pragma once

#include "aimrt_module_cpp_interface/module_base.h"
#include "aimrt_module_cpp_interface/co/task.h"
#include "aimrt_module_cpp_interface/co/aimrt_context.h"
#include "aimrt_module_cpp_interface/co/async_scope.h"

namespace example::executor::executor_co_module {

class ExecutorCoModule : public aimrt::ModuleBase {
public:
    // 使用默认的构造函数和析构函数
    ExecutorCoModule() = default;
    ~ExecutorCoModule() override = default;

    // aimrt是一个工作空间
    // AimRT加载模块时调用此函数读取模块信息
    aimrt::ModuleInfo Info() const override {
        // 除 name 外，其他参数为非必须参数
        return aimrt::ModuleInfo{.name = "ExecutorCoModule"};
    }

    bool Initialize(aimrt::CoreRef core) override;
    bool Start() override;
    void Shutdown() override;

private:
    // 日志配置
    auto GetLogger() {
        return core_.GetLogger();
    }

    aimrt::co::Task<void>SimpleExecutorDemo();
    aimrt::co::Task<void>ThreadSafeExecutorDemo();
    aimrt::co::Task<void>TimeScheduleExecutorDemo();

private:
    aimrt::CoreRef core_;
    
    aimrt::co::AimRTContext ctx_;
    aimrt::co::AsyncScope scope_;
    bool run_flag_ = true;
};

} // namespace example::executor::executor_co_module