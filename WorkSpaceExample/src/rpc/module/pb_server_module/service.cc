#include "pb_server_module/service.h"
#include "aimrt_module_protobuf_interface/util/protobuf_tools.h"

namespace example::rpc::pb_server_module {

aimrt::rpc::Status RpcInfoServiceImpl::GetRpcInfo(
    aimrt::rpc::ContextRef ctx_ref,
    const ::example::protocols::test::RpcInfoReq& req,
    ::example::protocols::test::RpcInfoRes& rsp) {

  // 基于请求数据做响应处理
  rsp.set_info("Response:" + req.info());
  rsp.set_req_stamp(req.time_stamp());
  rsp.set_res_stamp(std::time(0));

  AIMRT_INFO("Server handle new rpc call.\n context: {}\n req: {}\n return rsp: {}",
        ctx_ref.ToString(), aimrt::Pb2CompactJson(req), aimrt::Pb2CompactJson(rsp));
  return aimrt::rpc::Status();
}

}