#include <iostream>
#include <list>
#include <mutex>
#include <functional>
#include <random>
#include <vector>
#include "shared_mutex.h"

#define	N_BUCKETS	4
ema::shared_mutex<N_BUCKETS> sm;


static void Read(int x, int y) {
    ema::s_lock<N_BUCKETS> l_(sm);
    std::cout << "read thread first x: " <<  x << " second y: " << y << std::endl;
}

static void Write(int& x, int& y) {
    ema::x_lock<N_BUCKETS> l_(sm);
    ++x;
    y = y + 2; 
    std::cout << "write thread first x: " <<  x << " second y: " << y << std::endl;
}

class Math {
public:
    static double random() {
        static std::random_device rd;
        static std::default_random_engine engine(rd());
        static std::uniform_real_distribution<> distribution(0.0, 1.0);
        return distribution(engine); 
    }
};

static int RandomVal(int val) {
    return (int)(Math::random() * val) + 1;
}


static void readFunc() {
    ema::s_lock<N_BUCKETS> l_(sm);
    std::cout << "Read Thread " << std::this_thread::get_id() << " is running" 
            << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(RandomVal(100)));
}

static void writeFunc() {
    ema::x_lock<N_BUCKETS> l_(sm);
    std::cout << "Write Thread " << std::this_thread::get_id() << " is running" 
        << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(RandomVal(200)));
}

void Test() {
    std::vector<std::thread> threads;
    int n_thead = 100;
    int i = 0;
    int x = 0, y = 0;

    for (int i = 0; i < n_thead; ++i) {
        if (Math::random() < 0.8) {
            // threads.emplace_back(readFunc);
            threads.emplace_back(std::thread(Read, x, y));
        } else {
            // threads.emplace_back(writeFunc);
            threads.emplace_back(std::thread(Write, std::ref(x), std::ref(y)));
        }
    }

    for (int i = 0; i < threads.size(); ++i) {
        threads[i].join();
    }
}

int main(int argc, char *argv[]) {
    Test();
}

