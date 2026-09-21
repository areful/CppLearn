//
// Created by areful on 2026/9/21.
//
#include <iostream>
#include <memory>
#include "set_console_output_cp.h"

struct Resource {
    int id;

    explicit Resource(int i) : id(i) {
        std::cout << "[构造] Resource " << id << "\n";
    }

    ~Resource() {
        std::cout << "[析构] Resource " << id << "\n";
    }
};

int main() {
    init_console_output();

    std::weak_ptr<Resource> wp; // 空的 weak_ptr

    {
        auto sp = std::make_shared<Resource>(1);
        wp = sp; // weak_ptr 观察它，强引用计数不变（仍是 1）

        std::cout << "--- 对象还活着 ---\n";
        std::cout << "expired() = " << std::boolalpha << wp.expired() << "\n"; // false
        std::cout << "use_count() = " << wp.use_count() << "\n"; // 1

        // lock() 成功：返回一个 shared_ptr，强引用 +1
        if (auto locked = wp.lock()) {
            std::cout << "lock() 成功，locked->id = " << locked->id << "\n";
            std::cout << "lock 期间 use_count() = " << wp.use_count() << "\n"; // 2
        } // locked 离开 if 作用域，计数回落
        std::cout << "lock 之后 use_count() = " << wp.use_count() << "\n"; // 1
    } // sp 离开作用域，强计数归 0，Resource 在这里被析构

    std::cout << "--- 对象已销毁 ---\n";
    std::cout << "expired() = " << wp.expired() << "\n"; // true
    std::cout << "use_count() = " << wp.use_count() << "\n"; // 0

    // lock() 失败：返回空的 shared_ptr
    if (auto locked = wp.lock()) {
        std::cout << "不会走到这里\n";
    } else {
        std::cout << "lock() 失败，返回空 shared_ptr\n";
    }

    // weak_ptr 本身还活着，只是观察的对象没了
    std::cout << "wp 仍存在，但它指向的资源已释放\n";

    wp.reset(); // weak_ptr 自己也可以置空
    return 0;
}
