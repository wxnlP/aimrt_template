#include "ros2_pub_module/ros2_pub_module.h"

#include "yaml-cpp/yaml.h"

namespace example::chn_protocols::ros2_pub_module {

bool Ros2PubModule::Initialize(aimrt::CoreRef core) {
  // Save aimrt framework handle
  core_ = core;

  try {
    // Read cfg
    auto file_path = core_.GetConfigurator().GetConfigFilePath();
    if (!file_path.empty()) {
      YAML::Node cfg_node = YAML::LoadFile(std::string(file_path));
      for (const auto& itr : cfg_node) {
        std::string k = itr.first.as<std::string>();
        std::string v = itr.second.as<std::string>();
        AIMRT_INFO("cfg [{} : {}]", k, v);
      }
    }

  } catch (const std::exception& e) {
    AIMRT_ERROR("Init failed, {}", e.what());
    return false;
  }

  AIMRT_INFO("Init succeeded.");

  return true;
}

bool Ros2PubModule::Start() {
  AIMRT_INFO("ros2_pub_module Started succeeded.");
  return true;
}

void Ros2PubModule::Shutdown() {
  AIMRT_INFO("ros2_pub_module Shutdown succeeded.");
}

}  // namespace example::chn_protocols::ros2_pub_module
