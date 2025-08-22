#include "ros2_publisher_module/ros2_publisher_module.h"
#include "aimrt_module_ros2_interface/channel/ros2_channel.h"
#include "example_ros2/msg/ros_test_msg.hpp"

#include "yaml-cpp/yaml.h"

namespace example::channel::ros2_publisher_module
{

bool Ros2PublisherModule::Initialize(aimrt::CoreRef core)
{
    // Save aimrt framework handle
    core_ = core;

    try {
        // Read cfg
        auto file_path = core_.GetConfigurator().GetConfigFilePath();
        if (!file_path.empty()) {
            YAML::Node cfg_node = YAML::LoadFile(std::string(file_path));
            // 获取文件的channel名称和通信频率配置
            channel_frq_ = cfg_node["channel_frq"].as<double>();
            channel_name_ = cfg_node["channel_name"].as<std::string>();
        }

        // 获取一个执行器句柄
        pub_executor_ = core_.GetExecutorManager().GetExecutor("pub_executor")
        AIMRT_CHECK_ERROR_THROW(pub_executor_ && pub_executor_.SupportTimerSchedule(),
                                "can not get a time enable executor.");

        // 获取一个发布者句柄
        publisher_ = core_.GetChannelHandle().GetPublisher(channel_name_);
        AIMRT_CHECK_ERROR_THROW(publisher_, "Get publisher for topic '{}' failed.", channel_name_);

        // 注册发布者的消息类型（必须在Initialize环节完成）
        bool ret = aimrt::channel::RegisterPublishType<example_ros2::msg::RosTestMsg>(publisher_);
        AIMRT_CHECK_ERROR_THROW(ret, "Register publishType failed.");

    } catch (const std::exception &e) {
        AIMRT_ERROR("Init failed, {}", e.what());
        return false;
    }

    AIMRT_INFO("Init succeeded.");

    return true;
}

bool Ros2PublisherModule::Start()
{
    try {
        run_flag_ = true;
        pub_executor_.Execute(std::bind(&Ros2PublisherModule::MainLoop, this));

    } catch (std::exception& e) {
        AIMRT_ERROR("Start failed, {}", e.what());
        return false;
    }

    AIMRT_INFO("ros2_publisher_module Started succeeded.");
    return true;
}

void Ros2PublisherModule::Shutdown()
{
    try {
        run_flag_ = false;
        stop_sig_.get_future().wait();

    } catch (std::exception& e) {
        AIMRT_ERROR("Shutdown failed, {}", e.what());
        return;
    }

    AIMRT_INFO("ros2_publisher_module Shutdown succeeded.");
}

void Ros2PublisherModule::MainLoop()
{
    try {
        uint32_t count = 0;
        while(run_flag_) 
        {
            count++;
            AIMRT_INFO("Loop {} -----------------", count);

            example_ros2::msg::RosTestMsg msg;
            msg.num = count;
            msg.data = { count*1, count*2, count*3 };

            aimrt::channel::Publish(publisher_, msg);
            AIMRT_INFO("Publish new ros msg : {} {}", msg.num, msg.data);

            // 按指定频率发送channel
            std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<uint32_t>(1000/channel_frq_));
        } 
        AIMRT_INFO("Exit MainLoop seccess.")

    } catch (std::exception& e) {
        AIMRT_ERROR("Exit MainLoop failed, {}", e.what());
        return;
    }

    stop_sig_.set_value();
}

} // namespace example::channel::ros2_publisher_module
