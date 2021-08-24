#include <thread>
#include <iostream>

using namespace std;
void func() {
}

int main(int argc, char* argv[]) {
    /*
        1 线程的基本用法
    */
    {
        // 获取当前信息
        thread t(func);
        cout << t.get_id() << endl;
        cout << thread::hardware_concurrency() << endl;
    }

    // 线程休眠
    std::this_thread::sleep_for(std::chrono::seconds(3));

    return 0;
}