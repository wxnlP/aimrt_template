#pragma once

#include "aimrt_module_cpp_interface/module_base.h"

namespace example::channel::ros2_publisher_module
{

class Ros2PublisherModule : public aimrt::ModuleBase
{
public:
    Ros2PublisherModule() = default;
    ~Ros2PublisherModule() override = default;

    aimrt::ModuleInfo Info() const override
    {
        return aimrt::ModuleInfo{.name = "Ros2PublisherModule"};
    }

    bool Initialize(aimrt::CoreRef core) override;

    bool Start() override;

    void Shutdown() override;

private:
    auto GetLogger() { return core_.GetLogger(); }

    void MainLoop();

private:
    aimrt::CoreRef core_;
    aimrt::channel::PublisherRef publisher_;
    aimrt::executor::ExecutorRef pub_executor_;

    bool run_flag_ = false;
    std::promise<void> stop_sig_;
    double channel_frq_;
    std::string channel_name_;
};

} // namespace example::channel::ros2_publisher_module
