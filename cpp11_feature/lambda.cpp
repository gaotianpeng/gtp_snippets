#include <iostream>
#include <string>
#include <functional>
using namespace std;

/*
 * C++11引入了lambda，允许inline函数的定义式被用作一个参数，或是一个local对象
 * 所谓lambda是一份功能定义式，可被定义于语句（statement）或表达式（expression）内部。因此你可以拿lambda当作inline函数使用
 */

std::function<int(int, int)> ReturnLambda() {
    return [] (int x, int y) {
        return x*y;
    };
}

int main(int argc, char* argv[]) {
    // 最简单的lambda示例
    [] {
        std::cout << "hello lambda" << std::endl;
    }();

    auto lam_fun = [] {
        std::cout << "hello lam_fun" << std::endl;
    };
    lam_fun();

    auto lam_fun1 = [] (const string& s) {
        std::cout << s << std::endl;
    };
    lam_fun1("hello lam_fun1");

    // 指名返回类型
    auto lam_fun2 = [] () -> double {
        return 1.0;
    };
    std::cout << "lam_fun2:" << lam_fun2() << std::endl;

    /*
     * 访问外部变量
     *   [=]意味着外部作用域以by value方式传递给lambda
     *   [&]意味着外部作用域以by reference方式传递给lambda
     */
    int x = 0;
    int y = 42;
    auto lam_fun3 = [x, &y] {
        std::cout << "x: " << x << std::endl;
        std::cout << "y: " << y << std::endl;
        ++y;    // OK
    };
    x = y = 77;
    lam_fun3();
    lam_fun3();
    std::cout << "final y: " << y << std::endl;

    /*
     * passing by value和passing by reference混合体,声明lambda为mutable
     */
    int id = 0;
    auto lam_fun4 = [id] () mutable {
        std::cout << "id: " << id << std::endl;
        ++id;   // Ok
    };
        /*
         * equals to
         *      class {
         *          private:
         *              int id; // copy of outside id
         *          public:
         *              void operator() () {
         *                  std::cout << "id: " << id << std::endl;
         *                  ++id;   // Ok
         *              }
         *       };
         */
    id = 42;
    lam_fun4();
    lam_fun4();
    lam_fun4();
    std::cout << id << std::endl;

    /*
     * lambda的类型：是个不具名function object
     * 每个lambda表达式的类型是独一无二的
     */
    auto lam_func5 = ReturnLambda();
    std::cout << lam_func5(6, 7) << std::endl;

    return 0;
}