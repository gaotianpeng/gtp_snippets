#include <iostream>
#include <thread>
#include <future>
#include <mutex>
#include <utility>
using namespace std;

/*
  std::future:作为异步结果的传输通道，可以方便地获取线程函数的返回值
  std::promise:用来包装一个值，将数据和future绑定起来，方便线程赋值
  std::package_task:用来包装一个可调用对象，将函数和future绑定起来，以便异步调用

  三者间的关系：
    std::future提供了一个访问异步操作结果的机制，它和线程是一个级别的，属于低层次的对象
    std::future之上的高一层是std::promise, std::package_task,内部都有future以便访问异步操作结果
    std::package_task包装的是一个异步操作，std::promise包装的是一个值
 */

int func(int x) {
  return x + 2;
}

int main(int argc, char* argv[]) {
  std::promise<int> pr;
  std::thread th([] (std::promise<int>& p) {
    p.set_value_at_thread_exit(9);
  }, std::ref(pr));

  std::future<int> f = pr.get_future();
  auto r = f.get();
  cout << r << endl;

  th.join();

  cout << "-----------------------------" << endl;
  std::packaged_task<int(int)> tsk(func);
  std::future<int> fut = tsk.get_future();
  std::thread(std::move(tsk), 2).detach();
  int value = fut.get();
  cout << "The result is " << value << endl;

  return 0;
}