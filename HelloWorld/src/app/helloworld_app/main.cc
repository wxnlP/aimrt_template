#include <csignal>
#include <iostream>

#include "core/aimrt_core.h"
#include "helloworld_module/helloworld_module.h"

using namespace aimrt::runtime::core;

// 全局 AimRTCore 指针
AimRTCore *global_core_ptr_ = nullptr;


// 信号处理函数
void SignalHandler(int sig) {
    if (global_core_ptr_ && (sig == SIGINT || sig == SIGTERM)) {
        global_core_ptr_->Shutdown();
        return;
    }
    raise(sig);
};

int32_t main(int32_t argc, char **argv) {
    // 处理 Ctrl+C 
    signal(SIGINT, SignalHandler);
    // 处理 kill 命令
    signal(SIGTERM, SignalHandler);

    std::cout << "AimRT start." << std::endl;

    try {
        // 全局 AimRTCore 指针赋值
        AimRTCore core;
        global_core_ptr_ = &core;

        // 注册模块
        HelloWorldModule helloworld_module;
        core.GetModuleManager().RegisterModule(helloworld_module.NativeHandle());

        // 定义配置文件结构体
        AimRTCore::Options options;
        // 加载配置文件
        options.cfg_file_path = argv[1];

        /* AimRT框架初始化（并调用各模块的Initialize） */
        core.Initialize(options);

        /* 启动框架（阻塞当前线程），启动失败抛出异常（并调用各模块的Start） */
        core.Start();

        /* 线程退出阻塞后，停止框架（并调用各模块的Shutdown） */
        core.Shutdown();

        // 全局 AimRTCore 指针
        global_core_ptr_ = nullptr;
    } catch (const std::exception &e) { // 处理AimRT框架启动异常
        std::cout << "AimRT run with exception and exit. " << e.what() << std::endl;
        return -1;
    }

    std::cout << "AimRT exit." << std::endl;
    return 0;
}