//
// Created by areful on 2026/9/22.
//
#include <iostream>
#include <memory>
#include <vector>
#include "set_console_output_cp.h"

class Task : public std::enable_shared_from_this<Task> {
private:
    struct PassKey {
        // 私有嵌套类型，外部无法命名它
        explicit PassKey() = default;
    };

    int id_;

public:
    // 构造需要这把钥匙 -> 外部拿不到钥匙，等于还是私有的
    explicit Task(int id, PassKey) : id_(id) {
    }

    void registerSelf(std::vector<std::shared_ptr<Task>>& pool) {
        pool.push_back(shared_from_this());
    }

    static std::shared_ptr<Task> create(int id) {
        return std::make_shared<Task>(id, PassKey{}); // make_shared 转发，OK
    }
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
