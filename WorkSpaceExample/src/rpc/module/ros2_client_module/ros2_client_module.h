#pragma once

#include "aimrt_module_cpp_interface/module_base.h"

namespace example::rpc::ros2_client_module {

class Ros2ClientModule : public aimrt::ModuleBase {
 public:
  Ros2ClientModule() = default;
  ~Ros2ClientModule() override = default;

  aimrt::ModuleInfo Info() const override {
    return aimrt::ModuleInfo{.name = "Ros2ClientModule"};
  }

  bool Initialize(aimrt::CoreRef core) override;

  bool Start() override;

  void Shutdown() override;

 private:
  auto GetLogger() { return core_.GetLogger(); }

 private:
  aimrt::CoreRef core_;
};

}  // namespace example::rpc::ros2_client_module
