#include <iostream>
#include <list>
#include <mutex>
#include <functional>
#include <random>
#include <vector>
#include <chrono>
#include <condition_variable>
#include <shared_mutex>

#include "shared_mutex.h"

#define	N_BUCKETS	4


#define R_SLEEP_TIME 100
#define W_SLEEP_TIME 300

class ScopedTimer {
public:
    ScopedTimer(const std::string& msg):t0(std::chrono::high_resolution_clock::now()), msg_(msg) {}

    ~ScopedTimer() {

    }

    auto GetTick() {
        auto t1 = std::chrono::high_resolution_clock::now();
        auto nanos = std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count();
        return nanos;
    }

private:
    std::chrono::high_resolution_clock::time_point  t0;
    std::string msg_;
};


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

ema::shared_mutex<N_BUCKETS> sm;

static void Read(int x, int y) {
    ema::s_lock<N_BUCKETS> l_(sm);
    std::cout << "read thread first x: " <<  x << " second y: " << y << std::endl;
    std::this_thread::sleep_for(std::chrono::microseconds(R_SLEEP_TIME));
}
 
static void Write(int& x, int& y) {
    ema::x_lock<N_BUCKETS> l_(sm);
    ++x;
    y = y + 2; 
    std::cout << "write thread first x: " <<  x << " second y: " << y << std::endl;
    std::this_thread::sleep_for(std::chrono::microseconds(W_SLEEP_TIME));
}


static RWLock rw_lock;


static void Read1(int x, int y) {
    rw_lock.lockRead();
    std::cout << "read thread1 first x: " <<  x << " second y: " << y << std::endl;
        std::this_thread::sleep_for(std::chrono::microseconds(R_SLEEP_TIME)); 
    rw_lock.unlockRead();
}
 
static void Write1(int& x, int& y) {
    rw_lock.lockWrite();
    ++x;
    y = y + 2; 
    std::cout << "write thread1 first x: " <<  x << " second y: " << y << std::endl;
    std::this_thread::sleep_for(std::chrono::microseconds(W_SLEEP_TIME));
    rw_lock.unlockWrite();
}

static std::shared_mutex mutex_;

static void Read2(int x, int y) {
    std::shared_lock<std::shared_mutex> lock(mutex_);
    std::cout << "read thread2 first x: " <<  x << " second y: " << y << std::endl;
        std::this_thread::sleep_for(std::chrono::microseconds(R_SLEEP_TIME));
}
 
static void Write2(int& x, int& y) {
    std::unique_lock<std::shared_mutex> lock(mutex_);
    ++x;
    y = y + 2; 
    std::cout << "write thread2 first x: " <<  x << " second y: " << y << std::endl;
    std::this_thread::sleep_for(std::chrono::microseconds(W_SLEEP_TIME)); 
    rw_lock.unlockWrite();
}

std::mutex uniq_lock;

static void Read3(int x, int y) {
    std::lock_guard<std::mutex> lock(uniq_lock);
    std::this_thread::sleep_for(std::chrono::microseconds(R_SLEEP_TIME));
    std::cout << "read thread3 first x: " <<  x << " second y: " << y << std::endl;
}
 
static void Write3(int& x, int& y) {
    std::lock_guard<std::mutex> lock(uniq_lock);
    ++x;
    y = y + 2; 
    std::cout << "write thread3 first x: " <<  x << " second y: " << y << std::endl;
    std::this_thread::sleep_for(std::chrono::microseconds(W_SLEEP_TIME));
}


int64_t t1 = 0;
int64_t t2 = 0;
int64_t t3 = 0;
int64_t t4 = 0;
int thread_num  = 10000;
double reader_ratio = 0.9;

void Test() {
    ScopedTimer timer("self define shared mutex");
    std::vector<std::thread> threads;
    int i = 0;
    int x = 0, y = 0;

    for (int i = 0; i < thread_num; ++i) {
        if (Math::random() < reader_ratio) {
            threads.emplace_back(std::thread(Read, x, y));
        } else {
            threads.emplace_back(std::thread(Write, std::ref(x), std::ref(y)));
        }
    }

    for (int i = 0; i < threads.size(); ++i) {
        threads[i].join();
    }

    t1 = timer.GetTick();
}

void Test1() {
    ScopedTimer timer("self define 2 shared mutex");
    std::vector<std::thread> threads;
    int i = 0;
    int x = 0, y = 0;
    for (int i = 0; i < thread_num; ++i) {
        if (Math::random() < reader_ratio) {
            threads.emplace_back(std::thread(Read1, x, y));
        } else {
            threads.emplace_back(std::thread(Write1, std::ref(x), std::ref(y)));
        }
    }

    for (int i = 0; i < threads.size(); ++i) {
        threads[i].join();
    }
    t2 = timer.GetTick();
}


void Test2() {
    ScopedTimer timer("std shared mutex");
    std::vector<std::thread> threads;
    int i = 0;
    int x = 0, y = 0;
    for (int i = 0; i < thread_num; ++i) {
        if (Math::random() < reader_ratio) {
            threads.emplace_back(std::thread(Read2, x, y));
        } else {
            threads.emplace_back(std::thread(Write2, std::ref(x), std::ref(y)));
        }
    }

    for (int i = 0; i < threads.size(); ++i) {
        threads[i].join();
    }

    t3 = timer.GetTick();
}

void Test3() {
    ScopedTimer timer("mutext ");
    std::vector<std::thread> threads;
    int i = 0;
    int x = 0, y = 0;
    for (int i = 0; i < thread_num; ++i) {
        if (Math::random() < reader_ratio) {
            threads.emplace_back(std::thread(Read3, x, y));
        } else {
            threads.emplace_back(std::thread(Write3, std::ref(x), std::ref(y)));
        }
    }

    for (int i = 0; i < threads.size(); ++i) {
        threads[i].join();
    }

    t4 = timer.GetTick();
}


int main(int argc, char *argv[]) {
    Test();
    Test1();
    Test2();
    Test3();

    std::cout << std::endl << std::endl << std::endl;
    std::cout << "-- 9000 read thread, 1000 write thread --" << std::endl;
    std::cout << "open source shared mutex   :" << t1  << " ms" << std::endl;
    std::cout << "chatgpt code shared mutex  :" << t2  << " ms" << std::endl;
    std::cout << "c++17 shared mutex         :" << t3  << " ms" << std::endl;
    std::cout << "use mutex                  :" << t4  << " ms" << std::endl;
}

