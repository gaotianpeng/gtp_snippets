#include <iostream>
using namespace std;

/*
    lambda 来源于函数式编程的概念
    lambda 表达式定义了一个匿名函数，并且可以捕获一定范围内的变量

    [capture] (params) opt -> ret { body; };
        capture 是捕获列表
        params 是参数表
        opt 是函数选项
        ret 是返回值类型
        body 是函数体
    
    C++11 中允许省略 lambda 表达式的返回值定义

    [] 不捕获任何变量
    [&] 捕获外部作用域中所有变量，并作为引用在函数体中使用(按引用捕获)
    [=] 捕获外部作用域中所有变量，并作为副本在函数体中使用(按值捕获)
    [=, &foo] 按值捕获外部作用域中所有变量，并按引用捕获foo变量
    [bar] 按值捕获bar变量，同时不捕获其他变量
    [this] 捕获当前类中的this指针，让lambda表达式拥有和当前类成员函数同样的访问权限。
        如果已经使用了＆或者=，就默认添加此选项。捕获this的目的是可以在lamda中使用当前类的成员函数和成员变量

*/

namespace Lambda {

void test() {
    {
        auto f = [](int a) -> int {
            return a + 1;
        };
        cout << f(1) << endl;
    }
    {
        // 省略返回类型
        auto f1 = [](int a) {
            return a + 1;
        };

        cout << f1(1) << endl;
    }
    {
        auto f1 = []() { return 1; };
        auto f2 = [] { return 1; };
        cout << f1() << endl;
        cout << f2() << endl;
    }
}

}  // namespace Lambda

int main(int argc, char* argv[]) {
    Lambda::test();
    return 0;
}