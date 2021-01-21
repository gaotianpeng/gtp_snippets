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

template <typename T>
class SyncQueue {
 public:
  SyncQueue(int max_size): max_size_(max_size) {}

  void Put(const T& x) {
    std::lock_guard<std::mutex> lock(mutex_);
    while (IsFull()) {
      std::cout << "缓冲区满了，需要等待..." << std::endl;
      not_full_.wait(mutex_);
    }

    queue_.push_back(x);
    not_full_.notify_one();
  }

  void Take(T& x) {
    std::lock_guard<std::mutex> lock(mutex_);
    while (IsEmpty()) {
      std::cout << "缓冲区空了，需要等待..." << std::endl;
      not_empty_.wait(mutex_);
    }

    x = queue_.front();
    queue_.pop_front();
    not_full_.notify_one();
  }

  bool Empty() {
    std::lock_guard<std::mutex> lock(mutex_);
    return queue_.empty();
  }

  bool Full() {
    std::lock_guard<std::mutex> lock(mutex_);
    return queue_.size() == max_size_;
  }

  size_t Size() const {
    std::lock_guard<std::mutex> lock(mutex_);
    return queue_.size_;
  }

  int Count() const {
    return queue_.size();
  }

 private:
  bool IsFull() const {
    return queue_.size() == max_size_;
  }

  bool IsEmpty() const {
    return queue_.empty();
  }
 private:
  std::list<T> queue_;    // 缓冲区
  std::mutex mutex_;      // 互斥量和条件变量结合起来使用
  std::condition_variable_any not_empty_; // 不为空的条件亦是
  std::condition_variable_any not_full_;  // 没有满的条件变量
  int max_size_;          // 同步队列最大的size
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
    sync_queue.Take(x);
    std::cout << x << std::endl;
  }
}

int main(int argc, char* argv[]) {
  std::thread th1(PutDatas);
  std::thread th2(TakeDatas);

  th1.join();
  th2.join();
  system("pause");

  return 0;
}



