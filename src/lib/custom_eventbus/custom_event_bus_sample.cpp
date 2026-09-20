//
// Created by areful on 2026/9/20.
//
#include <iostream>
#include <chrono>
#include <thread>
#include "custom_event_bus.hpp"
#include "set_console_output_cp.h"

// ===================== 定义事件类型 =====================

struct LoginEvent {
    std::string username;
    int user_id;
};

struct LogoutEvent {
    std::string username;
    std::chrono::system_clock::time_point timestamp;
};

struct DataUpdateEvent {
    int entity_id;
    double new_value;
};

// ===================== 业务模块示例 =====================

class LoggerModule {
public:
    void init(evt::EventBus &bus) {
        // 订阅登录事件（高优先级，先记录）
        sub_login_ = bus.subscribe<LoginEvent>(
            [](const LoginEvent &e) {
                std::cout << "[Logger] " << e.username << " (id=" << e.user_id
                        << ") logged in\n";
            },
            evt::Priority::HIGH
        );

        // 订阅登出事件（普通优先级）
        sub_logout_ = bus.subscribe<LogoutEvent>(
            [](const LogoutEvent &e) {
                std::cout << "[Logger] " << e.username << " logged out\n";
            }
        );
    }

    void shutdown() {
        sub_login_.cancel();
        sub_logout_.cancel();
    }

private:
    evt::Subscription sub_login_;
    evt::Subscription sub_logout_;
};

class AnalyticsModule {
public:
    void init(evt::EventBus &bus) {
        // 使用全局单例总线也可以：evt::global_bus()
        sub_ = bus.subscribe<LoginEvent>(
            [](const LoginEvent &e) {
                std::cout << "[Analytics] Tracking login: " << e.username << "\n";
            },
            evt::Priority::NORMAL // 比 Logger 低，后执行
        );
    }

private:
    evt::Subscription sub_;
};

// ===================== 主函数 =====================

int main() {
    init_console_output();

    evt::EventBus bus;

    LoggerModule logger;
    AnalyticsModule analytics;
    logger.init(bus);
    analytics.init(bus);

    std::cout << "===== 同步发布 =====\n";
    bus.publish(LoginEvent{"Alice", 1001});
    // 预期输出顺序：
    // [Logger] Alice (id=1001) logged in      <- HIGH 优先级
    // [Analytics] Tracking login: Alice       <- NORMAL 优先级

    std::cout << "\n===== 异步发布 =====\n";
    bus.publish_async(LoginEvent{"Bob", 1002});
    bus.publish_async(DataUpdateEvent{42, 3.14});

    // 等待异步工作线程处理完毕
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    std::cout << "\n===== 取消订阅后再次发布 =====\n";
    logger.shutdown(); // 取消 Logger 的所有订阅
    bus.publish(LoginEvent{"Charlie", 1003});
    // 预期：只有 Analytics 输出，Logger 不再输出

    std::cout << "\n===== 使用全局单例总线 =====\n";
    auto sub = evt::global_bus().subscribe<DataUpdateEvent>(
        [](const DataUpdateEvent &e) {
            std::cout << "[GlobalBus] Entity " << e.entity_id
                    << " updated to " << e.new_value << "\n";
        }
    );
    evt::global_bus().publish(DataUpdateEvent{99, 2.718});
    sub.cancel();

    std::cout << "\n===== 清理失效订阅 =====\n";
    bus.gc(); // 清理 logger 取消后留下的失效句柄
    std::cout << "GC done.\n";

    return 0;
}
