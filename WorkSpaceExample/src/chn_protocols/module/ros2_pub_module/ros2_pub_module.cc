#include "ros2_pub_module/ros2_pub_module.h"
#include "aimrt_module_ros2_interface/channel/ros2_channel.h"
#include "aimrt_module_ros2_interface/util/ros2_type_support.h"
#include "test_msgs/msg/info.hpp"
#include "yaml-cpp/yaml.h"

namespace example::chn_protocols::ros2_pub_module {

bool Ros2PubModule::Initialize(aimrt::CoreRef core) {
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

        pub_executor_ = core_.GetExecutorManager().GetExecutor("pub_executor");
        AIMRT_CHECK_ERROR_THROW(pub_executor_ && pub_executor_.SupportTimerSchedule(),
            "can not get a time enable executor.");

        publisher_ = core_.GetChannelHandle().GetPublisher(topic_name_);
        AIMRT_CHECK_ERROR_THROW(publisher_, "Get publisher {} failed.",
                                topic_name_);

        bool ret = aimrt::channel::RegisterPublishType<
            test_msgs::msg::Info>(publisher_);
        AIMRT_CHECK_ERROR_THROW(ret, "Register publish type failed.");

    } catch (const std::exception& e) {
        AIMRT_ERROR("Init failed, {}", e.what());
        return false;
    }

    AIMRT_INFO("Init succeeded.");

    return true;
}

bool Ros2PubModule::Start() {
    run_flag_ = true;
    pub_executor_.Execute(std::bind(&Ros2PubModule::MainLoop, this));
    AIMRT_INFO("Ros2PubModule Started succeeded.");
    return true;
}

void Ros2PubModule::Shutdown() {
    run_flag_ = false;
    stop_sig_.get_future().wait();
    AIMRT_INFO("Ros2PubModule Shutdown succeeded.");
}

void Ros2PubModule::MainLoop() {
    test_msgs::msg::Info msg;
    msg.info = "hello aimrt info";
    while (run_flag_)
    {
        msg.time_stamp = std::time(0);

        aimrt::channel::Publish(publisher_, msg);
        AIMRT_INFO("Publish msg: {}", test_msgs::msg::to_yaml(msg));

        std::this_thread::sleep_for(std::chrono::milliseconds(
            static_cast<uint32_t>(1000/channel_frq_)));
    }
    AIMRT_INFO("Ros2PubModule MainLoop exit normally.");

    stop_sig_.set_value();
}

}  // namespace example::chn_protocols::ros2_pub_module
