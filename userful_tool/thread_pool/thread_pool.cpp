#include "thread_pool.h"

const int max_task_count = 100;

ThreadPool::ThreadPool(int num_threads): queue_(max_task_count) {
    Start(num_threads);
}

ThreadPool::~ThreadPool() {
    // 如果没有停止时则主动停止线程池
    Stop();
}

void ThreadPool::Start(int num_threads) {
    running_ = true;
    // 创建线程组
    for (int i = 0; i < num_threads; ++i) {
        thread_grouop_.push_back(std::make_shared<std::thread>(&ThreadPool::RunInThead, this));
    }
}

void ThreadPool::Stop() {
    // 保证多线程情况下只调用一次StopThreadGroup
    std::call_once(flag_, [this] { StopThreadGroup(); });
}

void ThreadPool::AddTask(Task &&task) {
    queue_.Put(std::forward<Task>(task));
}

void ThreadPool::AddTask(const Task& task) {
    queue_.Put(task);
}

void ThreadPool::RunInThead() {
    while (running_) {
        // 取任务分别喝执行
        std::list<Task> list;
        queue_.Take(list);
        for (auto& task: list) {
            if (!running_) {
                return;
            }
            task();
        }
    }
}

void ThreadPool::StopThreadGroup() {
    queue_.Stop(); // 让同步队列中的线程停止
    running_ = false;  // 置为false,让内部线程跳出循环并退出
    for (auto thread: thread_grouop_) {
        if (thread) {
            thread->join();
        }
    }
    thread_grouop_.clear();
}