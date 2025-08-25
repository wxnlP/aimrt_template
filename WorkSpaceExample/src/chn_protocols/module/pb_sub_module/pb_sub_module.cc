#include "pb_sub_module/pb_sub_module.h"
#include "aimrt_module_protobuf_interface/channel/protobuf_channel.h"
#include "aimrt_module_protobuf_interface/util/protobuf_tools.h"

#include "yaml-cpp/yaml.h"

namespace example::chn_protocols::pb_sub_module {

bool PbSubModule::Initialize(aimrt::CoreRef core) {
    // Save aimrt framework handle
    core_ = core;

    try {
        // Read cfg
        auto file_path = core_.GetConfigurator().GetConfigFilePath();
        if (!file_path.empty()) {
            YAML::Node cfg_node = YAML::LoadFile(std::string(file_path));
            topic_name_ = cfg_node["channel_name"].as<std::string>();
        }
        subscriber_pb_ = core_.GetChannelHandle().GetSubscriber(topic_name_);
        AIMRT_CHECK_ERROR_THROW(subscriber_pb_, 
            "Get subscriber for topic '{}' failed.", topic_name_);

        bool ret = aimrt::channel::Subscribe<example::protocols::test::Info>(
            subscriber_pb_,
            std::bind(
                &PbSubModule::PbSubscriberCallback, 
                this, 
                std::placeholders::_1));
        AIMRT_CHECK_ERROR_THROW(ret, "Subscribe type failed.");

    } catch (const std::exception& e) {
        AIMRT_ERROR("Init failed, {}", e.what());
        return false;
    }

    AIMRT_INFO("Init succeeded.");

    return true;
}

bool PbSubModule::Start() {
    AIMRT_INFO("chn_subscriber_module Started succeeded.");
    return true;
}

void PbSubModule::Shutdown() {
    AIMRT_INFO("chn_subscriber_module Shutdown succeeded.");
}

void PbSubModule::PbSubscriberCallback(
    const std::shared_ptr<const example::protocols::test::Info>& msg) 
{
    AIMRT_INFO("In PbSubscriberCallback.");
    AIMRT_INFO("Received message: '{}'", aimrt::Pb2CompactJson(*msg));  
}

}  // namespace example::chn_protocols::chn_subscriber_module
