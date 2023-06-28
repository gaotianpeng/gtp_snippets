#include <iostream>
#include <functional>

using namespace std;

namespace Callable {
/*
    可调用函数对象
    1）函数指针
    2）operator() 成员函数的类对象（仿函数）
    3）可被转换为函数指针的类对象
    4）类成员函数指针
*/
void func() {
    cout << "函数指针" << endl;
}

struct Foo {
    void operator()(void) {
        cout << "仿函数" << endl;
    }
};

struct Bar {
    using fr_t = void(*)(void);
    static void func() {
        cout << "可被转换为函数指针的类对象" << endl;
    }

    operator fr_t(void) {
        return func;
    }
};

struct A {
    int a_;
    void mem_func() {
        cout << "类成员函数指针" << endl;
    }
};

void test() {
    // 1 函数指针
    void (*func_ptr)() = &func;
    func_ptr();

    // 2 仿函数
    Foo foo;
    foo();

    // 3 可被转换为函数指针的类对象
    Bar bar;
    bar();

    // 4 类成员函数指针
    void (A::*mem_func_ptr)() = &A::mem_func;
    A aa;
    (aa.*mem_func_ptr)();
}

} // namespace Callable

namespace Function {
/*
    可调用对象定义方法五花八门，C++11 通过提供 std::function 和 std::bind 统一了可调用对象的各种操作
    std::function 可以容纳除了类成员函数指针之外的所有可调用对象，用统一的方式处理函数、函数对象
    函数指针，并允许保存和延迟执行它们
*/
void func() {
    std::cout << __FUNCTION__ << std::endl;
}

class Foo {
public:
    static int foo_func(int a) {
        std::cout << __FUNCTION__ << "( " << a << ") ->: ";
        return a;
    }
};

class Bar {
public:
    int operator()(int a) {
        std::cout << __FUNCTION__ << "(" << a << ") ->: ";
        return a;
    }
};

// std::function 作为回调函数
class A {
    std::function<void()> callback_;
public:
    A(const std::function<void()>& f):callback_(f) {
    }

    void notify(void)  {
        callback_();
    }
};

class Foo1 {
public:
    void operator()() {
        std::cout << __FUNCTION__ << std::endl;
    }
};

// std::function 作为函数入参
void call_when_even(int x, const std::function<void(int)>& f) {
    if (!(x&1)) {
        f(x);
    }
}
void output(int x) {
    std::cout << x << " ";
}

void test() {
    // 绑定一个普通函数
    std::function<void(void)> fr1 = func;
    fr1();

    // 绑定一个类的静态成员函数
    std::function<int(int)> fr2 = Foo::foo_func;
    std::cout << fr2(123) << std::endl;

    // 绑定一个仿函数
    Bar bar;
    fr2 = bar;
    std::cout << fr2(123) << endl;

    // std::function 作为回调函数
    Foo1 foo;
    A aa(foo);
    aa.notify();

    // std::function 作为函数入参
    for (int i = 0; i < 10; ++i) {
        call_when_even(i, output);
    }
    cout << endl;
}

} // namespace Function

namespace Bind {
/*
    std::bind 用来将可调用对象与其参数一起进行绑定
    绑定后的结果可以使用std::function进行保存，并延迟调用到任何我们需要的时候
    两大作用
    1）将可调用对象与其参数一起绑定成一个仿函数
    2）将多元n可调用对象转成一元或者n-1元可调用对象，即只绑定部分参数
*/
void call_when_even(int x, const std::function<void(int)>& f) {
    if (!(x & 1)) {
        f(x);
    }
}
void output(int x) {
    std::cout << x << " ";
}

void output_add_2(int x) {
    std::cout << x + 2 << " ";
}

void output2(int x, int y) {
    std::cout << x << " " << y << std::endl;
}

class A {
public:
    int i_ = 0;
    void output(int x, int y) {
        std::cout << x << " " << y << std::endl;
    }
};

void test() {
    {
        // std::placeholders::_1是一个占位符，代表这个位置将在函数调用时，被传入的第一个参数所替代
        // std：：bind真正的返回类型是一个stl内部定义的仿函数类型，可以直接赋值给一个std::function
        auto fr = std::bind(output, std::placeholders::_1);
        for (int i = 0; i < 10; ++i) {
            call_when_even(i, fr);
        }
        std::cout << std::endl;
    }

    {
        auto fr = std::bind(output_add_2,
                    std::placeholders::_1);
        for (int i = 0; i < 10; ++i) {
            call_when_even(i, fr);
        }
        std::cout << std::endl;
    }

    {
        std::bind(output2, 1, 2)();
        std::bind(output2, std::placeholders::_1, 2)(1);
        std::bind(output2, 2, std::placeholders::_1)(1);

        std::bind(output2, std::placeholders::_1, std::placeholders::_2)(1, 2);
        std::bind(output2, std::placeholders::_2, std::placeholders::_1)(1, 2);
    }

    {
        A a;
        std::function<void(int, int)> fr = 
            std::bind(&A::output, &a, std::placeholders::_1, std::placeholders::_2);
        fr(1, 2);
        // std::bind将A的成员i_的指针和a绑定，返回的结果被放入std::function<int＆（void）>中存储，并可以在需要时修改访问这个成员
        std::function<int& (void)> fr_i = std::bind(&A::i_, &a);
        fr_i() = 123;
        std::cout << a.i_ << std::endl;
    }
}

} // namespace Bind

int main(int argc, char* argv[]) {
    Callable::test();
    Function::test();
    Bind::test();

    return 0;
}