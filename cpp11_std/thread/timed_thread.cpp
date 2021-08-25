#include <iostream>
#include <thread>
#include <chrono>
using namespace std;

std::timed_mutex mutex_;

void work() {
    std::chrono::milliseconds timeout(100);
    while (true) {
        if (mutex_.try_lock_for(timeout)) {
            std::cout << std::this_thread::get_id() << ": do work with the mutex" << std::endl;
            std::chrono::milliseconds sleep_dur(250);
            std::this_thread::sleep_for(sleep_dur);
            mutex_.unlock();
            std::this_thread::sleep_for(sleep_dur);
        } else {
            std::cout << std::this_thread::get_id() << ": do work without the mutex" << std::endl;
            std::chrono::milliseconds sleep_dur(100);
            std::this_thread::sleep_for(sleep_dur);
        }
    }
}

int main(int argc, char* argv[]) {
    std::thread th1(work);
    std::thread th2(work);
    std::thread th3(work);

    th1.join();
    th2.join();
    th3.join();
    return 0;
}