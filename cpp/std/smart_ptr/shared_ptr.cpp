#include <memory>      // 智能指针需要引用<memory>头文件
#include <iostream>

using namespace std;

void DeleteIntPtr(int* p) {
    delete p;
}

int main(int argc, char* argv[]) {
    /*
        01 基本使用
    */
    // 智能指针初始化
    {
        std::shared_ptr<int> p(new int(1));
        cout << p.use_count() << endl;
        std::shared_ptr<int> p2 = p;
        cout << p.use_count() << endl;
        cout << p2.use_count() << endl;
        p2.reset();
        cout << p.use_count() << endl;
        cout << p2.use_count() << endl;

        shared_ptr<int> ptr;
        ptr.reset(new int(1));
        if (ptr) {
            cout << "ptr is not null" << endl;
        }
    }

    {
        shared_ptr<int> ptr;
        ptr.reset(new int(1));
        // 获取原始指针
        int* p3 = ptr.get();
    }

    {
        // 指定删除器
        std::shared_ptr<int> p4(new int, DeleteIntPtr);
        std::shared_ptr<int> p5(new int, [](int* p) {
            delete p;
        });
    }

    /*
        02 使用时要注意的问题
    */
    {
        // 不要用一个原始指针初始化多个shared_ptr
        int* ptr = new int;
        shared_ptr<int> p1(ptr);
        shared_ptr<int> p2(ptr);    // error:会产生释放两次的问题
    }

    {
        /*
        不要在函数实参中创建shared_ptr
            function(shared_ptr<int>(new int), g())
            C++的函数参数的计算顺序在不同的编译器不同的调用约定下可能是不一样的
            可能先new int 后调用g(),如果g()发生异常，而shared_ptr<int>还没有创建，则int内存泄露
        */
    }

    {
        /*
        shared_from_this（）返回this指针
        不要将this指针作为shared_ptr返回出来，因为this指针本质上是一个裸指针，因此，这样可能会导致重复析构
        */
        // 错误做法
        {
            struct A {
                shared_ptr<A> GetSelf() {
                    return shared_ptr<A>(this);
                } 
            };
            // 重复析构
            shared_ptr<A> sp1(new A);
            shared_ptr<A> sp2 = sp1->GetSelf();
        }
        // 正确做法
        {
            class A: public std::enable_shared_from_this<A> {
                public:
                    std::shared_ptr<A> GetSelf() {
                        return shared_from_this();
                    }
            };
            shared_ptr<A> sp1(new A);
            shared_ptr<A> sp2 = sp1->GetSelf();
        }
    }

    {
        // 要避免循环引用
        struct A;
        struct B;
        struct A {
            std::shared_ptr<B> bptr;
            ~A() {
                cout << "A is deleted!" << endl;
            }
        };

        struct B {
            std::shared_ptr<A> aptr;
            ~B() { cout << "B is deleted!" << endl; }
        };

        {
            std::shared_ptr<A>  ap(new A);
            std::shared_ptr<B>  bp(new B);
            ap->bptr = bp;
            bp->aptr = ap;
        }
    }

    return 0;
}
