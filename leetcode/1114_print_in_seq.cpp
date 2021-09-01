/*

public class Foo {
  public void first() { print("first"); }
  public void second() { print("second"); }
  public void third() { print("third"); }
}
三个不同的线程 A、B、C 将会共用一个 Foo 实例。

一个将会调用 first() 方法
一个将会调用 second() 方法
还有一个将会调用 third() 方法
请设计修改程序，以确保 second() 方法在 first() 方法之后被执行，third() 方法在 second() 方法之后被执行。

*/
#include <semaphore.h>
#include <iostream>
#include <thread>
using namespace std;

class Foo {
protected:
    sem_t sem1;
    sem_t sem2;

public:
    Foo() {
        sem_init(&sem1, 0, 0);    // 初始化两个信号量
        sem_init(&sem2, 0, 0);
    }

    void first() {
        cout << "first running..." << endl;
        sem_post(&sem1);
    }

    void second() {
        sem_wait(&sem1);    // 等待sem1结束
        cout << "second running..." << endl;
        sem_post(&sem2);    // 通知sem2可以开始了
    }

    void third() {
        sem_wait(&sem2);
        cout << "third running..." << endl;
    }
};

int main(int argc, char* argv[]) {
    Foo foo;
    thread th1(&Foo::first, &foo);
    thread th2(&Foo::second, &foo);
    thread th3(&Foo::third, &foo);

    th1.join();
    th2.join();
    th3.join();
    return 0;
}



