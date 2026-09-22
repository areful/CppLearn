//
// Created by areful on 2026/9/22.
//
#include <iostream>
#include <memory>
#include <vector>
#include "set_console_output_cp.h"

class Task : public std::enable_shared_from_this<Task> {
public:
    static std::shared_ptr<Task> create(int id) {
        return std::shared_ptr<Task>(new Task(id)); // ✅ new 在类内部，合法
    }

    ~Task() { std::cout << "[析构] Task " << id_ << "\n"; }

    void registerSelf(std::vector<std::shared_ptr<Task> > &pool) {
        pool.push_back(shared_from_this());
    }

private:
    explicit Task(int id) : id_(id) {
        std::cout << "[构造] Task " << id_ << "\n";
    }

    int id_;
};

int main() {
    init_console_output();

    // Task t(1);              // 💥 编译错误：构造函数是私有的
    // Task* p = new Task(1);  // 💥 编译错误

    auto t = Task::create(42); // 唯一合法出生方式：直接就是 shared_ptr
    std::cout << "use_count() = " << t.use_count() << "\n"; // 1

    std::vector<std::shared_ptr<Task> > pool;
    t->registerSelf(pool);
    std::cout << "use_count() = " << t.use_count() << "\n"; // 2

    return 0;
}
