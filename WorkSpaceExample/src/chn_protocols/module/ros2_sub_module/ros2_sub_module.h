#pragma once

#include "aimrt_module_cpp_interface/module_base.h"

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

 private:
  aimrt::CoreRef core_;
};

}  // namespace example::chn_protocols::ros2_sub_module
