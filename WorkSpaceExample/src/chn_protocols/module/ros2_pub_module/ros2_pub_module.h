#pragma once

#include "aimrt_module_cpp_interface/module_base.h"

namespace example::chn_protocols::ros2_pub_module {

class Ros2PubModule : public aimrt::ModuleBase {
 public:
  Ros2PubModule() = default;
  ~Ros2PubModule() override = default;

  aimrt::ModuleInfo Info() const override {
    return aimrt::ModuleInfo{.name = "Ros2PubModule"};
  }

  bool Initialize(aimrt::CoreRef core) override;

  bool Start() override;

  void Shutdown() override;

 private:
  auto GetLogger() { return core_.GetLogger(); }

 private:
  aimrt::CoreRef core_;
};

}  // namespace example::chn_protocols::ros2_pub_module
