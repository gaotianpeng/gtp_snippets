#include <thread>
#include <iostream>
#include <list>
#include <condition_variable>
#include <mutex>

template<typename T>
class SimpleSyncQueue {
public:
    SimpleSyncQueue() {}

public:
    bool IsEmpty() {
        std::lock_guard<std::mutex> guard(mutex_);
        return queue_.empty();
    }

    int Size() {
        std::lock_guard<std::mutex> guard(mutex_);
        return queue_.size();
    }

    void Put(const T& elem) {
        std::lock_guard<std::mutex> guard(mutex_);
        queue_.push_back(elem);
        std::cout << "Put " << elem << " ";
        not_empty_.notify_one();
    }

    void Get(T& elem) {
        std::unique_lock<std::mutex> guard(mutex_);
        not_empty_.wait(guard, [this] {
            return !queue_.empty();// return false 则继续等
        });
        elem = queue_.front();
        queue_.pop_front();
    }

private:
    std::list<T> queue_;
    std::mutex mutex_;
    std::condition_variable not_empty_;
};

SimpleSyncQueue<int> syncQueue;

void PutDatas()
{
    for (int i = 0; i < 20; ++i)
    {
        syncQueue.Put(i);
    }
    std::cout << std::endl;
}

void TakeDatas()
{
    int x = 0;

    for (int i = 0; i < 20; ++i)
    {
        syncQueue.Get(x);
        std::cout << x << " ";
    }
    std::cout << std::endl;
}

int main(int argc, char* argv[]) {
    for (int i  = 0; i < 1000; i++) {
        std::thread t1(PutDatas);
        std::thread t2(TakeDatas);

        t1.join();
        t2.join();
    }

    return 0;
}