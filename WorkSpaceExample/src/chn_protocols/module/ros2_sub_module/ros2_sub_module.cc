#include "ros2_sub_module/ros2_sub_module.h"
#include "aimrt_module_ros2_interface/channel/ros2_channel.h"
#include "yaml-cpp/yaml.h"

namespace example::chn_protocols::ros2_sub_module {

bool Ros2SubModule::Initialize(aimrt::CoreRef core) {
  // Save aimrt framework handle
  core_ = core;

  try {
    // Read cfg
    auto file_path = core_.GetConfigurator().GetConfigFilePath();
    if (!file_path.empty()) {
      YAML::Node cfg_node = YAML::LoadFile(std::string(file_path));
      topic_name_ = cfg_node["channel_name"].as<std::string>();
    }

    subscriber_ = core_.GetChannelHandle().GetSubscriber(topic_name_);
    AIMRT_CHECK_ERROR_THROW(subscriber_, "Get subscriber failed.");

    bool ret = aimrt::channel::Subscribe<test_msgs::msg::Info>(
        subscriber_,
        std::bind(&Ros2SubModule::SubscriberCallback, this, std::placeholders::_1));
    AIMRT_CHECK_ERROR_THROW(ret, "Subscribe failed.");

  } catch (const std::exception& e) {
    AIMRT_ERROR("Init failed, {}", e.what());
    return false;
  }

  AIMRT_INFO("Init succeeded.");

  return true;
}

bool Ros2SubModule::Start() {
  AIMRT_INFO("ros2_sub_module Started succeeded.");
  return true;
}

void Ros2SubModule::Shutdown() {
  AIMRT_INFO("ros2_sub_module Shutdown succeeded.");
}

void Ros2SubModule::SubscriberCallback(const std::shared_ptr<const test_msgs::msg::Info>& msg) {
  AIMRT_INFO("Receive msg: {}", test_msgs::msg::to_yaml(*msg));
}


}  // namespace example::chn_protocols::ros2_sub_module
