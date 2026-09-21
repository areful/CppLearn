//
// Created by areful on 2026/9/21.
//
#include <iostream>
#include <memory>

#include "set_console_output_cp.h"

int main() {
    init_console_output();
    std::shared_ptr<int> p1 = std::make_shared<int>(42); // 推荐写法
    std::shared_ptr<int> p2 = p1; // 拷贝，计数 1 -> 2

    std::cout << *p1 << "\n"; // 42
    std::cout << p1.use_count() << "\n"; // 2

    p2.reset(); // p2 放弃所有权，计数 2 -> 1
    std::cout << p1.use_count() << "\n"; // 1

    // p1 离开作用域，计数归 0，int 被自动释放
    return 0;
}
