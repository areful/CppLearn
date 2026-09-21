//
// Created by areful on 2026/9/21.
//
#include <chrono>
#include <iostream>
#include <memory>
#include <thread>
#include "set_console_output_cp.h"

struct Data {
    int value = 42;
};

std::weak_ptr<Data> wp;

void observer_wrong() {
    if (!wp.expired()) {
        // 检查时可能还活着...
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
        // 💥 但这一刻 wp.lock() 之前，对象可能已被释放
        auto p = wp.lock(); // 这里必须再判空！
        if (p) std::cout << "wrong 用法侥幸成功: " << p->value << "\n";
        else std::cout << "wrong 用法: expired() 是 false，但 lock() 还是失败了\n";
    }
}

void observer_right() {
    if (auto p = wp.lock()) {
        // 一步到位：要么拿到，要么没有
        std::cout << "right 用法: 安全使用 " << p->value << "\n";
    } else {
        std::cout << "right 用法: 对象已销毁，放弃\n";
    }
}

int main() {
    init_console_output();

    auto owner = std::make_shared<Data>();
    wp = owner;

    std::thread t1(observer_wrong);
    std::thread t2(observer_right);
    std::this_thread::sleep_for(std::chrono::microseconds(500));

    owner.reset(); // 主线程释放资源，和两个观察者"赛跑"

    t1.join();
    t2.join();
    return 0;
}
