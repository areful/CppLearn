#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <algorithm>

class Traveler {
public:
    std::string name;
    std::vector<int> route;
    size_t currentPoint;

    Traveler(const std::string &name, const std::vector<int> &route)
            : name(name), route(route), currentPoint(0) {}

    // 判断旅行者是否到达终点
    bool isAtEnd() const {
        return currentPoint >= route.size();
    }

    // 模拟旅行者到达下一个点
    void travelToNextPoint() {
        if (currentPoint < route.size()) {
            int travelTime = route[currentPoint];
            std::cout << name << " 开始从点 " << currentPoint
                      << " 出发，预计耗时 " << travelTime << " 秒\n";
            std::flush(std::cout);
            std::this_thread::sleep_for(std::chrono::seconds(travelTime));
            currentPoint++;
            std::cout << name << " 到达点 " << currentPoint << "\n";
            std::flush(std::cout);
        }
    }
};

// 同步旅行逻辑
void syncTravel(std::vector<Traveler> &travelers) {
    std::mutex mtx;
    std::condition_variable cv;
    std::atomic<int> readyCount(0);

    while (true) {
        readyCount = 0;

        // 创建线程池，让所有旅行者并发移动
        std::vector<std::thread> threads;
        for (auto &traveler: travelers) {
            if (!traveler.isAtEnd()) {
                threads.emplace_back([&traveler, &readyCount, &mtx, &cv]() {
                    traveler.travelToNextPoint();
                    {
                        std::lock_guard<std::mutex> lock(mtx);
                        readyCount++;
                    }
                    cv.notify_all();
                });
            }
        }

        // 等待所有旅行者到达当前点
        {
            std::unique_lock<std::mutex> lock(mtx);
            cv.wait(lock, [&]() { return readyCount == threads.size(); });
        }

        // 等待所有线程完成
        for (auto &thread: threads) {
            if (thread.joinable()) {
                thread.join();
            }
        }

        std::cout << "所有旅行者已到达点 " << travelers.front().currentPoint << "\n";
        std::flush(std::cout);

        // 检查是否所有旅行者都到达终点
        if (std::all_of(travelers.begin(), travelers.end(), [](const Traveler &t) { return t.isAtEnd(); })) {
            break;
        }
    }

    std::cout << "所有旅行者已到达目的地！\n";
}

int main() {
    // 定义旅行者及其路径
    std::vector<Traveler> travelers = {
            {"旅行者A", {2, 3, 1}},
            {"旅行者B", {1, 2, 3}},
            {"旅行者C", {3, 1, 2}},
    };

    // 开始同步旅行
    syncTravel(travelers);
    return 0;
}
