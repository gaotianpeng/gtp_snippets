#include <iostream>
#include <thread>
#include <mutex>
#include <shared_mutex>
#include <vector>

using namespace std;

using ReadLock = std::shared_lock<std::shared_mutex>;
using WriteLock = std::unique_lock<std::shared_mutex>;

int a = 10;

std::shared_mutex rwlock;

void Read() {
    ReadLock read_lock(rwlock);
    cout << std::this_thread::get_id() << " read : " <<a << endl;
    std::this_thread::sleep_for(std::chrono::microseconds(100));
}

void Write() {
    WriteLock write_lock(rwlock);
    a = 20;
    cout << std::this_thread::get_id() << " write : " << a << endl;
}

void test() {
    std::vector<std::unique_ptr<std::thread>> threads;

    for (int i = 0; i < 10; ++i) {
        threads.push_back(std::make_unique<std::thread>(Read));
    }

    threads.push_back(std::make_unique<std::thread>(Write));
    
    for (int i = 0; i < 10; ++i) {
        threads.push_back(std::make_unique<std::thread>(Read));
    }

    for (int i = 0; i < threads.size(); ++i) {
        threads[i]->join();
    }
}

int main(int argc, char* argv[]) {
    test();
    return 0;
}
