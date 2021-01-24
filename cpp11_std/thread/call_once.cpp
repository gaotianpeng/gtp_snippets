#include <iostream>
#include <thread>
#include <mutex>
using namespace std;

/*
 * 保证在多线程环境中某个函数仅被调用一次
 */

std::once_flag flag;

void do_once() {
  std::call_once(flag, []() {
    cout << "Called once" << endl;
  });
}

int main(int argc, char* argv[]) {
  std::thread th1(do_once);
  std::thread th2(do_once);
  std::thread th3(do_once);
  th1.join();
  th2.join();
  th3.join();

  return 0;
}