#include <iostream>
#include "thread_pool.h"
using namespace std;

void TestThreadPool() {
    ThreadPool pool;
    pool.Start(2);
    std::thread th1([&pool] {
        for (int i = 0; i < 10; i++) {
            auto th_id = this_thread::get_id();
            pool.AddTask([th_id] {
                cout << "同步层线程1的线程ID: " << th_id << endl;
            });
        }
    });

    std::thread th2([&pool] {
        for (int i = 0; i < 10; i++) {
            auto th_id = this_thread::get_id();
            pool.AddTask([th_id] {
                cout << "同步层线程2的线程ID:"<< th_id << endl;
            });
        }
    });

    this_thread::sleep_for(std::chrono::seconds(2));
    getchar();
    pool.Stop();
    th1.join();
    th2.join();
}

int main(int argc, char* argv[]) {
    TestThreadPool();
    return 0;
}
