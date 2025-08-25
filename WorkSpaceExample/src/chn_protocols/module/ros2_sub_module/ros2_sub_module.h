#pragma once

#include "aimrt_module_cpp_interface/module_base.h"
#include "test_msgs/msg/info.hpp"

namespace example::chn_protocols::ros2_sub_module {

class Ros2SubModule : public aimrt::ModuleBase {
 public:
  Ros2SubModule() = default;
  ~Ros2SubModule() override = default;

  aimrt::ModuleInfo Info() const override {
    return aimrt::ModuleInfo{.name = "Ros2SubModule"};
  }

  bool Initialize(aimrt::CoreRef core) override;

  bool Start() override;

  void Shutdown() override;

 private:
  auto GetLogger() { return core_.GetLogger(); }
  void SubscriberCallback(const std::shared_ptr<const test_msgs::msg::Info>& msg);

 private:
  aimrt::CoreRef core_;
  aimrt::channel::SubscriberRef subscriber_;

  std::string topic_name_;
};

}  // namespace example::chn_protocols::ros2_sub_module
