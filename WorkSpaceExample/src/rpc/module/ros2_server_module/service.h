#pragma once

#include "RpcInfo.aimrt_rpc.srv.h"
#include "util/log_util.h"

namespace example::rpc::ros2_server_module {

class Ros2RpcInfoServiceImpl : public test_msgs::srv::RpcInfoSyncService {
 public:
  Ros2RpcInfoServiceImpl() = default;
  ~Ros2RpcInfoServiceImpl() override = default;

  aimrt::rpc::Status RpcInfo(
    aimrt::rpc::ContextRef ctx_ref,
    const test_msgs::srv::RpcInfo_Request& req,
    test_msgs::srv::RpcInfo_Response& rsp) override;

 private:
  auto GetLogger() { return aimrt::common::util::SimpleLogger(); }   
};

};  // namespace example::rpc::ros2_server_module
