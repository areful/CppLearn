//
// Created by areful on 2026/9/21.
//
#include <iostream>
#include <string>
#include <eventpp/eventdispatcher.h>      // 同步派发

#include "set_console_output_cp.h"

template<typename Key, typename Arg>
class EventBus {
public:
    template<typename F>
    void on(const Key &key, F &&f) {
        dispatcher_.appendListener(key, std::forward<F>(f));
    }

    void emit(const Key &key, const Arg &arg) {
        dispatcher_.dispatch(key, arg);
    }

private:
    eventpp::EventDispatcher<Key, void(const Arg &)> dispatcher_;
};

// ============================================================
int main() {
    init_console_output();

    EventBus<int, std::string> bus;
    bus.on(1, [](const std::string &msg) {
        std::cout << "  [listener A] " << msg << "\n";
    });
    bus.emit(1, "hello");

    std::cout << "\n===== all done =====\n";
    return 0;
}
