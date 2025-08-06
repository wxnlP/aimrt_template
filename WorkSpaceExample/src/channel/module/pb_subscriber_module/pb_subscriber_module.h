#pragma once

#include "aimrt_module_cpp_interface/module_base.h"

namespace example::channel::pb_subscriber_module {

class PbSubscriberModule : public aimrt::ModuleBase {
 public:
  PbSubscriberModule() = default;
  ~PbSubscriberModule() override = default;

  aimrt::ModuleInfo Info() const override {
    return aimrt::ModuleInfo{.name = "PbSubscriberModule"};
  }

  bool Initialize(aimrt::CoreRef core) override;

  bool Start() override;

  void Shutdown() override;

 private:
  auto GetLogger() { return core_.GetLogger(); }

 private:
  aimrt::CoreRef core_;
};

}  // namespace example::channel::pb_subscriber_module
