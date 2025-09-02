#include "ros2_server_module/service.h"

namespace example::rpc::ros2_server_module {

// 注意接口的命名空间
aimrt::rpc::Status Ros2RpcInfoServiceImpl::RpcInfo(
    aimrt::rpc::ContextRef ctx_ref,
    const test_msgs::srv::RpcInfo_Request& req,
    test_msgs::srv::RpcInfo_Response& rsp)
{
    // 基于请求数据做响应处理
    rsp.info = "Response:" + req.info;
    rsp.req_stamp = req.time_stamp;
    rsp.res_stamp = std::time(0);

    AIMRT_INFO("Server handle new rpc call.\n context: {}\n req: {}\n return rsp: {}",
          ctx_ref.ToString(), test_msgs::srv::to_yaml(req), test_msgs::srv::to_yaml(rsp));
    return aimrt::rpc::Status();
}

};  // namespace example::rpc::ros2_server_module