#include "executor_co_module/executor_co_module.h"
#include "aimrt_module_cpp_interface/co/sync_wait.h"
#include "aimrt_module_cpp_interface/co/schedule.h"
#include "aimrt_module_cpp_interface/co/inline_scheduler.h"
#include "aimrt_module_cpp_interface/co/on.h"


using namespace std::chrono_literals;

namespace example::executor::executor_co_module {

bool ExecutorCoModule::Initialize(aimrt::CoreRef core) {
    // Save aimrt framework handle
    core_ = core;

    /* 创建一个执行器协程接口类 */
    context_ = aimrt::co::AimRTContext(core_.GetExecutorManager());

    /* 获取一个 executor 句柄，命名为 "work_executor" */
    auto work_executor_ = core_.GetExecutorManager().GetExecutor("work_executor");
    AIMRT_CHECK_ERROR_THROW(work_executor_, "Can not get executor.");

    /* 获取一个线程安全 executor 句柄，命名为 "thread_safe_executor" */
    auto thread_safe_executor_ = core_.GetExecutorManager().GetExecutor("thread_safe_executor");
    AIMRT_CHECK_ERROR_THROW(thread_safe_executor_ && thread_safe_executor_.ThreadSafe(),
                            "Can not get thread_safe_executor.");

    /* 获取一个时间调度 executor 句柄，命名为 "time_schedule_executor" */
    auto time_schedule_executor_ = core_.GetExecutorManager().GetExecutor("time_schedule_executor");
    AIMRT_CHECK_ERROR_THROW(time_schedule_executor_ && thread_safe_executor_.SupportTimerSchedule(),
                            "Can not get time_schedule_executor.");


    AIMRT_INFO("Executor init succeeded.");

    return true;
}

bool ExecutorCoModule::Start() {
    AIMRT_INFO("Start executor example succeeded.");

    scope_.spawn(aimrt::co::On(aimrt::co::InlineScheduler(), SimpleExecutorDemo()));

    return true;
}

void ExecutorCoModule::Shutdown() {

    aimrt::co::SyncWait(scope_.complete());
    AIMRT_INFO("Shutdown executor example succeeded.");
}

aimrt::co::Task<void> ExecutorCoModule::SimpleExecutorDemo() {
    // 通过执行器名称获取一个调度器
    auto work_scheduler = context_.GetScheduler("work_executor");

    co_await aimrt::co::Schedule(work_scheduler);

    AIMRT_INFO("This is a simple task");   

    co_return;
}

aimrt::co::Task<void> ExecutorCoModule::ThreadSafeExecutorDemo() {
    AIMRT_INFO("This is a thread safe task");

    co_return;
}


aimrt::co::Task<void> ExecutorCoModule::TimeScheduleExecutorDemo() {
    AIMRT_INFO("This is a time schedule task");

    co_return;
}

} // namespace example::executor::executor_co_module