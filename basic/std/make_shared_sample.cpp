//
// Created by areful on 2026/9/21.
//
#include <iostream>
#include <memory>
#include <string>
#include "set_console_output_cp.h"

struct Widget {
    std::string name;

    explicit Widget(std::string n) : name(std::move(n)) {
        std::cout << "构造 " << name << "\n";
    }

    ~Widget() { std::cout << "析构 " << name << "\n"; }
};

int main() {
    init_console_output();

    auto w = std::make_shared<Widget>("hello"); // 完美转发参数给构造函数
    return 0;
}
