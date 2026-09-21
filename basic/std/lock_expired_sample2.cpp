//
// Created by areful on 2026/9/21.
//
#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>
#include "set_console_output_cp.h"

struct Image {
    std::string name;

    explicit Image(std::string n) : name(std::move(n)) {
        std::cout << "[加载] " << name << "（模拟昂贵加载）\n";
    }

    ~Image() { std::cout << "[卸载] " << name << "\n"; }
};

class ImageCache {
public:
    // 取用：优先复用，失效则重新加载
    std::shared_ptr<Image> get(const std::string &key) {
        if (auto it = cache_.find(key); it != cache_.end()) {
            if (auto img = it->second.lock()) {
                // 还活着 -> 复用
                std::cout << "[命中缓存] " << key << "\n";
                return img;
            }
            std::cout << "[缓存失效] " << key << "，重新加载\n";
        }
        auto img = std::make_shared<Image>(key);
        cache_[key] = img; // 只存弱引用，不阻止卸载
        return img;
    }

private:
    std::unordered_map<std::string, std::weak_ptr<Image> > cache_;
};

int main() {
    init_console_output();

    ImageCache cache;

    auto a = cache.get("背景图.png"); // 第一次：加载
    auto b = cache.get("背景图.png"); // 第二次：命中缓存（a 还活着）
    std::cout << "a 和 b 是同一张图: " << std::boolalpha << (a == b) << "\n";

    a.reset(); // 释放强引用
    b.reset(); // 强计数归 0，Image 被卸载，缓存里的 weak 失效

    auto c = cache.get("背景图.png"); // weak 失效 -> lock 返回空 -> 重新加载
    (void) c;
    return 0;
}
