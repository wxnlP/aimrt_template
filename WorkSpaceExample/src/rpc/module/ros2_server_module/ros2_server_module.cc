#include "ros2_server_module/ros2_server_module.h"

#include "yaml-cpp/yaml.h"

namespace example::rpc::ros2_server_module {

bool Ros2ServerModule::Initialize(aimrt::CoreRef core) {
  // Save aimrt framework handle
  core_ = core;

  try {
    // Read cfg
    auto file_path = core_.GetConfigurator().GetConfigFilePath();
    if (!file_path.empty()) {
      YAML::Node cfg_node = YAML::LoadFile(std::string(file_path));
      service_name_ = cfg_node["service_name"].as<std::string>();
    }

    // 创建服务指针
    service_ptr_ = std::make_shared<Ros2RpcInfoServiceImpl>();

    // 注册服务
    bool ret = false;
    if (service_name_.empty()) {
      ret = core_.GetRpcHandle().RegisterService(service_ptr_.get());
    } else {
      ret = core_.GetRpcHandle().RegisterService(service_name_, service_ptr_.get());
    }
    AIMRT_CHECK_ERROR_THROW(ret, "Register service failed.");

  } catch (const std::exception& e) {
    AIMRT_ERROR("Init failed, {}", e.what());
    return false;
  }

  AIMRT_INFO("Init succeeded.");

  return true;
}

bool Ros2ServerModule::Start() {
  AIMRT_INFO("ros2_server_module Started succeeded.");
  return true;
}

void Ros2ServerModule::Shutdown() {
  AIMRT_INFO("ros2_server_module Shutdown succeeded.");
}

}  // namespace example::rpc::ros2_server_module
