#include <iostream>
#include <atomic>

using namespace std;

class spinlock_mutex {
public:
    spinlock_mutex(): flag(ATOMIC_FLAG_INIT) {
    }

    void lock() {
        while (flag.test_and_set(std::memory_order_acquire));
    }

    void unlock() {
        flag.clear(std::memory_order_release);
    }

private:
    std::atomic_flag flag;
};

int main(int arg, char* argv[]) {


    return 0;
}