#include <list>
#include <mutex>
#include <thread>
#include <condition_variable>
#include <iostream>

/*
 * 条件变量:阻塞一个或多个线程，直到收到另外一个线程发出的通知或者超时，才会唤醒当前阻塞的线程
 *  条件变量需要和互斥量配合起来使用
 *
 * 条件变量使用过程
 *  1）拥有条件变量的线程获取互斥量
 *  2）循环检查某个条件，如果条件不满足，则阻塞直到条件满足；如果条件满足，则向下执行
 *  3）某个线程满足条件执行完之后调用notify_one或notify_all唤醒一个或者所有等待线程
 */

#include <thread>
#include <iostream>
#include <list>
#include <condition_variable>
#include <mutex>

template<typename T>
class SyncQueue {
public:
    SyncQueue(int max_size): max_size_(max_size) {}

public:
    bool IsEmpty() {
        std::lock_guard<std::mutex> guard(mutex_);
        return queue_.empty();
    }

    bool IsFull() {
        std::lock_guard<std::mutex> guard(mutex_);
        return queue_.size() == max_size_;
    }

    void Put(const T& elem) {
        std::lock_guard<std::mutex> guard(mutex_);
        while (IsFull_()) {
            std::cout << "the queue is full, failed to add an elem." << std::endl;
            not_full_.wait(mutex_);
        }

        queue_.push_back(elem);
        not_empty_.notify_one();
    }

    void Get(T& elem) {
        std::lock_guard<std::mutex> guard(mutex_);
        while (IsEmpty_()) {
            std::cout << "the queue is empty, failed to get an elem." << std::endl;
            not_empty_.wait(mutex_);
        }

        elem = queue_.front();
        queue_.pop_front();
        not_full_.notify_one();
    }

    int Size() {
        std::lock_guard<std::mutex> guard(mutex_);
        return queue_.size();
    }

private:
    bool IsEmpty_() {
        return queue_.empty();
    }

    bool IsFull_() {
        return queue_.size() == max_size_;
    }

private:
    std::list<T> queue_;
    int max_size_;
    std::mutex mutex_;
    std::condition_variable_any not_empty_;
    std::condition_variable_any not_full_;
};

SyncQueue<int> sync_queue(5);

void PutDatas() {
    for (int i = 0; i < 20; ++i) {
    sync_queue.Put(888);
    }
}

void TakeDatas() {
    int x = 0;
    for (int i = 0; i < 20; ++i) {
        sync_queue.Get(x);
        std::cout << x << std::endl;
    }
}

int main(int argc, char* argv[]) {
    std::thread th1(PutDatas);
    std::thread th2(TakeDatas);

    th1.join();
    th2.join();

    return 0;
}
