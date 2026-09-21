//
// Created by areful on 2026/9/21.
//
#include <iostream>
#include <string>
#include <utility>

#include "eventpp/eventdispatcher.h"

class TypedBus {
public:
    // Meyers 单例：C++11 起局部静态变量初始化是线程安全的
    static TypedBus &instance() {
        static TypedBus bus;
        return bus;
    }

    // 禁止拷贝和赋值
    TypedBus(const TypedBus &) = delete;

    TypedBus &operator=(const TypedBus &) = delete;

    template<typename EventT, typename F>
    void on(F &&f) {
        dispatcherFor<EventT>().appendListener(0, std::forward<F>(f));
    }

    template<typename EventT>
    void emit(const EventT &e) {
        dispatcherFor<EventT>().dispatch(0, e);
    }

private:
    TypedBus() = default;

    ~TypedBus() = default;

    template<typename EventT>
    static eventpp::EventDispatcher<int, void(const EventT &)> &
    dispatcherFor() {
        static eventpp::EventDispatcher<int, void(const EventT &)> d;
        return d;
    }
};

int main() {
    struct LoginEvent {
        std::string user;
    };
    struct PurchaseEvent {
        int amount;
    };

    TypedBus::instance().on<LoginEvent>([](const LoginEvent &e) {
        std::cout << "login: " << e.user << "\n";
    });

    TypedBus::instance().on<PurchaseEvent>([](const PurchaseEvent &e) {
        std::cout << "purchase: " << e.amount << "\n";
    });

    TypedBus::instance().emit(LoginEvent{"alice"});
    TypedBus::instance().emit(PurchaseEvent{99});

    return 0;
}
