#pragma once

#include "aimrt_module_cpp_interface/module_base.h"

namespace example::executor::executor_module {

class ExecutorModule : public aimrt::ModuleBase {
public:
    // 使用默认的构造函数和析构函数
    ExecutorModule() = default;
    ~ExecutorModule() override = default;

    // aimrt是一个工作空间
    // AimRT加载模块时调用此函数读取模块信息
    aimrt::ModuleInfo Info() const override {
        // 除 name 外，其他参数为非必须参数
        return aimrt::ModuleInfo{.name = "ExecutorModule"};
    }

    bool Initialize(aimrt::CoreRef core) override;
    bool Start() override;
    void Shutdown() override;

private:
    // 日志配置
    auto GetLogger() {
        return core_.GetLogger();
    }

    void SimpleExecutorDemo();
    void ThreadSafeExecutorDemo();
    void TimeScheduleExecutorDemo();

private:
    aimrt::CoreRef core_;

    // 基础执行器
    aimrt::executor::ExecutorRef work_executor_;
    // 线程安全执行器
    aimrt::executor::ExecutorRef thread_safe_executor_;
    // 时间调度执行器
    aimrt::executor::ExecutorRef time_schedule_executor_;
    bool run_flag_ = true;
    uint32_t loop_count_ = 0;
};

} // namespace example::executor::executor_module