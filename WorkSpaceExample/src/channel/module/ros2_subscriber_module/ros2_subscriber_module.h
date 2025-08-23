#pragma once

#include "aimrt_module_cpp_interface/module_base.h"
#include "std_msgs/msg/string.hpp"

namespace example::channel::ros2_subscriber_module
{

class Ros2SubscriberModule : public aimrt::ModuleBase
{
public:
    Ros2SubscriberModule() = default;
    ~Ros2SubscriberModule() override = default;

    aimrt::ModuleInfo Info() const override
    {
        return aimrt::ModuleInfo{.name = "Ros2SubscriberModule"};
    }

    bool Initialize(aimrt::CoreRef core) override;

    bool Start() override;

    void Shutdown() override;


private:
    auto GetLogger() { return core_.GetLogger(); }
    void SubscriberCallback(const std::shared_ptr<const std_msgs::msg::String>& msg);


private:
    aimrt::CoreRef core_;
    aimrt::channel::SubscriberRef subscriber_;
    std::string topic_name_;
};

} // namespace example::channel::ros2_subscriber_module
