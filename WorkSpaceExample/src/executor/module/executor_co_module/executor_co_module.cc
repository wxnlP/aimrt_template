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
    ctx_ = aimrt::co::AimRTContext(core_.GetExecutorManager());

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


    /* 启动三个协程，并在当前执行器（主线程）执行 */
    scope_.spawn(aimrt::co::On(aimrt::co::InlineScheduler(), SimpleExecutorDemo()));
    scope_.spawn(aimrt::co::On(aimrt::co::InlineScheduler(), ThreadSafeExecutorDemo()));
    scope_.spawn(aimrt::co::On(aimrt::co::InlineScheduler(), TimeScheduleExecutorDemo()));

    AIMRT_INFO("Start executor example succeeded.");

    return true;
}

void ExecutorCoModule::Shutdown() {
    run_flag_ = false;
    // 阻塞式等到所有协程结束
    aimrt::co::SyncWait(scope_.complete());

    AIMRT_INFO("Shutdown executor example succeeded.");
}

aimrt::co::Task<void> ExecutorCoModule::SimpleExecutorDemo() {
    // 通过执行器名称获取调度器接口句柄
    auto work_scheduler = ctx_.GetScheduler("work_executor");

    // 挂起协程，通过 work_scheduler 将后续程序执行权移交至 work_executor 执行器
    co_await aimrt::co::Schedule(work_scheduler);

    AIMRT_INFO("This is a simple task");   

    co_return;
}

aimrt::co::Task<void> ExecutorCoModule::ThreadSafeExecutorDemo() {
    AIMRT_INFO("This is a thread safe task");

    // 通过执行器名称获取调度器接口句柄
    auto thread_safe_scheduler = ctx_.GetScheduler("thread_safe_executor");
    
    aimrt::co::AsyncScope scope;

    // 定义一个函数，由函数指针指向
    uint32_t n = 0;
    auto task = [&n]() -> aimrt::co::Task<void> {
        n++;
        co_return;
    };

    for (int i = 0; i < 1000; i++) {
        
        scope.spawn(aimrt::co::On(thread_safe_scheduler, task()));
    }

    // 等待所有任务结束
    co_await aimrt::co::On(aimrt::co::InlineScheduler(), scope.complete());

    AIMRT_INFO("Value of n is {}", n);

    co_return;
}


aimrt::co::Task<void> ExecutorCoModule::TimeScheduleExecutorDemo() {
    AIMRT_INFO("Start loop executor");

    // 通过执行器名称获取调度器接口句柄
    auto time_scheduler = ctx_.GetScheduler("time_schedule_executor");

    // 挂起协程，通过 time_scheduler 将后续程序执行权移交至 time_schedule_executor 执行器
    co_await aimrt::co::Schedule(time_scheduler);

    uint32_t count = 0;
    while (run_flag_)
    {
        count++;
        AIMRT_INFO("Loop count : {} ⭐⭐⭐", count);
        // 挂起当前协程，1秒后由 time_scheduler 恢复
        co_await aimrt::co::ScheduleAfter(time_scheduler, std::chrono::seconds(1));
    }
    
    AIMRT_INFO("Exit loop executor");

    co_return;
}

} // namespace example::executor::executor_co_module