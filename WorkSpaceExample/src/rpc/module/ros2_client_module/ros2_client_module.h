#pragma once

#include "aimrt_module_cpp_interface/module_base.h"
/* 特别注意这个头文件的形式，与ROS2的不一样 */
#include "RpcInfo.aimrt_rpc.srv.h"


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
  void MainLoop();

 private:
  aimrt::CoreRef core_{};
  aimrt::rpc::RpcHandleRef rpc_handle_{};
  aimrt::executor::ExecutorRef work_executor_{};

  // 同步型接口
  std::shared_ptr<test_msgs::srv::RpcInfoSyncProxy> sync_proxy_{};
  // 异步回调型接口
  std::shared_ptr<test_msgs::srv::RpcInfoAsyncProxy> async_proxy_{};
  // 异步Future型接口
  std::shared_ptr<test_msgs::srv::RpcInfoFutureProxy> future_proxy_{};
  // 无栈协程型接口
  std::shared_ptr<test_msgs::srv::RpcInfoCoProxy> co_proxy_{};

  std::string service_name_{};
  double rpc_frq_{};
  // 多线程使用，保持原子性操作
  std::atomic_bool run_flag_{false};
  std::promise<void> stop_sig_{};
};

}  // namespace example::rpc::ros2_client_module
