#pragma once

#include "aimrt_module_cpp_interface/module_base.h"

namespace example::chn_protocols::ros2_pub_module {

class Ros2PubModule : public aimrt::ModuleBase {
   public:
    Ros2PubModule() = default;
    ~Ros2PubModule() override = default;

    aimrt::ModuleInfo Info() const override {
        return aimrt::ModuleInfo{.name = "Ros2PubModule"};
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

}  // namespace example::chn_protocols::ros2_pub_module
