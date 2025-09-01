#pragma once

#include "aimrt_module_cpp_interface/module_base.h"

namespace example::rpc::pb_server_module {

class PbServerModule : public aimrt::ModuleBase {
 public:
  PbServerModule() = default;
  ~PbServerModule() override = default;

  aimrt::ModuleInfo Info() const override {
    return aimrt::ModuleInfo{.name = "PbServerModule"};
  }

  bool Initialize(aimrt::CoreRef core) override;

  bool Start() override;

  void Shutdown() override;

 private:
  auto GetLogger() { return core_.GetLogger(); }

 private:
  aimrt::CoreRef core_;
};

}  // namespace example::rpc::pb_server_module
