#ifndef CPP_TEST__SYNC_QUEUE_H_
#define CPP_TEST__SYNC_QUEUE_H_

#include <iostream>
#include <list>
#include <mutex>
#include <thread>
#include <condition_variable>

template<typename T>
class SyncQueue {
public:
    SyncQueue(int max_size): max_size_(max_size), need_stop_(false) {}

    void Put(const T& x) {
        Add(x);
    }

    void Put(T&& x) {
        Add(std::forward<T>(x));
    }

    void Take(std::list<T>& list) {
        std::unique_lock<std::mutex> locker(mutex_);
        not_empty_.wait(locker, [this] { return need_stop_ || NotEmpty(); });
        if (need_stop_) {
            return;
        }

        list = std::move(queue_);
        m_not_full_.notify_one();
    }

    void Take(T& t) {
        std::unique_lock<std::mutex> locker(mutex_);
        not_empty_.wait(locker, [this] { return need_stop_ || NotEmpty(); });
        if (need_stop_) {
            return;
        }

        t = queue_.front();
        queue_.pop_front();
        m_not_full_.notify_one();
    }

    void Stop() {
        {
            std::lock_guard<std::mutex> locker(mutex_);
            need_stop_ = true;
        }
        m_not_full_.notify_all();
        not_empty_.notify_all();
    }

    bool Empty() const {
        std::lock_guard<std::mutex> locker(mutex_);
        return queue_.empty();
    }

    bool Full() {
        std::lock_guard<std::mutex> locker(mutex_);
        return queue_.size() == max_size_;
    }

    size_t Size() {
        std::lock_guard<std::mutex> locker(mutex_);
        return queue_.size();
    }

    int Count() {
        return queue_.size();
    }

private:
    bool NotFull() const {
        bool full = queue_.size() >= max_size_;
        if (full) {
            std::cout << "缓冲区满了，需要等待...." << std::endl;
        }
        return !full;
    }

    bool NotEmpty() const {
        bool empty = queue_.empty();
        if (empty) {
            std::cout << "缓冲区空了，需要等待...，异步层的线程ID: " << std::this_thread::get_id() << std::endl;
        }

        return !empty;
    }

template<typename F>
void Add(F&& x) {
    std::unique_lock<std::mutex> locker(mutex_);
    m_not_full_.wait(locker, [this]{return need_stop_ || NotFull(); });
    if (need_stop_) {
        return;
    }

    queue_.push_back(std::forward<F>(x));
    not_empty_.notify_one();
}

private:
    std::list<T> queue_; // 缓冲区
    std::mutex mutex_;   // 互斥量和条件变量结合起来使用
    std::condition_variable not_empty_; // 不为空的条件变量
    std::condition_variable m_not_full_;  // 没有满的条件变量
    int max_size_;      // 同步队列最大的size
    bool need_stop_;    // 停止的标志
};

#endif //CPP_TEST__SYNC_QUEUE_H_
