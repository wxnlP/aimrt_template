#include "helloworld_module/helloworld_module.h"
#include "util/log_util.h"
#include "yaml-cpp/yaml.h"

namespace aimrt::helloworld::helloworld_module {

bool HelloWorldModule::Initialize(aimrt::CoreRef core) {
    // Save aimrt framework handle
    core_ = core;

    // Log
    AIMRT_HL_INFO(core_.GetLogger(), "Init.");

    try {
        // Read cfg
        auto file_path = core_.GetConfigurator().GetConfigFilePath();
        if (!file_path.empty()) {
            YAML::Node cfg_node = YAML::LoadFile(file_path.data());
            for (const auto& itr : cfg_node) {
                std::string k = itr.first.as<std::string>();
                std::string v = itr.second.as<std::string>();
                AIMRT_HL_INFO(core_.GetLogger(), "cfg [{} : {}]", k, v);
            }
        }

    } catch (const std::exception& e) {
        AIMRT_HL_ERROR(core_.GetLogger(), "Init failed, {}", e.what());
        return false;
    }

    AIMRT_HL_INFO(core_.GetLogger(), "Init succeeded.");

    return true;
}

bool HelloWorldModule::Start() {
    AIMRT_HL_INFO(core_.GetLogger(), "Start succeeded.");
    return true;
}

void HelloWorldModule::Shutdown() {
    AIMRT_HL_INFO(core_.GetLogger(), "Shutdown succeeded.");
}

}