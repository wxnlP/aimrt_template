#pragma once

#include "aimrt_module_cpp_interface/module_base.h"

namespace example::channel::ros2_publisher_module {

class Ros2PublisherModule : public aimrt::ModuleBase {
 public:
  Ros2PublisherModule() = default;
  ~Ros2PublisherModule() override = default;

  aimrt::ModuleInfo Info() const override {
    return aimrt::ModuleInfo{.name = "Ros2PublisherModule"};
  }

  bool Initialize(aimrt::CoreRef core) override;

  bool Start() override;

  void Shutdown() override;

 private:
  auto GetLogger() { return core_.GetLogger(); }

 private:
  aimrt::CoreRef core_;
};

}  // namespace example::channel::ros2_publisher_module
