#include <iostream>
#include <string>
using namespace std;

/*
static_cast
    任何具有明确定义的类型转换，只要不包含底层const都可以使用
    一个较大的算术类型赋值给较小的类型时，static_cast非常有用

const_cast
    只能改变运算对象的底层const
    对于将常量对象转换成非常量对象的行为，称为去掉const性质
    常常用于有函数重载的上下文

reinterpret_cast
    通常为运算对象的位模式提供较低层次上的重新解释

dynamic_cast
    通常情况下要转换的类型类型应该含有虚函数
        dynamic_cast<type*>(e):e必须是一个有效的指针
        dynamic_cast<type&>(e):e必须是一个左值
        dynamic_cast<type&&>(e):e不能是左值
            type必须是一个类类型，并且通常情况下该类型应该含有虚函数
        e类型必须符合以下三个条件中的任意一个
            e的类型是目标type的公有派生类
            e的类型是目标type的公有基类
            e的类型就是目标type的类型
        当对引用的类型转换失败时，程序抛出一个名为std::bad_cast的异常
*/

class Base {
public:
    virtual void Print() {}
};

class Derived : public Base {
public:
    virtual void Print() {}
};

void DynamicTest(const Base& base) {
    try {
        const Derived& d = dynamic_cast<const Derived&>(base);
    }	
    catch (bad_cast) {
        // 处理类型转换失败的情况
    }
}

int main(int argc, char* argv[]) {
    // static cast 
    int a_int = 0;
    double b_double = 13.7439339;
    a_int = static_cast<int>(b_double);
    cout << a_int << endl;

    void *p = &b_double;
    double* dp = static_cast<double*>(p);

    // const_cast
    const char* pc = "gtp";
    char* p_char = const_cast<char*>(pc);
    static_cast<string>(pc);	// ok: 字符串字面值转换成string类型
    //const_cast<string>(pc);	// error: const_cast只改变常量属性

    // reinerpret_cast
    int* ip = nullptr;
    char* ch_p = reinterpret_cast<char*>(ip);

    // dynamic_cast
    Base base;
    DynamicTest(base);

    return 0;
}