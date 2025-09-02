#pragma once

#include <atomic>
#include <future>
#include "aimrt_module_cpp_interface/module_base.h"
#include "info_rpc.aimrt_rpc.pb.h"


namespace example::rpc::pb_client_module {

class PbClientModule : public aimrt::ModuleBase {
 public:
  PbClientModule() = default;
  ~PbClientModule() override = default;

  aimrt::ModuleInfo Info() const override {
    return aimrt::ModuleInfo{.name = "PbClientModule"};
  }

  bool Initialize(aimrt::CoreRef core) override;

  bool Start() override;

  void Shutdown() override;

 private:
  auto GetLogger() { return core_.GetLogger(); }
  void MainLoop();

 private:
  aimrt::CoreRef core_{};
  aimrt::rpc::RpcHandleRef rpc_handle_{};
  aimrt::executor::ExecutorRef work_executor_{};
  
  // 同步型接口
  std::shared_ptr<example::protocols::test::InfoRpcSyncProxy> sync_proxy_{};
  // 异步回调型接口
  std::shared_ptr<example::protocols::test::InfoRpcAsyncProxy> async_proxy_{};
  // 异步Future型接口
  std::shared_ptr<example::protocols::test::InfoRpcFutureProxy> future_proxy_{};

  std::string service_name_{};
  double rpc_frq_{};
  // 多线程使用，保持原子性操作
  std::atomic_bool run_flag_{false};
  std::promise<void> stop_sig_{};
};

}  // namespace example::rpc::pb_client_module
