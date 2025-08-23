#include "ros2_subscriber_module/ros2_subscriber_module.h"
#include "aimrt_module_ros2_interface/channel/ros2_channel.h"
#include "yaml-cpp/yaml.h"


namespace example::channel::ros2_subscriber_module
{

bool Ros2SubscriberModule::Initialize(aimrt::CoreRef core)
{
    // Save aimrt framework handle
    core_ = core;

    try {
        // Read cfg
        auto file_path = core_.GetConfigurator().GetConfigFilePath();
        if (!file_path.empty()) {
            YAML::Node cfg_node = YAML::LoadFile(std::string(file_path));
            // 读取部署文件的话题名称
            topic_name_ = cfg_node["channel_name"].as<std::string>();
        }

        // 获取一个订阅者句柄
        subscriber_ = core_.GetChannelHandle().GetSubscriber(topic_name_);
        AIMRT_CHECK_ERROR_THROW(subscriber_, "Get subscriber for topic '{}' failed.",
                                topic_name_);

        // 订阅消息类型（必须在Initialize环节完成）
        bool ret = aimrt::channel::Subscribe<std_msgs::msg::String>(
            subscriber_,
            std::bind(&Ros2SubscriberModule::SubscriberCallback, this, std::placeholders::_1)
        );
        AIMRT_CHECK_ERROR_THROW(ret, "Subscribe type failed.");

    } catch (const std::exception &e) {
        AIMRT_ERROR("Init failed, {}", e.what());
        return false;
    }

    AIMRT_INFO("Init succeeded.");

    return true;
}

bool Ros2SubscriberModule::Start()
{
    AIMRT_INFO("ros2_subscriber_module Started succeeded.");
    return true;
}

void Ros2SubscriberModule::Shutdown()
{
    AIMRT_INFO("ros2_subscriber_module Shutdown succeeded.");
}

void Ros2SubscriberModule::SubscriberCallback(const std::shared_ptr<const std_msgs::msg::String>& msg)
{
    AIMRT_INFO("Received message: '{}'", std_msgs::msg::to_yaml(*msg));
}

} // namespace example::channel::ros2_subscriber_module
