//
// Created by areful on 2026/9/21.
//
#include <iostream>
#include <memory>
#include "set_console_output_cp.h"

int main() {
    init_console_output();

    auto p = std::make_shared<int>(1);
    p.reset(new int(100)); // 旧 int(1) 销毁（若没别人引用），p 现在管 100
    p.reset(); // p 变空，资源释放
    if (!p) std::cout << "p is null\n";

    // 典型用法：提前释放大资源 / 打破循环（见下文）
    return 0;
}
