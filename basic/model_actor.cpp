#include <iostream>
#include <vector>
#include <thread>
#include <mutex>

class Actor {
public:
    void sendMessage(const std::string &msg) {
        std::lock_guard<std::mutex> lock(mutex_);
        std::cout << "Received message: " << msg << std::endl;
    }

private:
    std::mutex mutex_;
};

int main() {
    Actor actor;
    std::vector<std::thread> threads;

    threads.reserve(5);
    for (int i = 0; i < 5; ++i) {
        threads.emplace_back([&actor, i]() {
            actor.sendMessage("Message " + std::to_string(i));
        });
    }

    for (auto &t: threads) {
        t.join();
    }

    return 0;
}
