//
// Created by areful on 2026/9/20.
//
#include <iostream>
#include <eventpp/eventqueue.h>

int main() {
    eventpp::EventQueue<int, void(const std::string&)> queue;

    queue.appendListener(1, [](const std::string& msg) {
        std::cout << "event 1: " << msg << "\n";
    });

    queue.appendListener(1, [](const std::string& msg) {
        std::cout << "another listener: " << msg << "\n";
    });

    queue.enqueue(1, "hello eventpp");
    queue.process();

    return 0;
}