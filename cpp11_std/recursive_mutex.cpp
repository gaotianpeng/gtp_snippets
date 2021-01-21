#include <iostream>
#include <thread>
using namespace std;

/*
 * std::mutex
 *
 * std::recursive_mutex
 *    递归锁允许同一线程多次获得该互斥锁，可以用来解决同一线程需要多次获取互拆量时死锁的问题
 */
struct Complex {
  std::mutex mutex_;
  int i;

  Complex(): i(0) {}

  void mul(int x) {
    std::lock_guard<std::mutex> lock(mutex_);
    i *= x;
  }

  void div(int x) {
    std::lock_guard<std::mutex> lock(mutex_);
    i /= x;
  }

  void both(int x, int y) {
    std::lock_guard<std::mutex> lock(mutex_);
    mul(x);
    div(y);
  }
};

struct ComplexRecur {
  std::recursive_mutex mutex_;
  int i;

  ComplexRecur(): i(0) {}

  void mul(int x) {
    std::lock_guard<std::recursive_mutex> lock(mutex_);
    i *= x;
  }

  void div(int x) {
    std::lock_guard<std::recursive_mutex> lock(mutex_);
    i /= x;
  }

  void both(int x, int y) {
    std::lock_guard<std::recursive_mutex> lock(mutex_);
    mul(x);
    div(y);
  }
};

int main(int argc, char* argv[]) {
  {
    /*
     * 用recursive_mutex解决单一线程内多次获取同一锁的死锁问题
     */
    ComplexRecur complex;
    cout << "ComplexRecur start ---------------------------" << endl;
    complex.both(32, 23);
    cout << "ComplexRecur end    ---------------------------" << endl;
  }
  {
    /*
     * 运行起来后发生死锁
     *  both时获取了互斥量，之后再调用mul又要获取相同的互斥量，
     *  但是这个互斥量已经被当前线程获取了，无法释放，这时就会发生死锁
     */
    Complex complex;
    cout << "Complex start ---------------------------" << endl;
    complex.both(32, 23);
    cout << "Complex end    ---------------------------" << endl;
  }

  return 0;
}