#include <iostream>
#include <atomic>
#include <thread>
using namespace std;

/*
    template<typename T>    
    class atomic: public atomics::detail::base_atomic<T> {
    public:
        atomic() = default; //默认构造      
        explicit atomic(T v); //指定初始值构造
        atomic(atomic const&) = delete;  
        
        //禁止对象间拷贝
        atomic& operator=(atomic const&) = delete;   //禁止对象间赋值

    public:
        bool  is_lock_free(); //是否无锁（lock-free)
        void store(T value);  //存值
        T load();     //取值
        T operator=(T v) volatile;  //赋值，相当于store      
        operator    T() volatile const ; //隐式类型转换，相当于load
        T           exchange(T new_value);     //存值，同时返回原值
        bool        compare_exchange_weak(T & expected, T desired);
        bool        compare_exchange_strong(T &expected, Tdesired);
        T&          storage() ;          //获取内部的值
        T const&    storage() const ;    
    };
    atomic可以把对类型T的操作原子化，但不是任意的类型都可以原子化的


*/

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