#ifndef CPP_TEST__THREAD_POOL_H_
#define CPP_TEST__THREAD_POOL_H_

#include <list>
#include <thread>
#include <functional>
#include <memory>
#include <atomic>
#include "sync_queue.h"

extern const int max_task_count;

class ThreadPool {
public:
    using Task = std::function<void()>;
    ThreadPool(int num_threads = std::thread::hardware_concurrency());
    ~ThreadPool();

    void Start(int num_threads);
    void Stop();
    void AddTask(Task&& task);
    void AddTask(const Task& task);

private:
    void RunInThead();
    void StopThreadGroup();

private:
    std::list<std::shared_ptr<std::thread>> thread_grouop_;  // 处理任务的线程组
    SyncQueue<Task> queue_;      // 同步队列
    std::atomic_bool running_;   // 是否停止的标志
    std::once_flag flag_;
};

#endif //CPP_TEST__THREAD_POOL_H_
