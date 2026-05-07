#include <iostream>
#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>

std::mutex mtx;
std::queue<int> messages;
std::condition_variable cv;

void sender() {
    for (int i = 0; i < 5; ++i) {
        {
            std::lock_guard<std::mutex> lock(mtx);
            messages.push(i);
        }
        cv.notify_one();
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
}

void receiver() {
    for (int i = 0; i < 5; ++i) {
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, []{ return !messages.empty(); });
        int msg = messages.front();
        messages.pop();
        lock.unlock();
        std::cout << "Received message: " << msg << std::endl;
    }
}

int main() {
    std::thread t1(sender);
    std::thread t2(receiver);

    t1.join();
    t2.join();

    return 0;
}
