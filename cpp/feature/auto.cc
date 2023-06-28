#include <iostream>
using namespace std;
/*
    c++ cv 限定符：const、volatile、mutable

    auto 推导规则, auto 可以同指针、引用结合起来使用，还可以带上cv限定符
    1）当不声明为指针为引用时，auto的推导结棍和初始化表达式抛弃引用和cv限定符后类型一致
    2）当声明为指针或引用时，auto的推导结棍将保持初始化表达式的cv属性


    auto 所修饰的变量必须被初始化

    auto 限制
    1）auto 不能用于函数参数
    2）auto 不能用于非静态成员变量
    3）auto 无法定义数组
    4）auto 无法推导出模板参数
*/

static void test() {
    int x = 0;
    auto* a = &x; // a -> int*, auto 为 int
    auto b = &x;  // b -> int*, auto 为 int*
    auto& c = x;  // c ->int&, auto 为 int
    auto d = c;     // d -> int, auto 为 int
}

int main(int argc, char* argv[]) {
    test();
    return 0;
}