#include <iostream>
#include <thread>
#include <future>
#include <chrono>
#include <random>
#include <list>
#include <exception>

using namespace std;

namespace async_normal {
    int doSomething(char c) {
        std::default_random_engine dre(c);
        std::uniform_int_distribution<int> id(100, 1000);
        for (int i = 0; i < 10; ++i) {
            this_thread::sleep_for(chrono::milliseconds(id(dre)));
            cout.put(c).flush();
        }
        return c;
    }

    int func1() {
        return doSomething('.');
    }

    int func2() {
        return doSomething('+');
    }

    /*
        async() 提供一个接口，让可调用对象，若是可能的话，在后台运行，成为一个独立线程
        future<> 允许你等待线程结束并获取其结果(正常结果或异常)
        调用async()并不保证传入的函数一定会被启动和结束，通过future<>.get 来保证
    */
    void run() {
        std::cout << "starting func1() in background" << 
        " and func2() in foreground: " << std::endl;

        /*
            future 是必要的
                1）允许你取得 传给async()的那个函数 的未来结果
                2）确保 传给async()的那个函数 或快或慢终会被调用
        */
        std::future<int> result1(std::async(func1));
        // std::future<int> result1(std::async(std::launch::async, fun1));
        // std::future<int> result1(std::async(std::launch::deferred, fun1));
        int result2 = func2();
        /*
            get被调用，以下三件事之一会发生
            1）如果func1() 启动于一个分离的线程，并且已经结束，你立刻获得其结果
            2）如果func1() 被启动但尚未结束，get() 会引发停滞，等待func1()结束后获得结棍
            3）如果func1() 尚未启动, 会强迫启动如同一个同步调用, get()会引发停滞直至产生结果
        */
        int result = result1.get() + result2;
        cout << "\nresult of func1() + func2(): " << result << std::endl;
    }
}

namespace async_exception {
    void task1() {
        list<int> v;
        while (true) {
            for (int i = 0; i < 1000000; ++i) {
                v.push_back(i);
            }
            cout.put('.').flush();
        }
    }

    void test() {
        cout << "starting 2 tasks" << endl;
        cout << "- task1: process endless loop of memory consumption " << endl;
        cout << "- tas21: wait for <return> and then for task1 " << endl;

        auto f1 = async(task1);
        cin.get();

        cout << "\n wait for the end of task1: " << endl;
        try {
            f1.get();
        } catch (const exception& e){
            cerr << "EXCEPTION: " << e.what() << endl;
        }
    }

    /*
        一个future<> 只能调用get()一次，调用之后future处于无效状态(用valid()来检测)
        future<> 提供接口，允许我们等待后台操作完成，而不需要处理其结果
        wait()
        wait_for()
        wait_until()
    */
}

int main(int argc, char* argv[]) {
    async_exception::test();
    return 0;
}