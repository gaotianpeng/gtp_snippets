#include <iostream>
#include <atomic>
#include <thread>

using namespace std;

struct AtomicCounter {
  std::atomic<int> value;
  void increment() {
    ++value;
  }

  void decrement() {
    --value;
  }

  int get() {
    return value.load();
  }
};

void ThreadFun(AtomicCounter* p_cnt) {
  p_cnt->increment();
}

int main(int argc, char* argv[]) {
  AtomicCounter counter;
  thread th1(ThreadFun, &counter);
  thread th2(ThreadFun, &counter);
  thread th3(ThreadFun, &counter);
  th1.join();
  th2.join();
  th3.join();
  cout << counter.get() << endl;
  return 0;
}