#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <chrono>
#include <random>

std::mutex mtx;
std::condition_variable cv;

void traveler(int id, std::vector<int> &currentCities, const std::vector<std::vector<int>> &travelPlan);

int main() {
    const int numTravelers = 4;
    std::vector<std::thread> threads;
    std::vector<std::vector<int>> travelPlan = {{1, 2, 3, 4},
                                                {1, 2, 3, 4},
                                                {1, 2, 3, 4},
                                                {1, 2, 3, 4},};
    std::vector<int> currentCities(numTravelers, 0);

    for (int i = 0; i < numTravelers; ++i) {
        threads.emplace_back(traveler, i, std::ref(currentCities), std::ref(travelPlan));
        //// same as below:
        // std::thread th(traveler, i, std::ref(currentCities), std::ref(travelPlan));
        // threads.emplace_back(std::move(th));
    }

    for (auto &t: threads) {
        t.join();
    }

    return 0;
}

void traveler(int id, std::vector<int> &currentCities, const std::vector<std::vector<int>> &travelPlan) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(500, 2000);

    for (size_t i = 0; i < travelPlan[id].size(); ++i) {
        std::this_thread::sleep_for(std::chrono::milliseconds(dis(gen))); // 模拟旅行者到达下一个城市的时间

        {
            std::unique_lock<std::mutex> lock(mtx);
            std::cout << "Traveler " << id << " arrived at city " << currentCities[id] << std::endl;

            currentCities[id] = travelPlan[id][i];
            cv.notify_all(); // 通知其他旅行者
            cv.wait(lock, [id, &currentCities, &travelPlan, &i]() { // 等待其他旅行者到达当前城市
                for (size_t j = 0; j < travelPlan.size(); ++j) {
                    if (j != id && currentCities[j] != travelPlan[j][i]) {
                        return false;
                    }
                }
                return true;
            });
        }
    }
}
