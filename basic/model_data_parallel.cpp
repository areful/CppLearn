#include <iostream>
#include <thread>
#include <vector>

void process(std::vector<int> &data, int start, int end) {
    for (int i = start; i < end; ++i) {
        data[i] *= 2;
    }
}

int main() {
    std::vector<int> data = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    std::vector<std::thread> threads;
    int numThreads = 4;
    int chunkSize = data.size() / numThreads;

    for (int i = 0; i < numThreads; ++i) {
        int start = i * chunkSize;
        int end = (i == numThreads - 1) ? data.size() : (i + 1) * chunkSize;
        threads.emplace_back(process, std::ref(data), start, end);
    }

    for (auto &t: threads) {
        t.join();
    }

    for (int val: data) {
        std::cout << val << " ";
    }
    std::cout << std::endl;

    return 0;
}
