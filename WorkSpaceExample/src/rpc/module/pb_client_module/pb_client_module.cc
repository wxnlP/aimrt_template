#include "pb_client_module/pb_client_module.h"

#include <chrono>

#include "aimrt_module_protobuf_interface/util/protobuf_tools.h"
#include "yaml-cpp/yaml.h"

#define USING_ASYNC 1

namespace example::rpc::pb_client_module {

bool PbClientModule::Initialize(aimrt::CoreRef core) {
  // Save aimrt framework handle
  core_ = core;

  try {
    // Read cfg
    auto file_path = core_.GetConfigurator().GetConfigFilePath();
    if (!file_path.empty()) {
      YAML::Node cfg_node = YAML::LoadFile(std::string(file_path));
      rpc_frq_ = cfg_node["rpc_frq"].as<double>();
      //  若不为空，则赋值
      if (cfg_node["service_name"]) {
        service_name_ = cfg_node["service_name"].as<std::string>();
      }
    }
    // 获取执行器
    work_executor_ = core_.GetExecutorManager().GetExecutor("work_thread_pool");
    AIMRT_CHECK_ERROR_THROW(work_executor_,
                            "Get executor 'work_thread_pool' failed.");

    // 获取RPC句柄
    rpc_handle_ = core_.GetRpcHandle();
    AIMRT_CHECK_ERROR_THROW(rpc_handle_, "Get rpc handle failed.");

    bool ret;
    // 注册RPC Client
    if (service_name_.empty()) {
      // 使用默认值
      ret = example::protocols::test::RegisterInfoRpcClientFunc(rpc_handle_);
    } else {
      // 使用指定值
      ret = example::protocols::test::RegisterInfoRpcClientFunc(rpc_handle_,
                                                                service_name_);
    }
    AIMRT_CHECK_ERROR_THROW(ret, "Register client failed.");

#if USING_SYNC
    // 创建一个同步型 proxy
    sync_proxy_ = std::make_shared<example::protocols::test::InfoRpcSyncProxy>(
        rpc_handle_);
#elif USING_ASYNC
    // 创建一个异步回调型 proxy
    async_proxy_ =
        std::make_shared<example::protocols::test::InfoRpcAsyncProxy>(
            rpc_handle_);
#elif USING_FUTURE
    // 创建一个异步future型 proxy
    future_proxy_ =
        std::make_shared<example::protocols::test::InfoRpcFutureProxy>(
            rpc_handle_);
#endif

    if (!service_name_.empty()) {
#if USING_SYNC
      sync_proxy_->SetServiceName(service_name_);
#elif USING_ASYNC
      async_proxy_->SetServiceName(service_name_);
#elif USING_FUTURE
      future_proxy_->SetServiceName(service_name_);
#endif
    }

  } catch (const std::exception& e) {
    AIMRT_ERROR("Init failed, {}", e.what());
    return false;
  }

  AIMRT_INFO("Init succeeded.");

  return true;
}

bool PbClientModule::Start() {
  try {
    run_flag_ = true;
    work_executor_.Execute(std::bind(&PbClientModule::MainLoop, this));

  } catch (const std::exception& e) {
    AIMRT_ERROR("pb_client_module Started Fail.");
    return false;
  }

  AIMRT_INFO("pb_client_module Started succeeded.");
  return true;
}

void PbClientModule::Shutdown() {
  try {
    run_flag_ = false;
    stop_sig_.get_future().wait();
  } catch (std::exception& e) {
    AIMRT_ERROR("Shutdown failed, {}", e.what());
    return;
  }

  AIMRT_INFO("pb_client_module Shutdown succeeded.");
}

void PbClientModule::MainLoop() {
  try {
    uint64_t count{0};
    example::protocols::test::RpcInfoReq req{};
    example::protocols::test::RpcInfoRes res{};
    while (run_flag_) {
      count++;
      AIMRT_INFO("Loop count: {}", count);

      // 更新请求数据
      req.set_info("count");
      req.set_time_stamp(std::time(0));

#if USING_SYNC
      // 创建一份新的context（可选）
      auto ctx_ptr = sync_proxy_->NewContextSharedPtr();
      ctx_ptr->SetTimeout(std::chrono::seconds(3));
      // 调用 rpc （在.proto中定义的函数）
      auto status = sync_proxy_->GetRpcInfo(ctx_ptr, req, res);
      // 判断返回状态
      if (status.OK()) {
        AIMRT_INFO("Client get rpc ret, status: {}, rsp: {}", status.ToString(),
                   aimrt::Pb2CompactJson(res));
      } else {
        AIMRT_WARN("Client get rpc error ret, status: {}", status.ToString());
      }
#elif USING_ASYNC
      // 创建一份新的context（可选）
      auto ctx_ptr = async_proxy_->NewContextSharedPtr();
      ctx_ptr->SetTimeout(std::chrono::seconds(3));
      // 调用 rpc
      async_proxy_->GetRpcInfo(
          ctx_ptr, req, res, [this, res](aimrt::rpc::Status status) {
            AIMRT_INFO("Async callback on!");
            // 回调函数处理状态
            if (status.OK()) {
              AIMRT_INFO("Client get rpc ret \nstatus: {} \nrsp: {}",
                         status.ToString(), aimrt::Pb2CompactJson(res));
            } else {
              AIMRT_WARN("Client get rpc error ret, status: {}",
                         status.ToString());
            }
          });
#elif USING_FUTURE
      // 创建一份新的context（可选）
      auto ctx_ptr = future_proxy_->NewContextSharedPtr();
      ctx_ptr->SetTimeout(std::chrono::seconds(3));
      // 调用 rpc，但会std::future<Status>
      auto status_future = future_proxy_->GetRpcInfo(ctx_ptr, req, res);

      // ... 这里可以做一些其他事情 ...

      // 阻塞的获取status
      auto status = status_future.get(); 
      // 判断返回状态
      if (status.OK()) {
        AIMRT_INFO("Get future result!");
        AIMRT_INFO("Client get rpc ret, status: {}, rsp: {}", status.ToString(),
                   aimrt::Pb2CompactJson(res));
      } else {
        AIMRT_WARN("Client get rpc error ret, status: {}", status.ToString());
      }
#endif
      std::this_thread::sleep_for(
          std::chrono::milliseconds(static_cast<uint32_t>(1000 / rpc_frq_)));
    }
    AIMRT_INFO("Exit MainLoop.");

  } catch (std::exception& e) {
    AIMRT_ERROR("Exit MainLoop with exception, {}", e.what());
  }
  stop_sig_.set_value();
}

}  // namespace example::rpc::pb_client_module
