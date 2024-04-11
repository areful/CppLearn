#include <iostream>
#include <thread>

void task() {
    std::cout << "Thread ID: " << std::this_thread::get_id() << std::endl;
}

int main() {
    std::thread t1(task);
    std::thread t2(task);

    t1.join();
    t2.join();

    return 0;
}
