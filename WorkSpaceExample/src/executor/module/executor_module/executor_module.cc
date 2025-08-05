#include "executor_module/executor_module.h"
#include "util/log_util.h"

using namespace std::chrono_literals;

namespace example::executor::executor_module {

bool ExecutorModule::Initialize(aimrt::CoreRef core) {
    // Save aimrt framework handle
    core_ = core;


    /* 获取一个 executor 句柄，命名为 "work_executor" */
    work_executor_ = core_.GetExecutorManager().GetExecutor("work_executor");
    AIMRT_CHECK_ERROR_THROW(work_executor_, "Can not get executor.");

    /* 获取一个线程安全 executor 句柄，命名为 "thread_safe_executor" */
    thread_safe_executor_ = core_.GetExecutorManager().GetExecutor("thread_safe_executor");
    AIMRT_CHECK_ERROR_THROW(thread_safe_executor_ && thread_safe_executor_.ThreadSafe(),
                            "Can not get thread_safe_executor.");

    /* 获取一个时间调度 executor 句柄，命名为 "time_schedule_executor" */
    time_schedule_executor_ = core_.GetExecutorManager().GetExecutor("time_schedule_executor");
    AIMRT_CHECK_ERROR_THROW(time_schedule_executor_ && thread_safe_executor_.SupportTimerSchedule(),
                            "Can not get time_schedule_executor.");


    AIMRT_INFO("Executor init succeeded.");

    return true;
}

bool ExecutorModule::Start() {
    AIMRT_INFO("Start executor example succeeded.");

    SimpleExecutorDemo();
    ThreadSafeExecutorDemo();
    TimeScheduleExecutorDemo();

    return true;
}

void ExecutorModule::Shutdown() {
    AIMRT_INFO("Shutdown executor example succeeded.");
}

void ExecutorModule::SimpleExecutorDemo() {
    // 向执行器投递一个 lamada 表达式的函数任务
    work_executor_.Execute([this](){
        AIMRT_INFO("This is a simple executor.");
    });
}

void ExecutorModule::ThreadSafeExecutorDemo() {
    uint32_t n = 0;
    // 向执行器投递多个任务
    // 若执行器是线程安全的，那么无需互斥锁即可保证线程安全
    for(uint32_t i = 0; i < 1000; i++) {
        thread_safe_executor_.Execute([&n](){
            n++;
        });
    }

    std::this_thread::sleep_for(1s);

    AIMRT_INFO("Vaule n is {}", n);
}

void ExecutorModule::TimeScheduleExecutorDemo() {
    if (!run_flag_) { return; }

    AIMRT_INFO("Loop count: {}", loop_count_++);

    // 套娃1s定时循环
    time_schedule_executor_.ExecuteAfter(
        1s, std::bind(&ExecutorModule::TimeScheduleExecutorDemo, this)
    );
}

} // namespace example::executor::executor_module