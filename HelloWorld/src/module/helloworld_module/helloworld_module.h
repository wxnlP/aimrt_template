#pragma once

#include "aimrt_module_cpp_interface/module_base.h"

class HelloWorldModule : public aimrt::ModuleBase {
public:
    // 使用默认的构造函数和析构函数
    HelloWorldModule() = default;
    ~HelloWorldModule() override = default;

    // aimrt是一个工作空间
    // AimRT加载模块时调用此函数读取模块信息
    aimrt::ModuleInfo Info() const override {
        // 除 name 外，其他参数为非必须参数
        return aimrt::ModuleInfo{.name = "HelloWorldModule"};
    }

    bool Initialize(aimrt::CoreRef core) override;
    bool Start() override;
    void Shutdown() override;

private:
    aimrt::CoreRef core_;
};
