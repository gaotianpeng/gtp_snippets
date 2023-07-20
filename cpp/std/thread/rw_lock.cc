#include <condition_variable>
#include <chrono>
#include <iostream>
#include <mutex>
#include <thread>
#include <vector>
#include <random>

class Math {
public:
    static double random() {
        static std::random_device rd;
        static std::default_random_engine engine(rd());
        static std::uniform_real_distribution<> distribution(0.0, 1.0);
        return distribution(engine); 
    }
};

class RWLock {
public:
    RWLock() : readers(0), writers(0), active_writers(0) {}

    void lockRead() {
        std::unique_lock<std::mutex> lock(mutex_);
        read_cv.wait(lock, [this] { return writers == 0; });
        ++readers;
    }

    void unlockRead() {
        std::unique_lock<std::mutex> lock(mutex_);
        --readers;
        if (readers == 0 && active_writers > 0) {
            write_cv.notify_one();
        }
    }

    void lockWrite() {
        std::unique_lock<std::mutex> lock(mutex_);
        ++writers;
        write_cv.wait(lock, [this] { return readers == 0 && active_writers == 0; });
        ++active_writers;
    }

    void unlockWrite() {
        std::unique_lock<std::mutex> lock(mutex_);
        --writers;
        --active_writers;
        read_cv.notify_all();
        write_cv.notify_one();
    }

private:
    std::mutex mutex_;
    std::condition_variable read_cv;
    std::condition_variable write_cv;
    int readers;
    int writers;
    int active_writers;
};

static RWLock rw_lock;

static int RandomVal(int val) {
    return (int)(Math::random() * val) + 1;
}

static void readFunc() {
    rw_lock.lockRead();
    std::cout << "Read Thread " << std::this_thread::get_id() << " is running" 
            << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(RandomVal(100)));
    rw_lock.unlockRead();
}

static void writeFunc() {
    rw_lock.lockWrite();
    std::cout << "Write Thread " << std::this_thread::get_id() << " is running" 
        << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(RandomVal(200)));
    rw_lock.unlockWrite();
}

void Test() {
    std::vector<std::thread> threads;
    int n_thead = 100;
    int i = 0;
    for (int i = 0; i < n_thead; ++i) {
        if (Math::random() < 0.8) {
            threads.emplace_back(readFunc);
        } else {
            threads.emplace_back(writeFunc);
        }
    }

    for (int i = 0; i < threads.size(); ++i) {
        threads[i].join();
    }
}

int main(int argc, char* argv[]) {
    std::cout << "test start ...." << std::endl;
    Test();
    std::cout << "test end" << std::endl;    
    return 0;
}
