#include <iostream>
#include <future>
#include <deque>
#include <thread>
#include <utility>
using namespace std;

/*
    场景：候机厅等待准备登机通知

    future适用于等待一次性事件
    分为两类
    1) 唯一future
    2) 共享future

    future对象本身并不提供同步访问
    如果多个线程需要访问同一个future对象, 它们必须通过互斥元或其他同步机制来保护访问
*/

int FindTheAns() {
    return 88;
}

void DoOtherTask() {
    cout << "do other task" << endl;
}

void FutureTest() {
    future<int> the_ans = std::async(FindTheAns);
    DoOtherTask();
    cout << "The answer is " << the_ans.get() << endl;
}

/*
    packaged_task<>将一个future绑定到一个函数或可调用对象上
    当packaged_task<>对象被调用时，它就调用相关联的函数或可调用对象, 并且让future就绪，将返回值作为关联数据储存
*/
std::mutex mutex_;
std::deque<std::packaged_task<void()>> tasks;

bool gui_shutdown_message_received() {return false; }
void get_and_process_gui_message() {}

void gui_thread() {
    while (!gui_shutdown_message_received()) {
        get_and_process_gui_message();
        std::packaged_task<void()> task;
        {
            std::lock_guard<std::mutex> locker(mutex_);
            if (tasks.empty()) {
                continue;
            }
            task = std::move(tasks.front());
            tasks.pop_front();
        }
        task();
    }
}

std::thread gui_bg_thread(gui_thread);

template<typename Func>
std::future<void> post_task_for_gui_thread(Func f) {
    std::packaged_task<void()> task(f);
    std::future<void> res = tasks.get_future();
    std::lock_guard<std::mutex> locker(mutex_);
    tasks.push_back(std::move(task));
}

int main(int argc, char* argv[]) {
    FutureTest();
    return 0;
}