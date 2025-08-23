#include "pb_subscriber_module/pb_subscriber_module.h"
#include "aimrt_module_protobuf_interface/channel/protobuf_channel.h"
#include "aimrt_module_protobuf_interface/util/protobuf_tools.h"
#include "yaml-cpp/yaml.h"

namespace example::channel::pb_subscriber_module {

bool PbSubscriberModule::Initialize(aimrt::CoreRef core) {
    // Save aimrt framework handle
    core_ = core;

    try {
        // Read cfg
        auto file_path = core_.GetConfigurator().GetConfigFilePath();
        if (!file_path.empty()) {
            YAML::Node cfg_node = YAML::LoadFile(std::string(file_path));
            topic_name_ = cfg_node["channel_name"].as<std::string>();
        }

        subscriber_ = core_.GetChannelHandle().GetSubscriber(topic_name_);
        AIMRT_CHECK_ERROR_THROW(subscriber_, "Get subscriber for topic '{}' failed.",
                                topic_name_);

        bool ret = aimrt::channel::Subscribe<aimrt::protocols::common::Header>(
            subscriber_,
            std::bind(&PbSubscriberModule::SubscriberCallback, this, std::placeholders::_1)
        );
        AIMRT_CHECK_ERROR_THROW(ret, "Subscribe type failed.");

    } catch (const std::exception& e) {
        AIMRT_ERROR("Init failed, {}", e.what());
        return false;
    }

    AIMRT_INFO("Init succeeded.");

    return true;
}

bool PbSubscriberModule::Start() {
    AIMRT_INFO("pb_subscriber_module Started succeeded.");
    return true;
}

void PbSubscriberModule::Shutdown() {
    AIMRT_INFO("pb_subscriber_module Shutdown succeeded.");
}

void PbSubscriberModule::SubscriberCallback(
    const std::shared_ptr<const aimrt::protocols::common::Header>& msg) 
{
    AIMRT_INFO("In SubscriberCallback.");
    AIMRT_INFO("Received message: '{}'", aimrt::Pb2CompactJson(*msg));   
}

}  // namespace example::channel::pb_subscriber_module
