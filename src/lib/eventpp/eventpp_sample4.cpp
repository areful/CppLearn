//
// Created by areful on 2026/9/21.
//
#include <iostream>
#include <string>
#include <eventpp/eventdispatcher.h>

#include "set_console_output_cp.h"

class TypedBus {
public:
    template<typename EventT, typename F>
    void on(F &&f) {
        dispatcherFor<EventT>().appendListener(0, std::forward<F>(f));
    }

    template<typename EventT>
    void emit(const EventT &e) {
        dispatcherFor<EventT>().dispatch(0, e);
    }

private:
    template<typename EventT>
    static eventpp::EventDispatcher<int, void(const EventT &)> &
    dispatcherFor() {
        static eventpp::EventDispatcher<int, void(const EventT &)> d;
        return d;
    }
};

// ============================================================
int main() {
    init_console_output();

    struct LoginEvent {
        std::string user;
    };
    struct PurchaseEvent {
        int amount;
    };

    TypedBus bus;
    bus.on<LoginEvent>([](const LoginEvent &e) {
        std::cout << "  [listener LoginEvent] " << e.user << "\n";
    });
    bus.on<PurchaseEvent>([](const PurchaseEvent &e) {
        std::cout << "  [listener PurchaseEvent] " << e.amount << "\n";
    });

    bus.emit(LoginEvent{"alice"});
    bus.emit(PurchaseEvent{1997});

    return 0;
}
