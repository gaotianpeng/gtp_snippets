#include <iostream>
#include <vector>
using namespace std;
/*
    auto 所修饰的变量必须被初始化，编译器需要通过初始化 auto 所代表的类型
    若希望得到类型，而不需要或不能定义变量的时候，该怎么办，
    decltype,用来在编译时推导出一个表达式的类型
    decltype 的推导过程是在编译期完成的，并不会真正计算表达式的值

    decltype 于一般的标记符表达式（id-expression），decltype将精确地推导出表达式定义本身的类型，
    不会像auto那样在某些情况下舍弃掉引用和cv限定符

    指针（*）、引用（＆）属于说明符（declarators），在定义的时候，是和变量名，
    而不是类型标识符（type-specifiers）相结合的


    decltype(exp)的推导规则
    1）exp是标识符、类访问表达式，decltype(exp)和exp的类型一致
    2）exp是函数调用，decltype(exp)和返回值的类型一致
    3）其他情况，若exp是一个左值，则decltype(exp)是exp类型的左值引用，否则和 exp 类型一致
*/
namespace Decltype {
class Foo {
public:
    static const int Number = 0;
    int x;
};
static void test() {
    // 基本使用
    {
        int x = 0;
        decltype(x) y = 1;          // y -> int
        decltype(x + y) z = 0;      // z -> int

        const int& i = x;
        decltype(i) j = y;          // j -> const int &
        const decltype(z) *p = &z;  // *p const int, p -> const int *

        decltype(z) * pi = &z;      // *pi -> int, pi -> int* 
        decltype(pi)* pp = &pi;     // *pp -> int*, pp -> int**
    }

    // decltype 作用于标识符和类访问表达式示例
    {
        int n = 0;
        volatile const int& x = n;
        decltype(n) a = n;              // a -> int
        decltype(x) b = n;              // b -> const volatile int &
        decltype(Foo::Number) c = 0;    // c -> const int
        Foo foo;
        decltype(foo.x) d = 0;          // d -> int
    }

    // decltype 作用于函数调用示例
    {
        int& func_int_r();
        int&& func_int_rr();
        int func_int(void);
        const int& func_cint_r();
        const int&& func_cint_rr();
        const int func_cint();
        const Foo func_cfoo();

        int x = 0;
        // decltype 的结果和函数的返回类型保持一致
        decltype(func_int_r()) a1 = x;      // a1 -> int &
        decltype(func_int_rr()) b1 = 0;     // b1 -> int &&
        decltype(func_int()) c1 = 0;        // c1 -> int
        decltype(func_cint_r()) a2 = x;     // a2 -> const int &
        decltype(func_cint_rr()) b2 = 0;    // b2 -> const int &&
        decltype(func_cint()) c2 = 0;       // c2 -> int
        decltype(func_cfoo()) ff = Foo();   // ff -> const Foo
    }
    
    // decltype 作用于带括号的表达式和加法运算表达式
    {
        const Foo foo = Foo();
        decltype(foo.x) a = 0;      // a -> int
        decltype((foo.x)) b = a;    // b -> const int &
        int n = 0, m = 0;
        decltype(n + m) c = 0;      // c -> int
        decltype(n += m) d = c;     // d -> int&
    }
}

}
namespace DecltypeApp {

template <typename ContainerT>
class Foo {
    typename ContainerT::iterator it_;
public:
    void func(ContainerT& container) {
        it_ = container.begin();
    }
};

// ContainerT::iterator并不能包括所有的迭代器类型，
// 当ContainerT是一个const类型时，应当使用const_iterator
template <typename ContainerT>
class Foo<const ContainerT> {
    typename ContainerT::const_iterator it_;
public:
    void func(const ContainerT& container) {
        it_ = container.begin();
    }
};

// decltype 应用于泛型编程
template <typename ContainerT>
class Foo1{
    decltype(ContainerT().begin()) it_;
public:
    void func(const ContainerT& container) {
        it_ = container.begin();
    }
};

// decltype 用于返回类型后置语法
// 泛型编程中，可能需要通过参数的运算来得到返回值的类型
template <typename R, typename T, typename U>
R add(T t, U u) {
    return t + u;
}

template <typename T, typename U>
decltype(T() + U()) add1(T t, U u) {
    return t + u;
}

template <typename T, typename U>
decltype((*(T*)0) + (*(U*)0)) add2(T t, U u) {
    return t + u;
}

// c++ 11 新增了返回后置(trailing-return-type)语法
// 回值类型后置语法，是为了解决函数返回值类型依赖于参数而导致难以确定返回值类型的问题
template <typename T, typename U>
auto add3(T t, U u) -> decltype(t+u) {
    return t + u;
}

static void test() {
    {
        typedef const std::vector<int> container_t;
        container_t arr;
        Foo<container_t> foo;
        foo.func(arr);
    }
    {
        typedef const std::vector<int> container_t;
        container_t arr;
        Foo1<container_t> foo;
        foo.func(arr);
    }
    {
        int a = 1;
        float b = 2.0;
        // 外部不知道参数之间应该如何运算，只有 add函数知道返回值应该如何推导
        auto c = add<decltype(a + b)>(a, b);
    }
    {
        auto ret = add<int>(1, 1);
        auto ret1 = add3(1, 1.2);
    }
}

} // namespace DecltypeApp

int main(int argc, char* argv[]) {
    Decltype::test();
    DecltypeApp::test();
    return 0;
}