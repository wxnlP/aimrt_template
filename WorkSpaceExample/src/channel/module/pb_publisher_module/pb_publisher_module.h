#pragma once

#include "aimrt_module_cpp_interface/module_base.h"

namespace example::channel::pb_publisher_module {

class PbPublisherModule : public aimrt::ModuleBase {
 public:
  PbPublisherModule() = default;
  ~PbPublisherModule() override = default;

  aimrt::ModuleInfo Info() const override {
    return aimrt::ModuleInfo{.name = "PbPublisherModule"};
  }

  bool Initialize(aimrt::CoreRef core) override;

  bool Start() override;

  void Shutdown() override;

 private:
  auto GetLogger() { return core_.GetLogger(); }

 private:
  aimrt::CoreRef core_;
};

}  // namespace example::channel::pb_publisher_module
