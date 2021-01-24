#include <iostream>
#include <atomic>
#include <thread>

struct AtomicCounter {
    std::atomic<int> value;
    void increment() {
        ++value;
    }

    void decrement() {
        --value;
    }

    int get() {
        return value.load();
    }
};

int main(int argc, char* argv[]) {
    return 0;
}