#include "parameter_module/parameter_module.h"
#include "util/log_util.h"
#include "yaml-cpp/yaml.h"

namespace example::parameter::parameter_module {

bool ParameterModule::Initialize(aimrt::CoreRef core) {
    // Save aimrt framework handle
    core_ = core;

    // Log
    AIMRT_INFO("Init.");

    try {
        // Read cfg
        auto file_path = core_.GetConfigurator().GetConfigFilePath();
        if (!file_path.empty()) {
            YAML::Node cfg_node = YAML::LoadFile(file_path.data());
            for (const auto& itr : cfg_node) {
                std::string k = itr.first.as<std::string>();
                std::string v = itr.second.as<std::string>();
                AIMRT_INFO("cfg [{} : {}]", k, v);
            }
        }

        // 创建一个执行器实例
        param_executor_ = core_.GetExecutorManager().GetExecutor("param_executor");
        AIMRT_CHECK_ERROR_THROW(param_executor_ && param_executor_.SupportTimerSchedule(), 
                                "can not get param_executor.");

        // 获取参数接口句柄
        param_handle_ = core_.GetParameterHandle();
        AIMRT_CHECK_ERROR_THROW(param_handle_, "can not get param_handle_.");

    } catch (const std::exception& e) {
        AIMRT_ERROR("Init failed, {}", e.what());
        return false;
    }

    AIMRT_INFO("Init succeeded.");

    return true;
}

bool ParameterModule::Start() {
    try {
        param_executor_.Execute(std::bind(&ParameterModule::SetParameterLoop, this));
        param_executor_.Execute(std::bind(&ParameterModule::GetParameterLoop, this));
    } catch (const std::exception& e) {
        AIMRT_ERROR("Start fail, {}.", e.what());
        return false;
    }

    AIMRT_INFO("Start succeeded.");
    return true;
}

void ParameterModule::Shutdown() {
    run_flag_ = false;

    AIMRT_INFO("Waitting for the cycle ends ......");

    // 等待循环结束（阻塞等待状态就绪）
    set_loop_stop_sig_.get_future().wait();
    get_loop_stop_sig_.get_future().wait();

    AIMRT_INFO("Shutdown succeeded.");
}

void ParameterModule::SetParameterLoop() {
    try {
        uint32_t count = 0;
        while (run_flag_)
        {
            count++;
            std::string key = "key-" + std::to_string(count); 
            std::string val = "val-" + std::to_string(count); 

            param_handle_.SetParameter(key, val);
            AIMRT_INFO("Set parameter: {} -- {}", key, val);

            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
        AIMRT_INFO("Exit SetParameterLoop.");

    } catch (std::exception& e) {
        AIMRT_ERROR("Set parameter fail, {}.", e.what());
    }
    
    // 触发状态就绪
    set_loop_stop_sig_.set_value();
}

void ParameterModule::GetParameterLoop() {
    try {
        uint32_t count = 0;
        while (run_flag_)
        {
            count++;
            std::string key = "key-" + std::to_string(count); 

            std::string val = param_handle_.GetParameter(key);
            AIMRT_INFO("Get parameter: '{}' -- '{}'", key, val);

            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
        AIMRT_INFO("Exit GetParameterLoop.");

    } catch (std::exception& e) {
        AIMRT_ERROR("Get parameter fail, {}.", e.what());
    }

    std::string file_key = "key1";
    std::string file_val = param_handle_.GetParameter(file_key);

    // 这里可以验证部署阶段设置的模块参数与逻辑阶段的参数接口并不相通
    // 这也是AimRT的核心理念之一，逻辑与部署分离
    AIMRT_INFO("File key and value '{}' '{}'", file_key, file_val);
    // 触发状态就绪
    get_loop_stop_sig_.set_value();
}

}