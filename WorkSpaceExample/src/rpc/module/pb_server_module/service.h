#pragma once

#include "util/log_util.h"
#include "info_rpc.aimrt_rpc.pb.h"

namespace example::rpc::pb_server_module {

class RpcInfoServiceImpl : public example::protocols::test::InfoRpcSyncService {
 public:
  RpcInfoServiceImpl() = default;
  ~RpcInfoServiceImpl() override = default;

  aimrt::rpc::Status GetRpcInfo(
      aimrt::rpc::ContextRef ctx_ref,
      const ::example::protocols::test::RpcInfoReq& req,
      ::example::protocols::test::RpcInfoRes& rsp) override;
 private:
    // 独立日志组件
    auto GetLogger() { return aimrt::common::util::SimpleLogger(); }
};

}
