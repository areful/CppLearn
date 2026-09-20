//
// Created by areful on 2026/9/20.
//
#include <iostream>
#include <string>
#include <thread>
#include <chrono>
#include <atomic>
#include <memory>

#include <eventpp/eventdispatcher.h>      // 同步派发
#include <eventpp/eventqueue.h>           // 异步队列（跨线程）
#include <eventpp/utilities/scopedremover.h>   // 作用域自动移除监听
#include <eventpp/utilities/counterremover.h>  // 监听 N 次后自动移除

#include "set_console_output_cp.h"

// ============================================================
// 场景 1：同步 EventDispatcher（发布者立即触发所有监听者）
// ============================================================
void demoDispatcher() {
    std::cout << "\n===== 1. EventDispatcher（同步） =====\n";

    // 事件类型 int，回调原型 void(const std::string&)
    eventpp::EventDispatcher<int, void(const std::string &)> dispatcher;

    // 同一个事件可以挂多个监听器，按添加顺序执行
    dispatcher.appendListener(1, [](const std::string &msg) {
        std::cout << "  [listener A] " << msg << "\n";
    });
    dispatcher.appendListener(1, [](const std::string &msg) {
        std::cout << "  [listener B] " << msg << "\n";
    });

    // prependListener 插到最前面
    dispatcher.prependListener(1, [](const std::string &msg) {
        std::cout << "  [listener C, prepended] " << msg << "\n";
    });

    // 派发：C -> A -> B 依次执行
    dispatcher.dispatch(1, "hello dispatcher");
}

// ============================================================
// 场景 2：CounterRemover —— 监听固定次数后自动移除
// ============================================================
void demoCounterRemover() {
    std::cout << "\n===== 2. CounterRemover（自动移除） =====\n";

    eventpp::EventDispatcher<int, void()> dispatcher;

    // 构造函数只传 dispatcher
    eventpp::CounterRemover<
        eventpp::EventDispatcher<int, void()>
    > remover(dispatcher);

    // 触发次数 2 是 appendListener 的第 3 个参数
    remover.appendListener(1, []() {
        std::cout << "  fired!\n";
    }, 2);

    dispatcher.dispatch(1); // 第 1 次触发
    dispatcher.dispatch(1); // 第 2 次触发，之后自动移除
    dispatcher.dispatch(1); // 不再触发
    std::cout << "  (第三次派发已无监听者)\n";
}

// ============================================================
// 场景 3：ScopedRemover —— 离开作用域自动移除，避免悬挂监听
// ============================================================
void demoScopedRemover() {
    std::cout << "\n===== 3. ScopedRemover（RAII） =====\n";

    eventpp::EventDispatcher<int, void()> dispatcher;

    {
        // 进入作用域时挂上监听
        eventpp::ScopedRemover<
            eventpp::EventDispatcher<int, void()>
        > scoped(dispatcher);

        scoped.appendListener(1, []() {
            std::cout << "  inside scope: fired\n";
        });

        dispatcher.dispatch(1);
    } // 离开作用域自动移除，即使发生异常也安全

    dispatcher.dispatch(1); // 不再触发
    std::cout << "  (离开作用域后派发，已无监听者)\n";
}

// ============================================================
// 场景 4：EventQueue 跨线程异步 —— 生产者/消费者模型
// ============================================================
struct LogEvent {
    std::chrono::steady_clock::time_point ts;
    std::string level;
    std::string message;
};

void demoEventQueue() {
    std::cout << "\n===== 4. EventQueue（跨线程异步） =====\n";

    // 异步队列：enqueue 只是把事件和参数拷贝进队列，立即返回
    eventpp::EventQueue<int, void(std::shared_ptr<LogEvent>)> queue;

    // 消费者线程：不断从队列取事件并处理
    std::atomic<bool> running{true};
    std::thread consumer([&]() {
        // processUntil：队列非空或条件满足时持续处理
        queue.processUntil([&]() { return !running.load(); });

        // 退出前清空剩余事件
        while (!queue.emptyQueue()) {
            queue.processOne();
        }
    });

    // 生产者线程：模拟产生 5 条日志
    std::thread producer([&]() {
        for (int i = 0; i < 5; ++i) {
            auto evt = std::make_shared<LogEvent>(LogEvent{
                std::chrono::steady_clock::now(),
                i % 2 == 0 ? "INFO" : "WARN",
                "log message #" + std::to_string(i)
            });
            queue.enqueue(1, evt); // 拷贝 shared_ptr 进队列，线程安全
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
        }
    });

    // 主线程注册监听（可以晚于生产者启动，enqueue 是安全的）
    queue.appendListener(1, [](const std::shared_ptr<LogEvent> &e) {
        std::cout << "  [" << e->level << "] " << e->message << "\n";
    });

    producer.join();
    running = false;
    consumer.join();
}

// ============================================================
int main() {
    init_console_output();

    demoDispatcher();
    demoCounterRemover();
    demoScopedRemover();
    demoEventQueue();

    std::cout << "\n===== all done =====\n";
    return 0;
}
