//
// Created by areful on 2026/9/21.
//
#include <iostream>
#include <memory>
#include "set_console_output_cp.h"

struct Node {
    int value;
    std::shared_ptr<Node> next; // 拥有下一个
    std::weak_ptr<Node> prev; // 只观察上一个（反向用 weak！）
    explicit Node(int v) : value(v) {
    }

    ~Node() { std::cout << "销毁 " << value << "\n"; }
};

int main() {
    init_console_output();

    auto a = std::make_shared<Node>(1);
    auto b = std::make_shared<Node>(2);
    a->next = b;
    b->prev = a; // weak_ptr，计数不变

    a.reset(); // 即使 a 释放，b->prev 也不会阻止任何析构
    b.reset(); // 离开作用域后全部正常销毁

    return 0;
}
