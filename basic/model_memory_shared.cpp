//
// Created by jian.gao on 2024-4-9.
//
#include <iostream>
#include <thread>
#include <mutex>
#include <vector>

std::mutex mtx;
int sharedData = 0;

void increment() {
    for (int i = 0; i < 1000; ++i) {
        std::lock_guard<std::mutex> lock(mtx);
        ++sharedData;
    }
}

int main() {
    std::vector<std::thread> threads;
    for (int i = 0; i < 10; ++i) {
        threads.push_back(std::thread(increment));
    }

    for (auto& t : threads) {
        t.join();
    }

    std::cout << "Shared data: " << sharedData << std::endl;

    return 0;
}
