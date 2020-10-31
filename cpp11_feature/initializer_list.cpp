#include <iostream>
#include <vector>
#include <string>
#include <complex>

/*
 * 用户自定应急类型之初值列(initializer lists for user-defined types)
 * C++11 提供了 class template std::initializer_list<>用来支持以一系列值(a list of values)进行初始化
 */
void print(std::initializer_list<int> vals) {
    for (auto p = vals.begin(); p != vals.end(); ++p) {
        std::cout << *p << std::endl;
    }
}

int main(int argc, char* argv[]) {
    /*
     * 一致性初始化(uniform initialization): 面对任何初始化动作，你可以使用相同的语法，即使用大括号
     */
    int values[] {1, 2, 3};
    std::vector<int> vec{2, 3, 5, 7, 11, 13, 17};
    std::vector<std::string> cities {"beijing", "shanghai", "guangzhou"};
    std::complex<double> cpx{4.0, 3.0};

    /*
     * 初值列(initializer list):value initialization,即使某个local变量属于某种基础类型也会
     *      被初始化为0
     */
    int i;      // i has undefined value
    int j{};    // j is initialized by 0
    int* p;     // p has undefined value
    int* q{};   // q is initialized by nullptr

    /*
     * 窄化(narrowing):精度降低或造成数值变动对大括号而言是不可成立的
     */
    int x1(5.3);    // ok
    int x2 = 5.3;   // ok
    int x3{5.3};    // error: narrowing
    int x4 = {5.3}; // error: narrowing
    char c1{7};     // ok
    char c2{99999}; // error: narrowing
    std::vector<int> v1{1, 2, 4, 5};    // ok
    std::vector<int> v2{1, 2.3, 4, 5.6};// error: narrowing

    // initializer lists for user-defined types
    print({12, 3, 5, 7, 11, 13, 17});
    return 0;
}
