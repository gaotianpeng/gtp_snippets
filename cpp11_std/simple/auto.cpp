/*
auto: 方便地获取复杂的类型，简化书写，提高编码效率
cv限定符(cv-qualifier)
  const
  volatile

1）当不声明为指针或引用时，auto的推导结果和初始化表达式抛弃引用和cv限定符后类型一致
2）当声明为指针或引用时，auto的推导结果将保持初始化的cv属性
3）auto是不能用于函数参数的
*/

#include <iostream>
using namespace std;

// Error: auto不能用于函数参数
void func(auto a = 1) {}

struct Foo {
  auto var1_ = 0;                 // Error: auto不能用于非静态成员变量
  static const auto var2_ = 0;    // OK
};

template <typename T>
struct Bar {};

int main(int argc, char* argv[]) {
  {
    auto x = 5;                         // OK: x is int
    auto pi = new auto(2);              // OK: pi is int*
    const auto *v = &x, u = 6;          // OK: v is const int*, u is const int
    static auto y = 0.0;
    // Error
    auto int r;
    /*
     * 'auto' requires an initializer
     *     auto声明变量必须马上初始化，以让编译器抢断出它的实际类型,并在编译时将auto占位符替换为真正的类型
     */
    auto s;
  }

  {
    int x = 0;
    auto *a = &x;     // a -> int*, auto == int
    auto b = &x;      // b -> int*, auto == int*
    auto& c = x;      // c -> int&, auto == int
    auto d = c;       // d -> int, auto == int: auto会把引用类型抛弃，直接推导成原始类型int
    const auto e = x; // e -> const int
    auto f = e;       // f -> int: auto会把const属性抛弃掉，推导成non-const类型int
    /*
     * 当auto和引用结合时，auto的推导将保留表达式的const属性
    */
    const auto& g = x;// g -> const int&
    auto& h = g;      // h -> const int&
  }

  {
    int arr[10] = {0};
    auto aa = arr;      // OK: aa -> int*
    auto rr[10] = arr;  // Error: auto 无法定义数组
    Bar<int> bar;
    Bar<auto> bb = bar; // Error: auto 无法推导模板参数
  }
  return 0;
}