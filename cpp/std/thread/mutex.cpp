#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>
using namespace std;

std::mutex g_lock;

void func() {
    g_lock.lock();
    std::cout << "entered thread " << std::this_thread::get_id() << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cout << "leaving thread " << std::this_thread::get_id() << std::endl;
    g_lock.unlock();
}
void func1() {
    std::lock_guard<std::mutex> lock_guard(g_lock);
    std::cout << "entered thread " << std::this_thread::get_id() << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cout << "leaving thread " << std::this_thread::get_id() << std::endl;
}

int main(int argc, char* argv[]) {
    /*
     * 01 mutex 基本使用
     */
    {
        std::thread t1(func);
        std::thread t2(func);
        std::thread t3(func);

        t1.join();
        t2.join();
        t3.join();
    }
    {
        std::thread t1(func1);
        std::thread t2(func1);
        std::thread t3(func1);

        t1.join();
        t2.join();
        t3.join();
    }
    /*
     * 02 mutex 死锁
     */
    {
        struct Complex {
            std::mutex mutex_;
            int i;
            Complex(): i(0) {}

            void mul(int x) {
                std::lock_guard<std::mutex> lock(mutex_);
                i *= x;
            }

            void div(int x) {
                std::lock_guard<std::mutex> lock(mutex_);
                i /= x;
            }

            void both(int x, int y) {
                std::lock_guard<std::mutex> lock(mutex_);
                mul(x);
                div(y);
            }
        };

        cout << "死锁 before----------------\n";
        Complex complex;
        complex.both(32, 23);
        cout << "死锁 after----------------\n";
    }
    /*
     * 03 recursive_mutex
     */
    {
        struct Complex {
            std::recursive_mutex mutex_;
            int i;
            Complex(): i(0) {}

            void mul(int x) {
                std::lock_guard<std::recursive_mutex> lock(mutex_);
                i *= x;
            }

            void div(int x) {
                std::lock_guard<std::recursive_mutex> lock(mutex_);
                i /= x;
            }

            void both(int x, int y) {
                std::lock_guard<std::recursive_mutex> lock(mutex_);
                mul(x);
                div(y);
            }
        };

        cout << "before----------------\n";
        Complex complex;
        complex.both(32, 23);
        cout << "after----------------\n";
    }

    return 0;
}
