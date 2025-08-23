#pragma once

#include "aimrt_module_cpp_interface/module_base.h"

namespace example::channel::pb_publisher_module {

class PbPublisherModule : public aimrt::ModuleBase {
public:
    PbPublisherModule() = default;
    ~PbPublisherModule() override = default;

    aimrt::ModuleInfo Info() const override {
        return aimrt::ModuleInfo{.name = "PbPublisherModule"};
    }

    bool Initialize(aimrt::CoreRef core) override;

    bool Start() override;

    void Shutdown() override;

private:
    auto GetLogger() { return core_.GetLogger(); }
    void MainLoop();

private:
    aimrt::CoreRef core_;
    aimrt::executor::ExecutorRef pub_executor_;
    aimrt::channel::PublisherRef publisher_;

    std::string topic_name_;
    double channel_frq_;
    bool run_flag_ = false;
    std::promise<void> stop_sig_;
    
};

}  // namespace example::channel::pb_publisher_module
