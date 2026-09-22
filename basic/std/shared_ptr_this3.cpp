//
// Created by areful on 2026/9/22.
//
#include <iostream>
#include <memory>
#include <vector>
#include <algorithm>
#include "set_console_output_cp.h"

class Task : public std::enable_shared_from_this<Task> {
public:
    static std::shared_ptr<Task> create(int id) {
        return std::shared_ptr<Task>(new Task(id));
    }

    void registerSelf(std::vector<std::shared_ptr<Task> > &pool) {
        pool.push_back(shared_from_this());
    }

    // 配套的注销：把自己从 pool 里移除，引用即释放
    void unregisterSelf(std::vector<std::shared_ptr<Task> > &pool) {
        auto it = std::find_if(pool.begin(), pool.end(),
                               [this](const std::shared_ptr<Task> &p) { return p.get() == this; });
        if (it != pool.end()) {
            pool.erase(it); // 元素销毁，shared_ptr 析构，引用 -1
        }
    }

    ~Task() { std::cout << "[析构] Task " << id_ << "\n"; }

private:
    explicit Task(int id) : id_(id) { std::cout << "[构造] Task " << id_ << "\n"; }
    int id_;
};

/*
    | 需求         | 写法                                             | 效果      |
    | ---------- | ---------------------------------------------- | ------- |
    | 对象主动退出池子   | `unregisterSelf`（`erase` 匹配 `p.get() == this`） | 只减自己的那份 |
    | 池子整个生命周期结束 | `pool.clear()`                                 | 全部释放    |
    | 按位置移除      | `pool.erase(pos)`                              | 移除指定元素  |
    | 元素置空但保留槽位  | `pool[i].reset()`                              | 单个释放    |
*/
int main() {
    init_console_output();

    std::vector<std::shared_ptr<Task> > pool;

    auto t = Task::create(42);
    t->registerSelf(pool);
    std::cout << "register 后 use_count = " << t.use_count() << "\n"; // 2

    // ---- 方式一：对象自己注销（最常用，配对出现）----
    t->unregisterSelf(pool);
    std::cout << "unregister 后 use_count = " << t.use_count() << "\n"; // 1

    // ---- 方式二：清空整个池子 ----
    t->registerSelf(pool);
    pool.clear(); // 所有元素析构，引用全部释放
    std::cout << "clear 后 use_count = " << t.use_count() << "\n"; // 1

    // ---- 方式三：erase 指定位置 ----
    t->registerSelf(pool);
    pool.erase(pool.begin()); // 移除第一个元素
    std::cout << "erase 后 use_count = " << t.use_count() << "\n"; // 1

    // ---- 方式四：单独置空某个元素（不挪动容器）----
    t->registerSelf(pool);
    pool[0].reset(); // 该元素释放所有权
    std::cout << "reset 后 use_count = " << t.use_count() << "\n"; // 1

    return 0;
    // t 离开作用域，计数归 0，Task 析构
}
