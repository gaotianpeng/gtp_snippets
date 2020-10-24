#include <iostream>
#include <vector>
#include <string>

double f(double in) {
    return in*in;
}

int main(int argc, char* argv[]) {
    auto i = 42; // i has type int
    auto d = f(10.0);   // d has type double

    // 以auto声明的变量，其类型会根据其初值被自动推导出来，因此一定需要一个初始化操作
    auto k; // error

    // 可为它加上额外的限定符
    static auto vat = 0.19;

    // 如果类型很长或表达式很复杂，auto特别有用
    std::vector<std::string> v;
	auto pos = v.begin();   // pos has type vector＜string＞::iterator
    auto lamba_ret = [](int x)-> bool {     // lamba_ret has the type of a lambda
        return true;                       // taking an int and returning a bool
    };

    std::cout << lamba_ret(10) << std::endl;
    return 0;
}
