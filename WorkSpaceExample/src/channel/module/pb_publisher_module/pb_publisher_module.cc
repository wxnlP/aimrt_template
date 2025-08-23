#include "pb_publisher_module/pb_publisher_module.h"

#include "aimrt_module_protobuf_interface/channel/protobuf_channel.h"
#include "aimrt_module_protobuf_interface/util/protobuf_tools.h"
#include "header.pb.h"
#include "yaml-cpp/yaml.h"
#include <chrono>

namespace example::channel::pb_publisher_module {

bool PbPublisherModule::Initialize(aimrt::CoreRef core) {
    // Save aimrt framework handle
    core_ = core;

    try {
        // Read cfg
        auto file_path = core_.GetConfigurator().GetConfigFilePath();
        if (!file_path.empty()) {
            YAML::Node cfg_node = YAML::LoadFile(std::string(file_path));
            topic_name_ = cfg_node["channel_name"].as<std::string>();
            channel_frq_ = cfg_node["channel_frq"].as<double>();
        }

        // 获取执行器句柄
        pub_executor_ = core_.GetExecutorManager().GetExecutor("pub_executor");
        AIMRT_CHECK_ERROR_THROW(pub_executor_ && pub_executor_.SupportTimerSchedule(),
                                "can not get a time enable executor.");

        // 获取发布者句柄
        publisher_ = core_.GetChannelHandle().GetPublisher(topic_name_);
        AIMRT_CHECK_ERROR_THROW(publisher_, "Get publisher {} failed.", 
                                topic_name_);
                        
        // 注册发布类型
        bool ret = aimrt::channel::RegisterPublishType<
                                aimrt::protocols::common::Header>(publisher_);
        AIMRT_CHECK_ERROR_THROW(ret, "Register publish type failed.");

    } catch (const std::exception &e) {
        AIMRT_ERROR("Init failed, {}", e.what());
        return false;
    }

    AIMRT_INFO("Init succeeded.");

    return true;
}

bool PbPublisherModule::Start() 
{
    run_flag_ = true;
    pub_executor_.Execute(std::bind(&PbPublisherModule::MainLoop, this));
    AIMRT_INFO("pb_publisher_module Started succeeded.");
    return true;
}

void PbPublisherModule::Shutdown() 
{
    run_flag_ = false;
    stop_sig_.get_future().wait();
    AIMRT_INFO("pb_publisher_module Shutdown succeeded.");
}

void PbPublisherModule::MainLoop() 
{
    try {
        aimrt::protocols::common::Header msg;
        msg.set_frame_id("base_link");
        msg.set_sequence_num(20250823);
        // 主循环
        while (run_flag_)
        {
            msg.set_time_stamp(std::time(0));
            aimrt::channel::Publish(publisher_, msg);

            AIMRT_INFO("Publish msg: frame_id={}, sequence_num={}, time_stamp={}",
                        msg.frame_id(), msg.sequence_num(), msg.time_stamp());

            std::this_thread::sleep_for(std::chrono::milliseconds(
                                        static_cast<uint32_t>(1000/channel_frq_)));
        }
        AIMRT_INFO("PbPublisherModule MainLoop exit normally.");

    } catch (std::exception& e) {
        AIMRT_ERROR("MainLoop get exception: {}", e.what());
    }
    stop_sig_.set_value();
}

}  // namespace example::channel::pb_publisher_module
