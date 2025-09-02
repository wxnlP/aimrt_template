#include "pb_server_module/pb_server_module.h"

#include "yaml-cpp/yaml.h"

#define USING_ASYNC 1

namespace example::rpc::pb_server_module {

bool PbServerModule::Initialize(aimrt::CoreRef core) {
  // Save aimrt framework handle
  core_ = core;

  try {
    // Read cfg
    auto file_path = core_.GetConfigurator().GetConfigFilePath();
    if (!file_path.empty()) {
      YAML::Node cfg_node = YAML::LoadFile(std::string(file_path));
      if (cfg_node["service_name"]) {
        service_name_ = cfg_node["service_name"].as<std::string>();
      }
    }

#if USING_SYNC
    service_ptr_ = std::make_shared<RpcInfoServiceImpl>();
    // Register service
    bool ret = false;
    if (service_name_.empty()) {
      ret = core_.GetRpcHandle().RegisterService(service_ptr_.get());
    } else {
      ret = core_.GetRpcHandle().RegisterService(service_name_,
                                                 service_ptr_.get());
    }
#elif USING_ASYNC
    async_service_ptr_ = std::make_shared<RpcInfoAsyncServiceImpl>();
    // Register service
    bool ret = false;
    if (service_name_.empty()) {
      ret = core_.GetRpcHandle().RegisterService(async_service_ptr_.get());
    } else {
      ret = core_.GetRpcHandle().RegisterService(service_name_,
                                                 async_service_ptr_.get());
    }
#endif

    AIMRT_CHECK_ERROR_THROW(ret, "Register service failed.");

    AIMRT_INFO("Register service succeeded.");

  } catch (const std::exception& e) {
    AIMRT_ERROR("Init failed, {}", e.what());
    return false;
  }

  AIMRT_INFO("Init succeeded.");

  return true;
}

bool PbServerModule::Start() {
  AIMRT_INFO("pb_server_module Started succeeded.");
  return true;
}

void PbServerModule::Shutdown() {
  AIMRT_INFO("pb_server_module Shutdown succeeded.");
}

}  // namespace example::rpc::pb_server_module
