#pragma once

#include "aimrt_module_cpp_interface/module_base.h"
#include "ros2_server_module/service.h"

namespace example::rpc::ros2_server_module {

class Ros2ServerModule : public aimrt::ModuleBase {
 public:
  Ros2ServerModule() = default;
  ~Ros2ServerModule() override = default;

  aimrt::ModuleInfo Info() const override {
    return aimrt::ModuleInfo{.name = "Ros2ServerModule"};
  }

  bool Initialize(aimrt::CoreRef core) override;

  bool Start() override;

  void Shutdown() override;

 private:
  auto GetLogger() { return core_.GetLogger(); }

 private:
  aimrt::CoreRef core_;

  std::shared_ptr<Ros2RpcInfoServiceImpl> service_ptr_{};
  std::shared_ptr<Ros2RpcInfoAsyncServiceImpl> async_service_ptr_{};

  std::string service_name_{};
};

}  // namespace example::rpc::ros2_server_module
