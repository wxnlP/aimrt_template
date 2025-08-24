#pragma once

#include "aimrt_module_cpp_interface/module_base.h"
#include "info.pb.h"

namespace example::chn_protocols::chn_subscriber_module {

class ChnSubscriberModule : public aimrt::ModuleBase {
   public:
    ChnSubscriberModule() = default;
    ~ChnSubscriberModule() override = default;

    aimrt::ModuleInfo Info() const override {
        return aimrt::ModuleInfo{.name = "ChnSubscriberModule"};
    }

    bool Initialize(aimrt::CoreRef core) override;

    bool Start() override;

    void Shutdown() override;

   private:
    auto GetLogger() { return core_.GetLogger(); }
    void PbSubscriberCallback(
        const std::shared_ptr<const example::protocols::test::Info>& msg);

   private:
    aimrt::CoreRef core_;
    aimrt::channel::SubscriberRef subscriber_pb_;
    aimrt::channel::SubscriberRef subscriber_ros2_;

    std::string topic_name_;
};

}  // namespace example::chn_protocols::chn_subscriber_module
