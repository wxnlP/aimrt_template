#pragma once

#include "aimrt_module_cpp_interface/module_base.h"
#include "header.pb.h"

namespace example::channel::pb_subscriber_module {

class PbSubscriberModule : public aimrt::ModuleBase {
public:
    PbSubscriberModule() = default;
    ~PbSubscriberModule() override = default;

    aimrt::ModuleInfo Info() const override {
        return aimrt::ModuleInfo{.name = "PbSubscriberModule"};
    }

    bool Initialize(aimrt::CoreRef core) override;
    
    bool Start() override;

    void Shutdown() override;

private:
    auto GetLogger() { return core_.GetLogger(); }
    void SubscriberCallback(const std::shared_ptr<const aimrt::protocols::common::Header>& msg);

private:
    aimrt::CoreRef core_;
    aimrt::channel::SubscriberRef subscriber_;

    std::string topic_name_;
};

}  // namespace example::channel::pb_subscriber_module
