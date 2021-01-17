//
// Created by Gao Tianpeng on 2021/1/17.
//

#include "thread_pool.h"

const int max_task_count = 100;

ThreadPool::ThreadPool(int num_threads): m_queue(max_task_count) {
  Start(num_threads);
}

ThreadPool::~ThreadPool() {
  // 如果没有停止时则主动停止线程池
  Stop();
}

void ThreadPool::Start(int num_threads) {
  m_running = true;
  // 创建线程组
  for (int i = 0; i < num_threads; ++i) {
    m_thread_grouop.push_back(std::make_shared<std::thread>(&ThreadPool::RunInThead, this));
  }
}

void ThreadPool::Stop() {
  // 保证多线程情况下只调用一次StopThreadGroup
  std::call_once(m_flag, [this] { StopThreadGroup(); });
}

void ThreadPool::AddTask(Task &&task) {
  m_queue.Put(std::forward<Task>(task));
}

void ThreadPool::AddTask(const Task& task) {
  m_queue.Put(task);
}

void ThreadPool::RunInThead() {
  while (m_running) {
    // 取任务分别喝执行
    std::list<Task> list;
    m_queue.Take(list);
    for (auto& task: list) {
      if (!m_running) {
        return;
      }
      task();
    }
  }
}

void ThreadPool::StopThreadGroup() {
  m_queue.Stop(); // 让同步队列中的线程停止
  m_running = false;  // 置为false,让内部线程跳出循环并退出
  for (auto thread: m_thread_grouop) {
    if (thread) {
      thread->join();
    }
  }
  m_thread_grouop.clear();
}