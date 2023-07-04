#include "thread_pool.h"
#include <atomic>
#include <chrono>
#include <condition_variable>
#include <iostream>
#include <mutex>
#include <queue>
#include <thread>

class ThreadPool::Impl {
public:
    Impl(int num):thread_num_(num) {
        is_available_.store(true);
        is_stop_.store(false);
        active_thread_num_.store(0);
    }

    bool Start() {
        if (!IsAvailable()) {
            is_available_.store(true);
            is_stop_.store(false);
            active_thread_num_.store(0);
            worker_.clear();
        }

        for (int i = 0; i < thread_num_; ++i) {
            worker_.emplace_back(
                [this]() {
                    for (;;) {
                        std::function<void()> task;
                        {
                            if (is_stop_.load()) {
                                break;
                            }
                            std::unique_lock<std::mutex> locker(task_mutex_);
                            task_cv_.wait(locker);

                            if (!task_queue_.empty()) {
                                task = std::move(task_queue_.front());
                                task_queue_.pop();
                            }
                        }
                        if (task) {
                            task();
                        }
                    }
                    active_thread_num_--;
                }
            );
            ++active_thread_num_;
        }
        return true;
    }

    bool Stop() {
        is_stop_.store(true);
        is_available_.store(false);
        while (active_thread_num_.load() > 0) {
            task_cv_.notify_all();
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
        
        for (auto& elem: worker_) {
            if (elem.joinable()) {
                elem.join();
            }
        }
        return true;
    }

    bool Clear() {
        std::lock_guard<std::mutex> lock(this->task_mutex_);
        while (!task_queue_.empty()) {
            task_queue_.pop();
        }

        return true;
    }

    bool IsFinish() {
        std::unique_lock<std::mutex> locker(task_mutex_);
        return task_queue_.empty();
    }

    void Notify() {
        this->task_cv_.notify_all();
    }
    
    void AddTask(std::function<void()> task) {
        if (!IsAvailable()) {
            return;
        }

        {
            std::lock_guard<std::mutex> lock(this->task_mutex_);
            task_queue_.emplace(task);
        }
        this->task_cv_.notify_one();
    }
    
    bool IsAvailable() {
        return is_available_.load();
    }

private:
    int thread_num_;
    std::atomic_bool is_available_;
    std::atomic_bool is_stop_;
    std::atomic_int active_thread_num_;
    std::mutex task_mutex_;
    std::condition_variable task_cv_;
    std::queue<std::function<void()>> task_queue_;
    std::vector<std::thread> worker_;
};

ThreadPool::ThreadPool(int num) {
    impl_.reset(new Impl(num));
}

bool ThreadPool::Start() {
    return impl_->Start();
}

bool ThreadPool::Stop() {
    return true;
}

bool ThreadPool::Clear() {
    return impl_->Clear();
}

bool ThreadPool::IsFinish() {
    return impl_->IsFinish();
}

void ThreadPool::Notify() {
    impl_->Notify();
}

void ThreadPool::AddTask(std::function<void()> func) {
    impl_->AddTask(func);
}

bool ThreadPool::IsAvailable() {
    return impl_->IsAvailable();
}
