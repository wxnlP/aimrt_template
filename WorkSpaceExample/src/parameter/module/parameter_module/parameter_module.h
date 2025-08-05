#pragma once

#include "aimrt_module_cpp_interface/module_base.h"

namespace example::parameter::parameter_module {

class ParameterModule : public aimrt::ModuleBase {
public:
    // 使用默认的构造函数和析构函数
    ParameterModule() = default;
    ~ParameterModule() override = default;

    // aimrt是一个工作空间
    // AimRT加载模块时调用此函数读取模块信息
    aimrt::ModuleInfo Info() const override {
        // 除 name 外，其他参数为非必须参数
        return aimrt::ModuleInfo{.name = "ParameterModule"};
    }

    bool Initialize(aimrt::CoreRef core) override;
    bool Start() override;
    void Shutdown() override;

private:
    auto GetLogger() { return core_.GetLogger(); }

    void SetParameterLoop();
    void GetParameterLoop();

private:
    aimrt::CoreRef core_;

    aimrt::executor::ExecutorRef param_executor_;
    aimrt::parameter::ParameterHandleRef param_handle_;

    bool run_flag_ = true;
    // 用于标记就绪态，记录循环结束
    std::promise<void> set_loop_stop_sig_;
    std::promise<void> get_loop_stop_sig_;
};

}