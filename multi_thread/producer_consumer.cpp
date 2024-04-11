#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>

std::mutex mtx;
std::condition_variable cv;
std::queue<int> buffer;
const int bufferSize = 10;

void producer() {
    for (int i = 0; i < 20; ++i) {
        {
            std::unique_lock<std::mutex> lock(mtx);
            // 等待缓冲区不满
            cv.wait(lock, [](){ return buffer.size() < bufferSize; });
            // 生产物品并放入缓冲区
            buffer.push(i);
            std::cout << "Produced: " << i << std::endl;
        }
        cv.notify_all(); // 通知消费者可以消费了
        std::this_thread::sleep_for(std::chrono::milliseconds(500)); // 休眠500ms
    }
}

void consumer() {
    for (int i = 0; i < 20; ++i) {
        {
            std::unique_lock<std::mutex> lock(mtx);
            // 等待缓冲区不空
            cv.wait(lock, [](){ return !buffer.empty(); });
            // 消费物品
            int val = buffer.front();
            buffer.pop();
            std::cout << "Consumed: " << val << std::endl;
        }
        cv.notify_all(); // 通知生产者可以生产了
        std::this_thread::sleep_for(std::chrono::milliseconds(1000)); // 休眠1s
    }
}

int main() {
    std::thread producerThread(producer);
    std::thread consumerThread(consumer);

    producerThread.join();
    consumerThread.join();

    return 0;
}
