#include <iostream>
using namespace std;
/*
    R-value reference: T&&

    什么是左值？什么是右值？看能不能对表达式取地址
    1) 左值：表达式结束后依然存在的持久对象
    2) 右值：表达式结束时就不再存在的临时对象

    右值由两个概念构成
    1）将亡值(xvalue, expiring value)：与右值引用相关的表达式
        将要被移动的对象
        T&&函数返回值
        std::move返回值
        转换为T&&的类型的转换函数的返回值
    2）纯右值(prvalue, PureRvalue)
        非引用返回的临时变量
        运算表达式产生的临时变量
        原始字面量
        lambda表达式

    &&特性：右值引用就是对一个右值进行引用的类型，右值不具名，只能通过引用的方式找到它
        通过右值引用的声明，该右值又“重获新生”，生命周期与右值引用类型变量的生命周期一样
*/

int main(int argc, char* argv[]) {
    cout << "hello world!" << endl;
    return 0;
}