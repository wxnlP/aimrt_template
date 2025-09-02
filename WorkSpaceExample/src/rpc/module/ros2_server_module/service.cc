#include "ros2_server_module/service.h"

namespace example::rpc::ros2_server_module {

// 注意接口的命名空间
aimrt::rpc::Status Ros2RpcInfoServiceImpl::RpcInfo(
    aimrt::rpc::ContextRef ctx_ref, const test_msgs::srv::RpcInfo_Request& req,
    test_msgs::srv::RpcInfo_Response& rsp) {
  // 基于请求数据做响应处理
  rsp.info = "Response:" + req.info;
  rsp.req_stamp = req.time_stamp;
  rsp.res_stamp = std::time(0);

  AIMRT_INFO(
      "Server handle new rpc call.\ncontext: {}\nreq: {}\nreturn rsp: {}",
      ctx_ref.ToString(), test_msgs::srv::to_yaml(req),
      test_msgs::srv::to_yaml(rsp));
  return aimrt::rpc::Status();
}

void Ros2RpcInfoAsyncServiceImpl::RpcInfo(
    aimrt::rpc::ContextRef ctx_ref, const test_msgs::srv::RpcInfo_Request& req,
    test_msgs::srv::RpcInfo_Response& rsp,
    std::function<void(aimrt::rpc::Status)>&& callback) {
  // 基于请求数据做响应处理
  rsp.info = "Async Response:" + req.info;
  rsp.req_stamp = req.time_stamp;
  rsp.res_stamp = std::time(0);

  AIMRT_INFO(
      "Server handle new async rpc call.\ncontext: {}\nreq: {}\nreturn rsp: {}",
      ctx_ref.ToString(), test_msgs::srv::to_yaml(req),
      test_msgs::srv::to_yaml(rsp));

  // 通过回调返回结果
  callback(aimrt::rpc::Status());
}

};  // namespace example::rpc::ros2_server_module