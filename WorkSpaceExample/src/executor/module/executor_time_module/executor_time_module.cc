#include "executor_time_module/executor_time_module.h"


// 支持使用字面量后缀来简化时间单位
using namespace std::chrono_literals;

namespace example::executor::executor_time_module {

bool ExecutorTimeModule::Initialize(aimrt::CoreRef core) {
    // Save aimrt framework handle
    core_ = core;

    timer_executor_ = core_.GetExecutorManager().GetExecutor("timer_executor");
    AIMRT_CHECK_ERROR_THROW(timer_executor_, "can not get timer_executor.");
    AIMRT_CHECK_ERROR_THROW(timer_executor_.SupportTimerSchedule(), 
                            "timer_executor is not a timer enabled executor.");

    AIMRT_INFO("Executor init succeeded.");

    return true;
}

bool ExecutorTimeModule::Start() {
    // 记录开始时间
    auto start_time = timer_executor_.Now();

    // 创建一个lamada式定时回调函数
    auto timer_callback = [this, start_time](aimrt::executor::TimerBase& timer) {
        static uint32_t count = 0;

        // 读取执行器当前的时间
        auto now = timer.Executor().Now();
        // 计算与开始时间的时间间隔（ms单位）
        auto timepoint = std::chrono::duration_cast<std::chrono::milliseconds>(now - start_time).count();
        
        AIMRT_INFO("Executed {} times, execute timepoint: {} ms", ++count, timepoint);

        if (count >= 10) {
            // 取消定时器（设置 cancel 状态）
            timer.Cancel();
            AIMRT_INFO("Timer cancelled at timepoint: {} ms", timepoint);
        }
    };

    // 创建一个 100ms 为周期的定时器
    timer_ = aimrt::executor::CreateTimer(timer_executor_, 100ms, std::move(timer_callback));

    AIMRT_INFO("Timer created at timepoint: 0 ms");

    // 350ms后重置定时器（会取消 cancel 状态，并重置下次执行时间，下一次执行时间会基于当前时间加上周期计算得出。）
    timer_executor_.ExecuteAfter(350ms, [this]() {
        timer_->Reset();
        AIMRT_INFO("Timer reset at timepoint: 350 ms");
    });

    // 600ms后重置定时器（会取消 cancel 状态，并重置下次执行时间，下一次执行时间会基于当前时间加上周期计算得出。）
    // 600ms不会触发回调函数，而是直接重置定时器
    timer_executor_.ExecuteAfter(600ms, [this]() {
        timer_->Reset();
        AIMRT_INFO("Timer reset at timepoint: 600 ms");
    });

    AIMRT_INFO("Start executor example succeeded.");

    return true;
}

void ExecutorTimeModule::Shutdown() {
    timer_->Cancel();

    AIMRT_INFO("Shutdown executor example succeeded.");
}


} // namespace example::executor::executor_time_module