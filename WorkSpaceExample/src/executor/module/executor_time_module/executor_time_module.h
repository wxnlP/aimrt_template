#pragma once

#include "aimrt_module_cpp_interface/module_base.h"
#include "aimrt_module_cpp_interface/executor/timer.h"


namespace example::executor::executor_time_module {

class ExecutorTimeModule : public aimrt::ModuleBase {
public:
    // 使用默认的构造函数和析构函数
    ExecutorTimeModule() = default;
    ~ExecutorTimeModule() override = default;

    // aimrt是一个工作空间
    // AimRT加载模块时调用此函数读取模块信息
    aimrt::ModuleInfo Info() const override {
        // 除 name 外，其他参数为非必须参数
        return aimrt::ModuleInfo{.name = "ExecutorTimeModule"};
    }

    bool Initialize(aimrt::CoreRef core) override;
    bool Start() override;
    void Shutdown() override;

private:
    // 日志配置
    auto GetLogger() {
        return core_.GetLogger();
    }


private:
    aimrt::CoreRef core_;

    aimrt::executor::ExecutorRef timer_executor_;
    std::shared_ptr<aimrt::executor::TimerBase> timer_;
};

} // namespace example::executor::executor_time_module