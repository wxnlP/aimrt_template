#pragma once

#include "aimrt_module_cpp_interface/module_base.h"

namespace example::chn_protocols::chn_publisher_module {

class ChnPublisherModule : public aimrt::ModuleBase {
 public:
  ChnPublisherModule() = default;
  ~ChnPublisherModule() override = default;

  aimrt::ModuleInfo Info() const override {
    return aimrt::ModuleInfo{.name = "ChnPublisherModule"};
  }

  bool Initialize(aimrt::CoreRef core) override;

  bool Start() override;

  void Shutdown() override;

 private:
  auto GetLogger() { return core_.GetLogger(); }

 private:
  aimrt::CoreRef core_;
};

}  // namespace example::chn_protocols::chn_publisher_module
